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

#include "realpaver/Logger.hpp"
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
   for(int i=0; i<n; ++i)
     simplex_->addColReal(soplex::LPColReal(0, dscol, soplex::infinity,
                                            -soplex::infinity));

   soplex::DSVectorReal dsrow(n);
   int nb_int = 0;
   std::vector<int> vint;
   for(int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      dsrow.add(j, 1);
      simplex_->addRowReal(soplex::LPRowReal(-soplex::infinity, dsrow,
                                             soplex::infinity));
      if (!v.isContinuous())
      {
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

   // constraints
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

   // bound constraints
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      soplex::DSVectorReal dsrow(n);
      for (int j=0;j<e.getNbTerms();j++)
        dsrow.add(e.getIndexVar(j), e.getCoef(j));

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
      dcost[j] = obj.getCoef(i);
   }
   simplex_->changeObjReal(dcost);

   if (isMinimization())
     simplex_->setIntParam(soplex::SoPlex::OBJSENSE,
                           soplex::SoPlex::OBJSENSE_MINIMIZE);
   else
     simplex_->setIntParam(soplex::SoPlex::OBJSENSE,
                           soplex::SoPlex::OBJSENSE_MAXIMIZE);
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
   simplex_->setIntParam(soplex::SoPlex::SOLVEMODE,
                         soplex::SoPlex::SOLVEMODE_REAL);

   simplex_->setBoolParam(soplex::SoPlex::ENSURERAY, true);

   simplex_->setRealParam(soplex::SoPlex::TIMELIMIT, getMaxSeconds());

   const int max_iter = getMaxIter();
   simplex_->setIntParam(soplex::SoPlex::ITERLIMIT, max_iter);

   simplex_->setIntParam(soplex::SoPlex::VERBOSITY,
                         soplex::SoPlex::VERBOSITY_ERROR);

   simplex_->solve();
   simplex_->ignoreUnscaledViolations();

   // assigns the status
   soplex::SPxSolver::Status status = simplex_->status();
   switch(status)
   {
      case soplex::SPxSolver::OPTIMAL:
         setStatus(LPStatus::Optimal); break;
      case soplex::SPxSolver::INFEASIBLE:
         setStatus(LPStatus::Infeasible); break;
      case soplex::SPxSolver::INForUNBD:
         setStatus(LPStatus::InfeasibleOrUnbounded); break;
      case soplex::SPxSolver::UNBOUNDED:
         setStatus(LPStatus::Unbounded); break;
      case soplex::SPxSolver::ABORT_ITER:
         setStatus(LPStatus::StopOnIterLimit); break;
      case soplex::SPxSolver::ABORT_TIME:
         setStatus(LPStatus::StopOnTimeLimit); break;
      default:
         setStatus(LPStatus::Other);
   }

   LOG_INTER("Status: " << getStatus());

   // assigns the solution
   if (status==soplex::SPxSolver::OPTIMAL)
   {
      // assigns the value of the optimum
      double objval = simplex_->objValueReal();
      setObjVal(objval);
      LOG_INTER("Optimum found: " << objval);

      int m = getNbLinCtrs();
      int n = getNbLinVars();

      soplex::DVectorReal solution(n);
      simplex_->getPrimal(solution);
      for (int i=0; i<n; ++i)
      {
         double val = solution[i];
         LinVar v = getLinVar(i);
         v.setObjVal(val);
         LOG_LOW(v.getName() << " = " << val);
      }

      // assigns the values of the dual variables (multipliers)
      soplex::DVectorReal dual(m+n);
      simplex_->getDual(dual);

      // first for the primal constraints
      for (int i=0; i<m; ++i)
      {
         LinCtr c = getLinCtr(i);
         double mult = dual[i];
         c.setMultiplier(mult);
         LOG_LOW("multiplier c" << i << " = " << mult);
      }

      // second for the primal bound constraints
      for (int i=0; i<n; ++i)
      {
         LinVar v = getLinVar(i);
         double mult = dual[m+i];
         v.setMultiplier(mult);
         LOG_LOW("multiplier " << v.getName() << " = " << mult);
      }

      return true;
   }
   else
   {
      return false;
   }
}

bool LPSolver::optimize()
{
   makeSoplexSimplex();
   return run();
}

} // namespace
