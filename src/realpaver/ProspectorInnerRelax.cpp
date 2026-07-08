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
 * @file   Prospector.cpp
 * @brief  Prospector to find feasible points
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "Bitset.hpp"
#include "DagExtended.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/LPSolver.hpp"
#include "realpaver/LinearizerInner.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/ProspectorInnerRelax.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

ProspectorInnerRelax::ProspectorInnerRelax(const Variable &vobj, const Term &obj,
                                           std::unique_ptr<LinearizerInner> linearizer,
                                           std::unique_ptr<Prover> prover)
    : Prospector(std::move(prover))
    , vobj_(vobj)
    , obj_(obj)
{
   REALPAVER_ASSERT(linearizer != nullptr, "No linearizer is given as input");
   linearizer_ = std::move(linearizer);
}

bool ProspectorInnerRelax::exploreImpl(Node &node, RealPoint &pt)
{
   LPSolver lp_solver;
   linearizer_->makeLP(lp_solver, *node.box());
   LinVar lv = lp_solver.getLinVar(linearizer_->linVarIndex(vobj_));
   LinExpr e({1.0}, {lv});

   lp_solver.setCost(e);
   lp_solver.setSense(LPSense::Min);

   LPStatus success = lp_solver.optimize();
   if (success == LPStatus::Optimal)
   {
      RealVector sol = lp_solver.primalSolution();
      for (Variable v : node.scope())
         pt.set(v, sol.get(linearizer_->linVarIndex(v)));

      // Check if prover is useful
      Bitset active = node.active();
      IntervalBox bsol(pt);
      use_prover_ = false; // Useful only if non-relaxed equations
      for (size_t i = 0; i < linearizer_->edag()->size(); i++)
      {
         SharedDagFun fun = linearizer_->edag()->getActiveFun(i, bsol, active);
         if (fun == nullptr)
         {
            continue;
         }
         // Use only prover when equation are not relaxed
         if (fun->isAnEquation() && linearizer_->getObjCtrIndex() != i &&
             linearizer_->getRelaxTol() == 0)
         {
            use_prover_ = true;
            break;
         }
      }
   }
   return success == LPStatus::Optimal;
}

void ProspectorInnerRelax::setRelaxTol(double tol)
{
   linearizer_->setRelaxTol(tol);
}

} // namespace realpaver
