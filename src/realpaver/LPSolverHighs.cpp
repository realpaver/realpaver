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
 * @file   LPSolverHighs.cpp
 * @brief  Wrapper class for the LP solver Highs.
 * @author Raphaël Chenouard
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/LPSolverHighs.hpp"
#include "realpaver/Logger.hpp"
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
   for (size_t i = 0; i < n; ++i)
   {
      const LinVar &v = getLinVar(i);
      simplex_->addVar(v.getLB(), v.getUB());
   }
}

void LPSolver::makeCtrs()
{
   // bound constraints (necessary for certification methods)
   size_t n = getNbLinVars();
   for (size_t i = 0; i < n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      simplex_->addRow(v.getLB(), v.getUB(), e.getNbTerms(), e.getIndexVars(),
                       e.getCoefs());
   }

   // constraints
   size_t m = getNbLinCtrs();
   for (size_t i = 0; i < m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      simplex_->addRow(c.getLB(), c.getUB(), e.getNbTerms(), e.getIndexVars(),
                       e.getCoefs());
   }
}

void LPSolver::makeCost()
{
   LinExpr obj = getCost();
   size_t n = obj.getNbTerms();
   for (size_t i = 0; i < n; ++i)
   {
      simplex_->changeColCost(obj.getIndexVar(i), obj.getCoef(i));
   }

   ObjSense sense =
       (getSense() == LPSense::Min) ? ObjSense::kMinimize : ObjSense::kMaximize;
   simplex_->changeObjectiveSense(sense);
}

void LPSolver::makeSimplex()
{
   if (simplex_ != nullptr)
      delete simplex_;
   simplex_ = new Highs();

   // assigns the options
   setOptions();

   // creates the model
   makeVars();
   makeCtrs();
   makeCost();
}

LPStatus LPSolver::toLPStatus() const
{
   HighsModelStatus status = simplex_->getModelStatus();
   switch (status)
   {
   case HighsModelStatus::kOptimal:
      return LPStatus::Optimal;

   case HighsModelStatus::kInfeasible:
      return LPStatus::Infeasible;

   case HighsModelStatus::kUnboundedOrInfeasible:
      return LPStatus::InfeasibleOrUnbounded;

   case HighsModelStatus::kUnbounded:
      return LPStatus::Unbounded;

   case HighsModelStatus::kIterationLimit:
      return LPStatus::StopOnIterLimit;

   case HighsModelStatus::kTimeLimit:
      return LPStatus::StopOnTimeLimit;

   default:
      return LPStatus::Other;
   }
}

void REALPAVER_ASSERT_OPTION(HighsStatus hs, const std::string &option)
{
   REALPAVER_ASSERT(hs == HighsStatus::kOk, "Bad option in Highs: " + option);
}

void LPSolver::setOptions()
{
   double maxsec = getMaxSeconds();
   int maxiter = (int)getMaxIter();
   double tol = getFeasTol();

   std::string option = "time_limit";
   REALPAVER_ASSERT_OPTION(simplex_->setOptionValue(option, maxsec), option);

   option = "simplex_iteration_limit";
   REALPAVER_ASSERT_OPTION(simplex_->setOptionValue(option, maxiter), option);

   option = "log_to_console";
   REALPAVER_ASSERT_OPTION(simplex_->setOptionValue(option, false), option);

   option = "output_flag";
   REALPAVER_ASSERT_OPTION(simplex_->setOptionValue(option, false), option);

   option = "primal_feasibility_tolerance";
   REALPAVER_ASSERT_OPTION(simplex_->setOptionValue(option, tol), option);
}

LPStatus LPSolver::run()
{
   LOG_INTER("Runs LP Solver Highs on \n---\n" << (*this) << "---");

   simplex_->run();

   LPStatus status = toLPStatus();

#if REALPAVER_LOG_ON
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
   makeSimplex();
   simplex_->setOptionValue("presolve", "choose");
   return run();
}

LPStatus LPSolver::reoptimize()
{
   for (size_t i = 0; i < getNbLinVars(); ++i)
      simplex_->changeColCost(i, 0.0);

   makeCost();
   simplex_->setOptionValue("presolve", "off");
   return run();
}

double LPSolver::costSolution() const
{
   return simplex_->getInfo().objective_function_value;
}

RealVector LPSolver::primalSolution() const
{
   const HighsInfo &info = simplex_->getInfo();
   const bool has_primal = info.primal_solution_status;
   THROW_IF(!has_primal, "Primal solution not found (Highs)");

   const HighsSolution &sol = simplex_->getSolution();
   size_t m = getNbLinVars();
   RealVector primal(m);
   for (size_t i = 0; i < m; ++i)
      primal[i] = sol.col_value[i];

   return primal;
}

RealVector LPSolver::dualSolution() const
{
   const HighsInfo &info = simplex_->getInfo();
   const bool has_dual = info.dual_solution_status;
   THROW_IF(!has_dual, "Dual solution not found (Highs)");

   const HighsSolution &sol = simplex_->getSolution();
   size_t p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);

   for (size_t i = 0; i < p; ++i)
      dual[i] = sol.row_dual[i];

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

   // Technique implemented in SCIP
   int dualdir = (getSense() == LPSense::Min) ? 1.0 : -1.0;
   for (size_t i = 0; i < p; ++i)
   {
      if (simplex_->getLp().row_lower_[i] > 0.0)
         ray[i] = dualdir;
      else if (simplex_->getLp().row_upper_[i] < 0.0)
         ray[i] = -dualdir;
      else
         ray[i] = 0.0;
   }

   // gets the ray
   bool hasRay = false;
   HighsStatus hs = simplex_->getDualRay(hasRay, ray.data());
   return (hs == HighsStatus::kOk) && hasRay;
}

} // namespace realpaver
