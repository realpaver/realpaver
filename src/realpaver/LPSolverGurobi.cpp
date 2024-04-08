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
 * @file   LPSolverGurobi.cpp
 * @brief  Wrapper class for the LP solver Gurobi.
 * @author Laurent Granvilliers
 * @date   2023-2-19
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/LPSolverGurobi.hpp"

namespace realpaver {

LPSolver::LPSolver() :
   env_(nullptr),
   simplex_(nullptr),
   vars_()
{
   env_ = new GRBEnv();
}

LPSolver::~LPSolver()
{
   if (simplex_ != nullptr) delete simplex_;
   if (env_ != nullptr) delete env_;
}

void LPSolver::makeVars()
{
   int n = getNbLinVars();

   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      char type = v.isContinuous() ? GRB_CONTINUOUS : GRB_INTEGER;

      GRBVar gv = simplex_->addVar(v.getLB(), v.getUB(), 0.0, type, v.getName());
      vars_.push_back(gv);
   }
}

GRBLinExpr LPSolver::makeGrbLinExpr(LinExpr e)
{
   GRBLinExpr ge;

   int n = e.getNbTerms();
   for (int i=0; i<n; ++i)
   {
      int j = e.getIndexVar(i);
      double coef = e.getCoef(i);
      GRBVar gv = vars_[j];
      ge += coef*gv;
   }

   return ge;
}

void LPSolver::makeCtrs()
{
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();

      GRBLinExpr ge = makeGrbLinExpr(e);

      if (c.isLessEqual())
         simplex_->addConstr(ge, GRB_LESS_EQUAL, c.getUB());

      else if (c.isGreaterEqual())
         simplex_->addConstr(ge, GRB_GREATER_EQUAL, c.getLB());

      else if (c.isEqual())
         simplex_->addConstr(ge, GRB_EQUAL, c.getLB());

      else
      {
         simplex_->addConstr(ge, GRB_LESS_EQUAL, c.getUB());
         simplex_->addConstr(ge, GRB_GREATER_EQUAL, c.getLB());
      }
   }
}

void LPSolver::makeObj()
{
   LinExpr e = getObjExpr();
   GRBLinExpr ge = makeGrbLinExpr(e);

   int sense = isMinimization() ? GRB_MINIMIZE : GRB_MAXIMIZE;
   simplex_->setObjective(ge, sense);
}

void LPSolver::makeGurobiSimplex()
{
   if (simplex_ != nullptr) delete simplex_;
   simplex_ = new GRBModel(env_);

   makeVars();
   makeCtrs();
   makeObj();
}

bool LPSolver::run()
{
   env_->set(GRB_DoubleParam_TimeLimit, getMaxSeconds());  
   env_->set(GRB_DoubleParam_IterationLimit, getMaxIter());  

   simplex_->optimize();

   bool optimal = simplex_->get(GRB_IntAttr_Status) == GRB_OPTIMAL;

   if (optimal)
   {
      setObjVal(simplex_->get(GRB_DoubleAttr_ObjVal));
    
      int n = getNbLinVars();
      for (int i=0; i<n; ++i)
      {
         double val = vars_[i].get(GRB_DoubleAttr_X);
         LinVar v = getLinVar(i);
         v.setObjVal(val);
      }
      
      setStatus(OptimizationStatus::Optimal);
   }
   else
   {
      setStatus(OptimizationStatus::Other);
  
      if (simplex_->get(GRB_IntAttr_Status) == GRB_INFEASIBLE)
         setStatus(OptimizationStatus::Infeasible);

      if (simplex_->get(GRB_IntAttr_Status) == GRB_TIME_LIMIT)
         setStatus(OptimizationStatus::StopOnTimeLimit);

      if (simplex_->get(GRB_IntAttr_Status) == GRB_ITERATION_LIMIT)
         setStatus(OptimizationStatus::StopOnIterLimit);
   }

   return optimal;
}

bool LPSolver::optimize()
{
   try
   {
      makeGurobiSimplex();
      return run();
   }
   catch (GRBException e)
   {
      LOG_INTER("Error in Gurobi" << e.getMessage());
      return false;
   }
}

bool LPSolver::reoptimize()
{
   try
   {
      makeObj();
      return run();
   }
   catch (GRBException e)
   {
      LOG_INTER("Error in Gurobi" << e.getMessage());
      return false;
   }
}   

} // namespace
