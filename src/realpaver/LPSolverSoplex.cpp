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

#include "realpaver/AssertDebug.hpp"
#include "realpaver/LPSolverSoplex.hpp"
#include "realpaver/Logger.hpp"
#include <cstddef>
#include <ostream>

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
   int n = getNbLinVars();

   soplex::DSVector dscol(0);
   for (int i = 0; i < n; ++i)
   {
      simplex_->addColReal(soplex::LPCol(0, dscol, soplex::infinity, -soplex::infinity));
   }
   // soplex::DSVectorReal dsrow(n);
   int nb_int = 0;
   std::vector<int> vint;
   for (int i = 0; i < n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      // dsrow.add(j, 1);
      // simplex_->addRowReal(soplex::LPRowReal(v.getLB(), dsrow, v.getUB()));
      //    simplex_->addRowReal(soplex::LPRowReal(-soplex::infinity, dsrow,
      //    soplex::infinity));
      //    //   simplex_->changeBoundsReal(j, v.getLB(), v.getUB());
      //    //   simplex_->changeRangeReal(j, v.getLB(), v.getUB());
      if (!v.isContinuous())
      {
         nb_int++;
         vint.push_back(j);
      }
      // dsrow.clear();
   }
   if (nb_int > 0)
      simplex_->setIntegralityInformation(nb_int, vint.data());
}

void LPSolver::makeCtrs()
{ // bound constraints
   int n = getNbLinVars();
   for (int i = 0; i < n; i++)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      soplex::DSVector row_v(n);
      for (int j = 0; j < e.getNbTerms(); j++)
      {
         row_v.add(e.getIndexVar(j), e.getCoef(j));
      }
      simplex_->addRowReal(soplex::LPRow(v.getLB(), row_v, v.getUB()));
   }
   int m = getNbLinCtrs();
   for (int i = 0; i < m; i++)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      soplex::DSVector row_c(n);
      for (int j = 0; j < e.getNbTerms(); j++)
      {
         row_c.add(e.getIndexVar(j), e.getCoef(j));
      }
      simplex_->addRowReal(soplex::LPRow(c.getLB(), row_c, c.getUB()));
   }
}

void LPSolver::makeCost()
{
   LinExpr obj = getCost();
   int n = obj.getNbTerms();

   soplex::DVector cost(getNbLinVars());
   // for (int i = 0; i < getNbLinVars(); i++)
   //    cost[i] = 0;
   for (int i = 0; i < n; ++i)
   {
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      // simplex_->changeObjReal(j, obj.getCoef(i));
      cost[j] = obj.getCoef(i);
   }
   simplex_->changeObjReal(cost);

   if (getSense() == LPSense::Min)
   {
      simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MINIMIZE);
   }
   else
   {
      simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MAXIMIZE);
   }
}

void LPSolver::makeSoplexSimplex()
{
   if (simplex_ != nullptr)
      delete simplex_;
   simplex_ = new soplex::SoPlex();

   // assigns the options
   setOptions();

   makeVars();
   makeCtrs();
   makeCost();
}

LPStatus LPSolver::toLPStatus() const
{
   soplex::SPxSolver::Status status = simplex_->status();
   switch (status)
   {
   case soplex::SPxSolver::OPTIMAL:
      return LPStatus::Optimal;

   case soplex::SPxSolver::INFEASIBLE:
      return LPStatus::Infeasible;

   case soplex::SPxSolver::INForUNBD:
      return LPStatus::InfeasibleOrUnbounded;

   case soplex::SPxSolver::UNBOUNDED:
      return LPStatus::Unbounded;

   case soplex::SPxSolver::ABORT_ITER:
      return LPStatus::StopOnIterLimit;

   case soplex::SPxSolver::ABORT_TIME:
      return LPStatus::StopOnTimeLimit;

   default:
      return LPStatus::Other;
   }
}

void LPSolver::setOptions()
{
   int maxsec = getMaxSeconds();
   int maxiter = getMaxIter();
   double tol = getFeasTol();

   simplex_->setIntParam(soplex::SoPlex::SOLVEMODE, soplex::SoPlex::SOLVEMODE_REAL);
   simplex_->setBoolParam(soplex::SoPlex::ENSURERAY, true);
   simplex_->setRealParam(soplex::SoPlex::TIMELIMIT, maxsec);
   simplex_->setIntParam(soplex::SoPlex::ITERLIMIT, maxiter);
   simplex_->setIntParam(soplex::SoPlex::VERBOSITY, soplex::SoPlex::VERBOSITY_ERROR);
   simplex_->setRealParam(soplex::SoPlex::FEASTOL, tol);
   // simplex_->setRealParam(soplex::SoPlex::OPTTOL, tol);
}

LPStatus LPSolver::run()
{
   LOG_INTER("Runs LP Solver SoPlex on \n---\n" << (*this) << "---");

   simplex_->optimize();
   simplex_->ignoreUnscaledViolations();

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

LPStatus LPSolver::optimize()
{
   makeSoplexSimplex();
   return run();
}

LPStatus LPSolver::reoptimize()
{
   makeCost();
   return run();
}

double LPSolver::costSolution() const
{
   return simplex_->objValueReal();
}

RealVector LPSolver::primalSolution() const
{
   int n = getNbLinVars();
   RealVector primal(n);
   soplex::DVectorReal solution(n);
   simplex_->getPrimal(solution);
   for (int i = 0; i < n; ++i)
      primal[i] = solution[i];
   return primal;
}

RealVector LPSolver::dualSolution() const
{
   // int m = getNbLinCtrs();
   // int n = getNbLinVars();
   int p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);
   soplex::DVectorReal dual_sol(p);
   simplex_->getDual(dual_sol);
   for (size_t i = 0; i < p; i++)
      dual[i] = dual_sol[i];
   // first for the primal constraints
   // for (int i = 0; i < m; i++)
   // {
   //    LinCtr c = getLinCtr(i);
   //    dual[i] = dual_sol[i];
   // }
   // // second for the primal bound constraints
   // for (int i = 0; i < n; i++)
   // {
   //    dual[m + i] = dual_sol[m + i];
   // }
   return dual;
}

bool LPSolver::infeasibleRay(RealVector &ray) const
{
   LPStatus status = toLPStatus();

   if ((status != LPStatus::Infeasible) && (status != LPStatus::InfeasibleOrUnbounded))
      return false;

   int p = getNbLinVars() + getNbLinCtrs();
   if (ray.size() != p)
      ray.resize(p);

   // gets the ray
   bool hasRay = simplex_->hasDualFarkas();
   // soplex::DVectorReal dual_farkas(p);
   if (hasRay)
   {
      soplex::DVector dual_farkas(p);
      hasRay = simplex_->getDualFarkas(dual_farkas);
      if (hasRay)
         for (size_t i = 0; i < p; i++)
            ray[i] = dual_farkas[i];
   }

   if (!hasRay)
   {
      // Technique implemented in SCIP
      int dualdir = (getSense() == LPSense::Min) ? 1.0 : -1.0;

      soplex::DVector lhs(p), rhs(p);
      simplex_->getLhsReal(lhs);
      simplex_->getRhsReal(rhs);
      for (int i = 0; i < getNbLinCtrs(); ++i)
      {
         if (lhs[i] > 0.0)
            ray[i] = dualdir;
         else if (rhs[i] < 0.0)
            ray[i] = -dualdir;
         else
            ray[i] = 0.0;
      }
   }
   return hasRay;
}

} // namespace realpaver
