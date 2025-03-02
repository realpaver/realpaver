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
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/LPModel.hpp"
#include <ostream>
#include "realpaver/LPSolverHighs.hpp"

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
   for (int i=0; i<n; ++i)
   {
      const LinVar& v = getLinVar(i);
      simplex_->addVar(v.getLB(), v.getUB());
   }
}

void LPSolver::makeCtrs()
{
   // bound constraints (necessary for certification methods)
   int n = getNbLinVars();
   for (int i=0; i<n; ++i)
   {
      LinVar v = getLinVar(i);
      LinExpr e = {{1.0}, {v}};
      simplex_->addRow(v.getLB(), v.getUB(), e.getNbTerms(),
                       e.getIndexVars(), e.getCoefs());
   }

   // constraints
   int m = getNbLinCtrs();
   for (int i=0; i<m; ++i)
   {
      LinCtr c = getLinCtr(i);
      LinExpr e = c.getExpr();
      simplex_->addRow(c.getLB(), c.getUB(), e.getNbTerms(),
                       e.getIndexVars(), e.getCoefs());
   }
}

void LPSolver::makeCost()
{
   LinExpr obj = getCost();
   int n = obj.getNbTerms();
   for (int i=0; i<n; ++i){
      LinVar v = getLinVar(i);
      int j = v.getIndex();
      simplex_->changeColCost(j, obj.getCoef(i));
   }

   ObjSense sense = (getSense() == LPSense::Min) ? ObjSense::kMinimize :
                                                   ObjSense::kMaximize;
   simplex_->changeObjectiveSense(sense);
}

void LPSolver::makeSimplex()
{
   if (simplex_ != nullptr) delete simplex_;
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
   switch(status)
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

void LPSolver::setOptions()
{
   int maxsec = getMaxSeconds();
   int maxiter = getMaxIter();
   double tol = getFeasTol();
   HighsStatus hs, ok = HighsStatus::kOk;

   hs = simplex_->setOptionValue("time_limit", maxsec);
   ASSERT(hs==ok, "Bad option in Highs: ");

   hs = simplex_->setOptionValue("simplex_iteration_limit", maxiter);
   ASSERT(hs==ok, "Bad option in Highs: ");

   hs = simplex_->setOptionValue("log_to_console", false);
   ASSERT(hs==ok, "Bad option in Highs: ");

   hs = simplex_->setOptionValue("output_flag", false);
   ASSERT(hs==ok, "Bad option in Highs: ");

   hs = simplex_->setOptionValue("primal_feasibility_tolerance", tol);
   ASSERT(hs==ok, "Bad option in Highs: ");
}

LPStatus LPSolver::run()
{
   LOG_INTER("Runs LP Solver Highs on \n---\n" << (*this) << "---");

   simplex_->run();

   LPStatus status = toLPStatus();

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
   makeSimplex();
   simplex_->setOptionValue("presolve", "choose");
   return run();
}

LPStatus LPSolver::reoptimize()
{
   for (int i=0; i<getNbLinVars(); ++i)
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
   const HighsInfo& info = simplex_->getInfo();
   const bool has_primal = info.primal_solution_status;
   THROW_IF(!has_primal, "Primal solution not found (Highs)");

   const HighsSolution& sol = simplex_->getSolution();
   int m = getNbLinVars();
   RealVector primal(m);
   for (int i=0; i<m; ++i)
      primal[i] = sol.col_value[i];

   return primal;
}

RealVector LPSolver::dualSolution() const
{
   const HighsInfo& info = simplex_->getInfo();
   const bool has_dual = info.dual_solution_status;
   THROW_IF(!has_dual, "Dual solution not found (Highs)");

   const HighsSolution& sol = simplex_->getSolution();
   int p = getNbLinVars() + getNbLinCtrs();
   RealVector dual(p);

   for (int i=0; i<p; ++i)
      dual[i] = sol.row_dual[i];

   return dual;
}

bool LPSolver::infeasibleRay(RealVector& ray) const
{
   LPStatus status = toLPStatus();

   if ((status != LPStatus::Infeasible) &&
       (status != LPStatus::InfeasibleOrUnbounded)) return false;

   int p = getNbLinVars() + getNbLinCtrs();
   if (ray.size() != p) ray.resize(p);

   // Technique implemented in SCIP
   int dualdir = (getSense()==LPSense::Min) ? 1.0 : -1.0;
   for (int i=0; i<p; ++i)
   {
      if (simplex_->getLp().row_lower_[i]>0.0)
         ray[i] = dualdir;
      else if (simplex_->getLp().row_upper_[i]<0.0)
         ray[i] = -dualdir;
      else
         ray[i] = 0.0;
   }

   // gets the ray
   bool hasRay = false;
   HighsStatus hs = simplex_->getDualRay(hasRay, ray.data());
   return (hs==HighsStatus::kOk) && hasRay;
}

} // namespace
