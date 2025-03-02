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
 * @date   2024-4-11
*/

#include "gurobi_c++.h"
#include "gurobi_c.h"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/LPSolverGurobi.hpp"

namespace realpaver {

LPSolver::LPSolver() :
   env_(nullptr),
   simplex_(nullptr),
   gvars_(),
   gctrs_()
{}

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
      double objCoef = 0.0;
      GRBVar gv = simplex_->addVar(v.getLB(), v.getUB(), objCoef,
                                   type, v.getName());
      gvars_.push_back(gv);
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
      GRBVar gv = gvars_[j];
      ge += coef*gv;
   }

   return ge;
}

void LPSolver::makeCtrs()
{
   GRBConstr cgur;

   // bound constraints (necessary for certification methods)
   int n = getNbLinVars();
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      GRBLinExpr ge = makeGrbLinExpr(e);
      cgur = simplex_->addRange(ge, v.getLB(), v.getUB());
      gctrs_.push_back(cgur);
   }

   // primal constraints
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      GRBLinExpr ge = makeGrbLinExpr(e);
      if (c.isLessEqual())
         cgur = simplex_->addConstr(ge, GRB_LESS_EQUAL, c.getUB());

      else if (c.isGreaterEqual())
         cgur = simplex_->addConstr(ge, GRB_GREATER_EQUAL, c.getLB());

      else if (c.isEqual())
         cgur = simplex_->addConstr(ge, GRB_EQUAL, c.getLB());

      else
         cgur = simplex_->addRange(ge, c.getLB(), c.getUB());

      gctrs_.push_back(cgur);
   }
}

void LPSolver::makeCost()
{
   LinExpr e = getCost();
   GRBLinExpr ge = makeGrbLinExpr(e);

   int sense = (getSense() == LPSense::Min) ? GRB_MINIMIZE : GRB_MAXIMIZE;
   simplex_->setObjective(ge, sense);
}

void LPSolver::makeSimplex()
{
   //  creates an environment
   createEnv();

   // creates the simplex with this environment
   if (simplex_ != nullptr) delete simplex_;
   simplex_ = new GRBModel(env_);

   // makes the model
   makeVars();
   makeCtrs();
   makeCost();
}

LPStatus LPSolver::toLPStatus()
{
   int status = simplex_->get(GRB_IntAttr_Status);
   switch(status)
   {
      case GRB_OPTIMAL:         return LPStatus::Optimal;
      case GRB_INFEASIBLE:      return LPStatus::Infeasible;
      case GRB_INF_OR_UNBD:     return LPStatus::InfeasibleOrUnbounded;
      case GRB_UNBOUNDED:       return LPStatus::Unbounded;
      case GRB_ITERATION_LIMIT: return LPStatus::StopOnIterLimit;
      case GRB_TIME_LIMIT:      return LPStatus::StopOnTimeLimit;
   }
   return LPStatus::Other;
}

void LPSolver::createEnv()
{
   if (env_ != nullptr) delete env_;
   env_ = new GRBEnv(true);

   int maxsec = getMaxSeconds();
   int maxiter = getMaxIter();
   double tol = getFeasTol();

   // limits
   env_->set(GRB_DoubleParam_TimeLimit, maxsec);
   env_->set(GRB_DoubleParam_IterationLimit, maxiter);

   // tolerances
   env_->set(GRB_DoubleParam_FeasibilityTol, tol);
   env_->set(GRB_DoubleParam_OptimalityTol, tol);

   // calculates additional infos for infeasible (or unbounded)
   // problems, necessary to obtain an infeasible ray
   env_->set(GRB_IntParam_InfUnbdInfo, 1);

   // no output
   env_->set(GRB_IntParam_OutputFlag, 0);

   env_->start();
}

LPStatus LPSolver::run()
{
   LOG_INTER("Runs LP Solver Gurobi on \n---\n" << (*this) << "---");

   simplex_->optimize();

   LPStatus status = toLPStatus();

   if (status==LPStatus::InfeasibleOrUnbounded)
   {
      // decide between Infeasible or Unbounded
      simplex_->set(GRB_IntParam_DualReductions, 0);
      simplex_->reset();
      simplex_->optimize();
      status = toLPStatus();
   }

#if LOG_ON
   if (status==LPStatus::Optimal)
   {
      LOG_INTER("Status: " << status << std::endl <<
                "Cost:   " << costSolution() << std::endl <<
                "Primal: " << primalSolution() << std::endl <<
                "Dual:   " << dualSolution() << std::endl);
   }
   else
      LOG_INTER("Status: " << status << std::endl);
#endif

   return status;
}

LPStatus LPSolver::optimize()
{
   try
   {
      makeSimplex();
      return run();
   }
   catch (GRBException e)
   {
      LOG_INTER("Error in Gurobi: " << e.getMessage());
      return LPStatus::Other;
   }
}

LPStatus LPSolver::reoptimize()
{
   try
   {
      makeCost();
      return run();
   }
   catch (GRBException e)
   {
      LOG_INTER("Error in Gurobi: " << e.getMessage());
      return LPStatus::Other;
   }
}

double LPSolver::costSolution() const
{
   ASSERT(simplex_->get(GRB_IntAttr_Status) == GRB_OPTIMAL,
          "Bad access to a solution in Gurobi");

   return simplex_->get(GRB_DoubleAttr_ObjVal);
}

RealVector LPSolver::primalSolution() const
{
   ASSERT(simplex_->get(GRB_IntAttr_Status) == GRB_OPTIMAL,
          "Bad access to a solution in Gurobi");

   int m = getNbLinVars();
   RealVector primal(m);
   for (int i=0; i<m; ++i)
      primal[i] = gvars_[i].get(GRB_DoubleAttr_X);

   return primal;
}

RealVector LPSolver::dualSolution() const
{
   ASSERT(simplex_->get(GRB_IntAttr_Status) == GRB_OPTIMAL,
          "Bad access to a solution in Gurobi");

   int p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);

   // the first m values are the reduced costs of the variables
   // and the next n values are the shadow prices of the constraints
   for (int i=0; i<p; ++i)
      dual[i] = gctrs_[i].get(GRB_DoubleAttr_Pi);

   return dual;
}

bool LPSolver::infeasibleRay(RealVector& ray) const
{
   if (simplex_->get(GRB_IntAttr_Status) != GRB_INFEASIBLE)
      return false;

   int p = getNbLinVars() + getNbLinCtrs();
   if (ray.size() != p) ray.resize(p);

   for (int i=0; i<p; ++i)
      ray[i] = gctrs_[i].get(GRB_DoubleAttr_FarkasDual);

   return true;
}

} // namespace
