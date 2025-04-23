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

#include "LPModel.hpp"
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
   size_t n = getNbLinVars();
   size_t nb_int = 0;
   std::vector<int> vint;
   soplex::DSVectorReal dscol(0);
   for (int i = 0; i < n; ++i)
   {
      simplex_->addColReal(
          soplex::LPColReal(0, dscol, soplex::infinity, -soplex::infinity));
      if (!getLinVar(i).isContinuous())
      {
         nb_int++;
         vint.push_back(getLinVar(i).getIndex());
      }
   }
   if (nb_int > 0)
      simplex_->setIntegralityInformation(nb_int, vint.data());
}

void LPSolver::makeCtrs()
{
   size_t n = getNbLinVars();
   soplex::DSVector row(n);
   for (size_t i = 0; i < n; i++)
   {
      const LinVar &v = getLinVar(i);
      row.add(v.getIndex(), 1);
      simplex_->addRowReal(soplex::LPRow(v.getLB(), row, v.getUB()));
      row.clear();
   }

   size_t m = getNbLinCtrs();
   for (size_t j = 0; j < m; j++)
   {
      soplex::DSVectorReal row(n);
      const LinCtr &c_j = getLinCtr(j);
      const LinExpr &l_e = c_j.getExpr();
      for (size_t k = 0; k < l_e.getNbTerms(); k++)
         row.add(l_e.getIndexVar(k), l_e.getCoef(k));
      simplex_->addRowReal(soplex::LPRowReal(c_j.getLB(), row, c_j.getUB()));
   }
}

void LPSolver::makeCost()
{
   if (getSense() == LPSense::Min)
   {
      simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MINIMIZE);
   }
   else
   {
      simplex_->setIntParam(soplex::SoPlex::OBJSENSE, soplex::SoPlex::OBJSENSE_MAXIMIZE);
   }

   soplex::DVectorReal obj(getNbLinVars());
   const LinExpr &l_e = getCost();
   for (size_t k = 0; k < l_e.getNbTerms(); k++)
   {
      obj[l_e.getIndexVar(k)] = l_e.getCoef(k);
   }
   simplex_->changeObjReal(obj);
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
   size_t maxsec = getMaxSeconds();
   size_t maxiter = getMaxIter();
   double tol = getFeasTol();

   simplex_->setIntParam(soplex::SoPlex::SOLVEMODE, soplex::SoPlex::SOLVEMODE_REAL);
   simplex_->setBoolParam(soplex::SoPlex::ENSURERAY, true);
   simplex_->setRealParam(soplex::SoPlex::TIMELIMIT, maxsec);
   simplex_->setIntParam(soplex::SoPlex::ITERLIMIT, maxiter);
   simplex_->setIntParam(soplex::SoPlex::VERBOSITY, soplex::SoPlex::VERBOSITY_ERROR);
   simplex_->setRealParam(soplex::SoPlex::FEASTOL, tol);
   simplex_->setRealParam(soplex::SoPlex::OPTTOL, tol);
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
   size_t n = getNbLinVars();
   RealVector primal(n);
   soplex::DVectorReal solution(n);
   simplex_->getPrimal(solution);
   for (int i = 0; i < n; ++i)
      primal[i] = solution[i];
   return primal;
}

RealVector LPSolver::dualSolution() const
{
   size_t p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);
   soplex::DVectorReal dual_sol(p);
   simplex_->getDual(dual_sol);
   for (size_t i = 0; i < p; i++)
      dual[i] = dual_sol[i];
   return dual;
}

bool LPSolver::infeasibleRay(RealVector &ray) const
{
   LPStatus status = toLPStatus();

   if ((status != LPStatus::Infeasible) && (status != LPStatus::InfeasibleOrUnbounded))
      return false;

   size_t p = getNbLinVars() + getNbLinCtrs();
   if (ray.size() != p)
      ray.resize(p);

   // gets the ray
   bool hasRay = simplex_->hasDualFarkas();
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
