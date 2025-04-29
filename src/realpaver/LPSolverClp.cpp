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
 * @file   LPSolverClp.cpp
 * @brief  Wrapper class for the LP solver Clp.
 * @author Laurent Granvilliers, Raphaël Chenouard
 * @date   2024-4-11
 */

#include "realpaver/LPSolverClp.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/Logger.hpp"
#include <CoinFinite.hpp>

namespace realpaver {

LPSolver::LPSolver()
    : simplex_(nullptr)
{
}

LPSolver::~LPSolver()
{
   if (simplex_ != nullptr)
      delete simplex_;
}

void LPSolver::makeVars()
{
   int m = getNbLinVars();
   simplex_->resize(0, m);

   for (int i = 0; i < m; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      simplex_->setColumnLower(j, v.getLB());
      simplex_->setColumnUpper(j, v.getUB());
   }
}

void LPSolver::makeCtrs()
{
   // bound constraints (necessary for certification methods)
   int m = getNbLinVars();
   for (int i = 0; i < m; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      double lb = v.getLB();
      double ub = v.getUB();
      simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(), lb, ub);
   }

   // constraints
   int n = getNbLinCtrs();
   for (int i = 0; i < n; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      double lb = c.getLB();
      double ub = c.getUB();
      simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(), lb, ub);
   }
}

void LPSolver::makeCost()
{
   LinExpr obj = getCost();
   int n = obj.getNbTerms();

   for (int i = 0; i < n; ++i)
      simplex_->setObjectiveCoefficient(obj.getIndexVar(i), obj.getCoef(i));

   int sense = (getSense() == LPSense::Min) ? 1.0 : -1.0;
   simplex_->setOptimizationDirection(sense);
}

void LPSolver::makeSimplex()
{
   if (simplex_ != nullptr)
      delete simplex_;
   simplex_ = new ClpSimplex();

   // assigns the options
   setOptions();

   // creates the model
   makeVars();
   makeCtrs();
   makeCost();
}

void LPSolver::setOptions()
{
   int maxsec = getMaxSeconds();
   int maxiter = getMaxIter();
   double tol = getFeasTol();

   simplex_->setMaximumSeconds(maxsec);
   simplex_->setMaximumIterations(maxiter);
   simplex_->setLogLevel(0);
   simplex_->setPrimalTolerance(tol);
}

LPStatus LPSolver::run()
{
   LOG_INTER("Runs LP Solver Clp on \n---\n" << (*this) << "---");

   simplex_->initialSolve();

   LPStatus status = toLPStatus();

#if LOG_ON
   if (status == LPStatus::Optimal)
   {
      LOG_INTER("Status: " << status << std::endl
                           << "Cost:   " << costSolution() << std::endl
                           << "Primal: " << primalSolution() << std::endl
                           << "Dual:   " << dualSolution() << std::endl);
   }
   else
      LOG_INTER("Status: " << status << std::endl);
#endif

   return status;
}

LPStatus LPSolver::toLPStatus()
{
   if (simplex_->isProvenOptimal())
      return LPStatus::Optimal;

   else if (simplex_->isProvenPrimalInfeasible())
      return LPStatus::Infeasible;

   else if (simplex_->isProvenDualInfeasible())
      return LPStatus::InfeasibleOrUnbounded;

   else if (simplex_->isIterationLimitReached())
      return LPStatus::StopOnIterLimit;

   else if (simplex_->hitMaximumIterations())
      return LPStatus::StopOnTimeLimit;

   else
      return LPStatus::Other;
}

LPStatus LPSolver::optimize()
{
   makeSimplex();
   return run();
}

LPStatus LPSolver::reoptimize()
{
   for (int i = 0; i < getNbLinVars(); ++i)
      simplex_->setObjectiveCoefficient(i, 0.0);

   makeCost();
   return run();
}

double LPSolver::costSolution() const
{
   return simplex_->getObjValue();
}

RealVector LPSolver::primalSolution() const
{
   double *colPrimal = simplex_->primalColumnSolution();
   int m = getNbLinVars();
   RealVector primal(m);
   for (int i = 0; i < m; ++i)
      primal[i] = colPrimal[i];

   return primal;
}

RealVector LPSolver::dualSolution() const
{
   int p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);

   double *rowDual = simplex_->dualRowSolution();
   for (int i = 0; i < p; ++i)
      dual[i] = rowDual[i];

   return dual;
}

bool LPSolver::infeasibleRay(RealVector &ray) const
{
   if (!(simplex_->isProvenPrimalInfeasible() || simplex_->isProvenDualInfeasible()))
      return false;

   double *clpRay = simplex_->infeasibilityRay();
   if (clpRay == nullptr)
      return false;

   int p = getNbLinVars() + getNbLinCtrs();
   if (ray.size() != p)
      ray.resize(p);
   for (int i = 0; i < p; ++i)
      ray[i] = clpRay[i];
   delete[] clpRay;
   return true;
}

} // namespace realpaver
