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
 * @file   LPSolverSoplex.cpp
 * @brief  Wrapper class for the LP solver Soplex.
 * @author Raphaël Chenouard
 * @date   2024-4-11
*/

#include "realpaver/LPSolverSoplex.hpp"

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

   soplex::DSVectorReal dscol(0);
   for(int i = 0; i < n; ++i) {
     simplex_->addColReal(soplex::LPColReal(0, dscol, soplex::infinity, -soplex::infinity));
   }
   soplex::DSVectorReal dsrow(n);
   int nb_int = 0;
   std::vector<int> vint;
   for(int i = 0; i < n; ++i) {
     LinVar v = getLinVar(i);
     int j = v.getIndex();
     dsrow.add(j, 1);
     simplex_->addRowReal(soplex::LPRowReal(-soplex::infinity, dsrow, soplex::infinity));
   //   simplex_->changeBoundsReal(j, v.getLB(), v.getUB());
   //   simplex_->changeRangeReal(j, v.getLB(), v.getUB());
     if (!v.isContinuous()){
       nb_int++;
       vint.push_back(j);
     }
     dsrow.clear();
   }
   if (nb_int>0)
     simplex_->setIntegralityInformation(nb_int, vint.data());
}

void LPSolver::makeCtrs()
{
   int m = getNbLinCtrs();
   int n = getNbLinVars();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      soplex::DSVectorReal dsrow(n);
      for (int j=0;j<e.getNbTerms();j++){
        dsrow.add(e.getIndexVar(j), e.getCoef(j));
      }
      simplex_->addRowReal(soplex::LPRowReal(c.getLB(), dsrow, c.getUB()));
   }

   // RAPHAEL
   // bound constraints
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      soplex::DSVectorReal dsrow(n);
      for (int j=0;j<e.getNbTerms();j++){
        dsrow.add(e.getIndexVar(j), e.getCoef(j));
      }
      simplex_->addRowReal(soplex::LPRowReal(v.getLB(), dsrow, v.getUB()));
   }
}

void LPSolver::makeObj()
{
   LinExpr obj = getObjExpr();
   int n = obj.getNbTerms();

   soplex::DVectorReal dcost(n);
   for (int i=0; i<n; ++i){
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      //simplex_->changeObjReal(j,obj.getCoef(i));
      dcost[j] = obj.getCoef(i);
   }
   simplex_->changeObjReal(dcost);

   if (isMinimization())
     simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MINIMIZE);
   else
     simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MAXIMIZE);
}

void LPSolver::makeSoplexSimplex()
{
   if (simplex_ != nullptr) delete simplex_;
   simplex_ = new soplex::SoPlex();

   makeVars();
   makeCtrs();
   makeObj();
}

bool LPSolver::run()
{
   simplex_->setIntParam(soplex::SoPlex::SOLVEMODE, soplex::SoPlex::SOLVEMODE_REAL);
   simplex_->setBoolParam(soplex::SoPlex::ENSURERAY, true);
   simplex_->setRealParam(soplex::SoPlex::TIMELIMIT, getMaxSeconds());
   const int max_iter = getMaxIter();
   simplex_->setIntParam(soplex::SoPlex::ITERLIMIT,max_iter);
   simplex_->setIntParam(soplex::SoPlex::VERBOSITY, soplex::SoPlex::VERBOSITY_ERROR);
//    simplex_->setRealParam(SoPlex::FEASTOL, tolerance);
//    simplex_->setRealParam(SoPlex::OPTTOL, tolerance);

   simplex_->solve();
   simplex_->ignoreUnscaledViolations();

   soplex::SPxSolver::Status soplex_status = simplex_->status();
   
   if (soplex_status==soplex::SPxSolver::OPTIMAL)
   {
      setObjVal(simplex_->objValueReal());

      int m = getNbLinCtrs();
      int n = getNbLinVars();

      soplex::DVectorReal solution(n);
      simplex_->getPrimalReal(solution);
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setObjVal(solution[i]);
      }

      // RAPHAEL
      // assigns the values of the dual variables (multipliers)
      soplex::DVectorReal dual(m+n);
      simplex_->getDualReal(dual);
      // first for the primal constraints
      for (int i=0; i<m; ++i)
      {
         LinCtr c = getLinCtr(i);
         c.setMultiplier(dual[i]);
      }
      // second for the primal bound constraints
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setMultiplier(dual[m+i]);
      }

      setStatus(OptimizationStatus::Optimal);
      return true;
   }
   else
   {
      setStatus(OptimizationStatus::Other);

      if (soplex_status==soplex::SPxSolver::INFEASIBLE ||
            soplex_status==soplex::SPxSolver::UNBOUNDED)
         setStatus(OptimizationStatus::Infeasible);

      if (soplex_status==soplex::SPxSolver::ABORT_TIME)
         setStatus(OptimizationStatus::StopOnIterLimit);

      if (soplex_status==soplex::SPxSolver::ABORT_ITER)
         setStatus(OptimizationStatus::StopOnTimeLimit);

      return false;
   }
}

bool LPSolver::optimize()
{
   makeSoplexSimplex();
   return run();
}

bool LPSolver::reoptimize()
{
   makeObj();
   return run();
}

} // namespace
