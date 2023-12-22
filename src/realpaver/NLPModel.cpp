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
    : obj_(nullptr),
      ctrs_(nullptr),
      scope_(pb.scope()),
      n_(pb.nbVars()),
      m_(pb.nbCtrs()),
      time_limit_(Param::GetDblParam("NLP_SOLVER_TIME_LIMIT")),
      iter_limit_(Param::GetIntParam("NLP_SOLVER_ITER_LIMIT")),
      tol_(Param::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Param::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL")),
      alg_(Param::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
    bool ismin = pb.getObjective().isMinimization();

    // objective function
    Term to = pb.getObjective().getTerm();

    // objective function to be minimized
    Term tomin = ismin ? to : -to;
    std::shared_ptr<Dag> odag = std::make_shared<Dag>();
    odag->insert(tomin);   // Add the objective expression to the objective dag
    obj_ = new RealFunction(odag, 0);

    // constraints
    std::shared_ptr<Dag> dag =  std::make_shared<Dag>();
    for (size_t j=0; j<m_; j++)
        dag->insert(pb.ctrAt(j));

    // TODO : GERER EXCEPTION si la contrainte n'est pas acceptée

    if (dag->nbFuns()>0)
        ctrs_ = new RealFunctionVector(dag);

    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction& obj)
    : obj_(nullptr),
      ctrs_(nullptr),
      scope_(),
      n_(obj.nbVars()),
      m_(0), 
      time_limit_(Param::GetDblParam("NLP_SOLVER_TIME_LIMIT")),
      iter_limit_(Param::GetIntParam("NLP_SOLVER_ITER_LIMIT")),
      tol_(Param::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Param::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL")),
      alg_(Param::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
    scope_.insert(obj.scope());
    obj_ = new RealFunction(obj);
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction& obj, const RealFunctionVector& ctrs)
    : obj_(nullptr),
      ctrs_(nullptr),
      scope_(),
      n_(obj.nbVars()),
      m_(ctrs.nbFuns()),
      time_limit_(Param::GetDblParam("NLP_SOLVER_TIME_LIMIT")),
      iter_limit_(Param::GetIntParam("NLP_SOLVER_ITER_LIMIT")),
      tol_(Param::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Param::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL")),
      alg_(Param::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
    scope_.insert(obj.scope());
    scope_.insert(ctrs.scope());
    obj_ = new RealFunction(obj);
    ctrs_ = new RealFunctionVector(ctrs);
    best_ = nullptr;
    best_val_ = Interval::universe().right();
}

NLPModel::~NLPModel()
{
   if (best_ != nullptr) delete best_;
   if (obj_ != nullptr) delete obj_;
   if (ctrs_ != nullptr) delete ctrs_;
}

double NLPModel::timeLimit() const
{
   return time_limit_;
}

void NLPModel::setTimeLimit(double val)
{
   time_limit_ = val;
}

size_t NLPModel::iterLimit() const
{
   return iter_limit_;
}

void NLPModel::setIterLimit(size_t val)
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

RealFunction* NLPModel::obj() const
{
    return obj_;
}

RealFunctionVector* NLPModel::ctrs() const
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

void NLPModel::setBestPoint(const RealPoint& best)
{
    if (best_ != nullptr) delete best_;
    best_ = new RealPoint(best);
}

OptimizationStatus NLPModel::status() const
{
   return status_;
}

std::string  NLPModel::algorithm() const
{
    return std::string(alg_);
}

void  NLPModel::setAlgorithm(const std::string& name)
{
   alg_ = name;
}

std::string NLPModel::getAlgorithm() const
{
   return alg_;
}

Tolerance NLPModel::tol() const
{
   return tol_;
}

void NLPModel::setTol(Tolerance tol)
{
   tol_ = tol;
}

Scope NLPModel::scope() const
{
   return scope_;
}

} // namespace
