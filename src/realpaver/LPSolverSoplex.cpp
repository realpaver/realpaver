///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
    //  simplex_->addRowReal(soplex::LPRowReal(v.getLB(), dsrow, v.getUB()));
     simplex_->changeBoundsReal(j, v.getLB(), v.getUB());
     simplex_->changeRangeReal(j, v.getLB(), v.getUB());
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
      DSVectorReal dsrow(n);
      for (int j=0;j<e.getNbTerms();j++){
        dsrow.add(e.getIndexVar(j), e.getCoef(j));
      }
      simplex_->addRowReal(LPRowReal(c.getLB(), dsrow, c.getUB()));
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
   simplex_->setIntParam(SoPlex::SOLVEMODE, SoPlex::SOLVEMODE_REAL);
   simplex_->setBoolParam(SoPlex::ENSURERAY, true);
   simplex_->setRealParam(SoPlex::TIMELIMIT, getMaxSeconds());
   const int max_iter = getMaxIter();
   simplex_->setIntParam(SoPlex::ITERLIMIT,max_iter);
   simplex_->setIntParam(SoPlex::VERBOSITY, SoPlex::VERBOSITY_ERROR);
//    simplex_->setRealParam(SoPlex::FEASTOL, tolerance);
//    simplex_->setRealParam(SoPlex::OPTTOL, tolerance);

   simplex_->solve();
   simplex_->ignoreUnscaledViolations();

   soplex::SPxSolver::Status soplex_status = simplex_->status();
   
   if (soplex_status==soplex::SPxSolver::OPTIMAL)
   {
      setObjVal(simplex_->objValueReal());

      int n = getNbLinVars();
      DVectorReal solution(n);
      simplex_->getPrimalReal(solution);
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         v.setObjVal(solution[i]);
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
