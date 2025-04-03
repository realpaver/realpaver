/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   NLPModel.cpp
 * @brief  Interface for local optimization solvers
 * @author Raphaël Chenouard
 * @date   2024-4-11
 */

#include "realpaver/NLPModel.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ScopeBank.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

NLPModel::NLPModel(const Problem &pb)
    : obj_(nullptr)
    , ctrs_(nullptr)
    , scop_(pb.scope())
    , n_(pb.nbVars())
    , m_(pb.nbCtrs())
    , time_limit_(Params::GetDblParam("NLP_SOLVER_TIME_LIMIT"))
    , iter_limit_(Params::GetIntParam("NLP_SOLVER_ITER_LIMIT"))
    , tol_(Params::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Params::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL"))
    , alg_(Params::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
   bool ismin = pb.getObjective().isMinimization();

   // objective function
   Term to = pb.getObjective().getTerm();

   // objective function to be minimized
   Term tomin = ismin ? to : -to;
   std::shared_ptr<Dag> odag = std::make_shared<Dag>();
   odag->insert(tomin); // Add the objective expression to the objective dag
   obj_ = new RealFunction(odag, 0);

   // constraints
   std::shared_ptr<Dag> dag = std::make_shared<Dag>();
   for (size_t j = 0; j < m_; j++)
      dag->insert(pb.ctrAt(j));

   // TODO : GERER EXCEPTION si la contrainte n'est pas acceptée

   if (dag->nbFuns() > 0)
      ctrs_ = new RealFunctionVector(dag);

   best_ = nullptr;
   best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction &obj)
    : obj_(nullptr)
    , ctrs_(nullptr)
    , scop_(obj.scope())
    , n_(obj.nbVars())
    , m_(0)
    , time_limit_(Params::GetDblParam("NLP_SOLVER_TIME_LIMIT"))
    , iter_limit_(Params::GetIntParam("NLP_SOLVER_ITER_LIMIT"))
    , tol_(Params::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Params::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL"))
    , alg_(Params::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
   obj_ = new RealFunction(obj);
   best_ = nullptr;
   best_val_ = Interval::universe().right();
}

NLPModel::NLPModel(const RealFunction &obj, const RealFunctionVector &ctrs)
    : obj_(nullptr)
    , ctrs_(nullptr)
    , scop_()
    , n_(obj.nbVars())
    , m_(ctrs.nbFuns())
    , time_limit_(Params::GetDblParam("NLP_SOLVER_TIME_LIMIT"))
    , iter_limit_(Params::GetIntParam("NLP_SOLVER_ITER_LIMIT"))
    , tol_(Params::GetDblParam("NLP_SOLVER_OBJ_REL_TOL"),
           Params::GetDblParam("NLP_SOLVER_OBJ_ABS_TOL"))
    , alg_(Params::GetStrParam("NLP_SOLVER_ALGORITHM"))
{
   scop_.insert(obj.scope());
   scop_.insert(ctrs.scope());
   scop_ = ScopeBank::getInstance()->insertScope(scop_);
   obj_ = new RealFunction(obj);
   ctrs_ = new RealFunctionVector(ctrs);
   best_ = nullptr;
   best_val_ = Interval::universe().right();
}

NLPModel::~NLPModel()
{
   if (best_ != nullptr)
      delete best_;
   if (obj_ != nullptr)
      delete obj_;
   if (ctrs_ != nullptr)
      delete ctrs_;
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

RealFunction *NLPModel::obj() const
{
   return obj_;
}

RealFunctionVector *NLPModel::ctrs() const
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

void NLPModel::setBestPoint(const RealPoint &best)
{
   if (best_ != nullptr)
      delete best_;
   best_ = new RealPoint(best);
}

OptimizationStatus NLPModel::status() const
{
   return status_;
}

std::string NLPModel::algorithm() const
{
   return std::string(alg_);
}

void NLPModel::setAlgorithm(const std::string &name)
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
   return scop_;
}

} // namespace realpaver
