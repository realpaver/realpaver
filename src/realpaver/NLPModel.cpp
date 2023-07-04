///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NLPModel.hpp"
#include "realpaver/Param.hpp"

#include "realpaver/Term.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

NLPModel::NLPModel(const Problem& pb)
    : n_(pb.nbVars()), m_(pb.nbCtrs()),
    time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT")),
    iter_limit_(Param::GetIntParam("LOCAL_SOLVER_ITER_LIMIT")),
    atol_(Param::GetDblParam("LOCAL_SOLVER_ATOL")),
    rtol_(Param::GetDblParam("LOCAL_SOLVER_RTOL")),
    alg_("DEFAULT")
{
    bool ismin = pb.getObjective().isMinimization();

    // objective function
    Term to = pb.getObjective().getTerm();

    // objective function to be minimized
    Term tomin = ismin ? to : -to;

    // scope of the objective function
    // to.makeScope(pb.getObjective().getTerm().scope());

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

    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction& obj)
    : n_(obj.nbVars()),m_(0), 
    time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT")),
    iter_limit_(Param::GetDblParam("LOCAL_SOLVER_ITER_LIMIT")),
    alg_("DEFAULT")
{
    obj_ = std::make_shared<RealFunction>(obj);
    ctrs_ = std::make_shared<RealFunctionVector>();
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction& obj, const RealFunctionVector& ctrs)
    : n_(obj.nbVars()),m_(ctrs.nbFuns()),
    time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT")),
    iter_limit_(Param::GetDblParam("LOCAL_SOLVER_ITER_LIMIT")),
    alg_("DEFAULT")
{
    obj_ = std::make_shared<RealFunction>(obj);
    ctrs_ = std::make_shared<RealFunctionVector>(ctrs);
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::~NLPModel()
{
}

double NLPModel::timeLimit() const
{
   return time_limit_;
}

void NLPModel::set_timeLimit(double val)
{
   time_limit_ = val;
}

size_t NLPModel::iterLimit() const
{
   return iter_limit_;
}

void NLPModel::set_iterLimit(size_t val)
{
   iter_limit_ = val;
}

size_t NLPModel::nbVars() const
{
    return n_;
}

size_t NLPModel::nbCtrs() const
{
    return m_;
}

std::shared_ptr<RealFunction> NLPModel::obj()
{
    return obj_;
}

std::shared_ptr<RealFunctionVector> NLPModel::ctrs()
{
    return ctrs_;
}

double NLPModel::bestVal() const
{
   return best_val_;
}

RealPoint NLPModel::bestPoint() const
{
   return *best_;
}

std::shared_ptr<RealPoint> NLPModel::bestPoint()
{
    return best_;
}

void NLPModel::set_bestPoint(std::shared_ptr<RealPoint> best)
{
    best_ = best;
}

OptimizationStatus NLPModel::status() const
{
   return status_;
}

std::string  NLPModel::algorithm() const
{
    return std::string(alg_);
}

void  NLPModel::set_algorithm(std::string alg_name)
{
    alg_ = alg_name;
}


} // namespace