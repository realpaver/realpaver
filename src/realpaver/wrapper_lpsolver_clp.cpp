///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/wrapper_lpsolver_clp.hpp"

namespace realpaver {

LPSolver::LPSolver() :
   simplex_(nullptr)
{
   simplex_ = new ClpSimplex();
}

LPSolver::~LPSolver()
{
   if (simplex_!= nullptr)
   delete simplex_;
}

void LPSolver::makeVars()
{
   int n = getNbLinVars();
   simplex_->resize(0, n);

   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      simplex_->setColumnLower(j, v.getLB());
      simplex_->setColumnUpper(j, v.getUB());

      if (v.isContinuous())
         simplex_->setContinuous(j);
      else
         simplex_->setInteger(j);
   }
}

void LPSolver::makeCtrs()
{
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();

      simplex_->addRow(e.getNbTerms(), e.getIndexVars(), e.getCoefs(),
                       c.getLB(), c.getUB());
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
   simplex_->setMaximumSeconds(getMaxSeconds());
   simplex_->setMaximumIterations(getMaxIterations());
  
   makeVars();
   makeCtrs();
   makeObj();
}

bool LPSolver::optimize()
{
   makeClpSimplex();

   simplex_->initialSolve();
   if (simplex_->isProvenOptimal())
   {
      setObjVal(simplex_->getObjValue());

      int n = getNbLinVars();
      double* sol = simplex_->primalColumnSolution();

      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setObjVal(sol[i]);
      }

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

} // namespace
