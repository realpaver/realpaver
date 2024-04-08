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
 * @date   2023-2-19
*/

#include "realpaver/LPSolverClp.hpp"

namespace realpaver {

LPSolver::LPSolver() :
   simplex_(nullptr)
{}

LPSolver::~LPSolver()
{
   if (simplex_!= nullptr) delete simplex_;
}

void LPSolver::makeVars()
{
   int n = getNbLinVars();
   simplex_->resize(0, n);

   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();

      // RAPHAEL
      //simplex_->setColumnLower(j, v.getLB());
      //simplex_->setColumnUpper(j, v.getUB());

      if (v.isContinuous())
         simplex_->setContinuous(j);
      else
         simplex_->setInteger(j);
   }
}

void LPSolver::makeCtrs()
{
   // linear constraints
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();

      simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(),
                       c.getLB(), c.getUB());
   }

   // RAPHAEL
   // bound constraints
   int n = getNbLinVars();
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(),
                       v.getLB(), v.getUB());
   }
}

void LPSolver::makeObj()
{
   LinExpr obj = getObjExpr();
   int n = obj.getNbTerms();

   for (int i=0; i<n; ++i)
      simplex_->setObjectiveCoefficient(obj.getIndexVar(i), obj.getCoef(i));

   int sense = isMinimization() ? 1.0: -1.0;
   simplex_->setOptimizationDirection(sense);
}

void LPSolver::makeClpSimplex()
{
   if (simplex_ != nullptr) delete simplex_;
   simplex_ = new ClpSimplex();

   makeVars();
   makeCtrs();
   makeObj();
}

bool LPSolver::run()
{
   simplex_->setMaximumSeconds(getMaxSeconds());
   simplex_->setMaximumIterations(getMaxIter());
   simplex_->setLogLevel(0);

   simplex_->initialSolve();

   if (simplex_->isProvenOptimal())
   {
      int n = getNbLinVars();
      int m = getNbLinCtrs();

      // assigns the values of the primal variables
      double* colPrimal = simplex_->primalColumnSolution();
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setObjVal(colPrimal[i]);
      }

      // assigns the optimum
      setObjVal(simplex_->getObjValue());

      // RAPHAEL
      // assigns the values of the dual variables (multipliers)
      double* rowDual = simplex_->dualRowSolution();
      // first for the primal constraints
      for (int i=0; i<m; ++i)
      {
         LinCtr c = getLinCtr(i);
         c.setMultiplier(rowDual[i]);
      }
      // second for the primal bound constraints
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setMultiplier(rowDual[m+i]);
      }
   
      // assigns the status
      setStatus(OptimizationStatus::Optimal);
      return true;
   }
   else
   {
      setStatus(OptimizationStatus::Other);

      if (simplex_->isProvenPrimalInfeasible() ||
            simplex_->isProvenDualInfeasible())
         setStatus(OptimizationStatus::Infeasible);

      if (simplex_->isIterationLimitReached())
         setStatus(OptimizationStatus::StopOnIterLimit);

      if (simplex_->hitMaximumIterations())
         setStatus(OptimizationStatus::StopOnTimeLimit);

      return false;
   }
}

bool LPSolver::optimize()
{
   makeClpSimplex();
   return run();
}

bool LPSolver::reoptimize()
{
   int n = getNbLinVars();

   for (int i=0; i<n; ++i)
      simplex_->setObjectiveCoefficient(i, 0.0);

   makeObj();
   return run();
}

} // namespace
