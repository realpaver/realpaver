///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/LocalOptimizer.hpp"
#include "realpaver/Param.hpp"

#include "realpaver/Term.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

LocalOptimizer::LocalOptimizer(const Problem& pb)
    : pb_(std::make_shared<Problem>(pb)),
    n_(pb.nbVars()), m_(pb.nbCtrs()), s_(pb.scope() | pb.getObjective().getTerm().scope()),
    os_(pb.getObjective().getTerm().scope()),
    time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT")),
    iter_limit_(Param::GetIntParam("LOCAL_SOLVER_ITER_LIMIT"))
{
    bool ismin = pb_->getObjective().isMinimization();

    // objective function
    Term to = pb_->getObjective().getTerm();

    // objective function to be minimized
    Term tomin = ismin ? to : -to;

    // scope of the objective function
    to.makeScope(os_);

    // DAG
    std::shared_ptr<Dag> dag =  std::make_shared<Dag>();
    std::shared_ptr<Dag> odag = std::make_shared<Dag>();
    odag->insert(tomin);   // Add the objective expression to the objective dag
    obj_ = std::make_shared<RealFunction>(odag,0);

    // Add all constraints to the constraint dag
    for (size_t j=0; j<m_; j++)
    {
        dag->insert(pb.ctrAt(j));
    }
    if (dag->nbFuns()==0)
        ctrs_ = std::make_shared<RealFunctionVector>();
    else
        ctrs_ = std::make_shared<RealFunctionVector>(dag);

    // initial region
    reg_ = std::shared_ptr<IntervalRegion>(new IntervalRegion(s_));

    for (Variable v : s_)
        reg_->set(v, pb_->getDomain(v));

    start_ = std::shared_ptr<RealPoint>(new RealPoint(reg_->midpoint()));
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

LocalOptimizer::LocalOptimizer(const RealFunction& obj, const RealFunctionVector& ctrs)
    : m_(ctrs.nbFuns()), s_(ctrs.scope()), os_(obj.scope()), 
    time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT")),
    iter_limit_(Param::GetDblParam("LOCAL_SOLVER_ITER_LIMIT"))
{
    obj_ = std::make_shared<RealFunction>(obj);
    ctrs_ = std::make_shared<RealFunctionVector>(ctrs);
    n_ = os_.size();
    m_ = ctrs.nbFuns();
    start_ = nullptr;
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

LocalOptimizer::~LocalOptimizer()
{
}

double LocalOptimizer::timeLimit() const
{
   return time_limit_;
}

void LocalOptimizer::timeLimit(double val)
{
   time_limit_ = val;
}

size_t LocalOptimizer::iterLimit() const
{
   return iter_limit_;
}

void LocalOptimizer::iterLimit(size_t val)
{
   iter_limit_ = val;
}

size_t LocalOptimizer::nbVars() const
{
    return n_;
}

size_t LocalOptimizer::nbCtrs() const
{
    return m_;
}

Scope LocalOptimizer::scope() const
{
    return s_;
}

Scope LocalOptimizer::obj_scope() const
{
    return os_;
}

std::shared_ptr<RealFunction> LocalOptimizer::obj()
{
    return obj_;
}

std::shared_ptr<RealFunctionVector> LocalOptimizer::ctrs()
{
    return ctrs_;
}

// std::shared_ptr<Dag> LocalOptimizer::ctr_dag()
// {
//     return dag_;
// }
// std::shared_ptr<Dag> LocalOptimizer::obj_dag()
// {
//     return odag_;
// }

SharedIntervalRegion LocalOptimizer::region()
{
    return reg_;
}

void LocalOptimizer::region(const IntervalRegion& reg)
{
    reg_ = std::make_shared<IntervalRegion>(reg);
}

std::shared_ptr<RealPoint> LocalOptimizer::start()
{
    return start_;
}

void LocalOptimizer::start(const RealPoint& start)
{
    start_ = std::make_shared<RealPoint>(start);
}

double LocalOptimizer::bestVal() const
{
   return best_val_;
}

RealPoint LocalOptimizer::bestPoint() const
{
   return *best_;
}

std::shared_ptr<RealPoint> LocalOptimizer::bestPoint()
{
    return best_;
}

void LocalOptimizer::bestPoint(std::shared_ptr<RealPoint> best )
{
    best_ = best;
}

OptimizationStatus LocalOptimizer::status() const
{
   return status_;
}


} // namespace