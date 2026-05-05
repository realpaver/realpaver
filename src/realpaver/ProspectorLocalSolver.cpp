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

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/ProspectorLocalSolver.hpp"
#include <memory>

namespace realpaver {

ProspectorLocalSolver::ProspectorLocalSolver(std::unique_ptr<BoxPicker> picker,
                                             std::unique_ptr<ProverOptim> prover,
                                             std::unique_ptr<NLPModel> localSolver)
    : Prospector(std::move(prover))
{
   REALPAVER_ASSERT(picker != nullptr, "No picker is given as input");
   picker_ = std::move(picker);
   localSolver_ = std::move(localSolver);
}

bool ProspectorLocalSolver::exploreImpl(Node &node, RealPoint &pt)
{
   picker_->apply(*node.box(), pt);
   IntervalBox B(*node.box());

   localSolver_->minimize(B, pt, node.active());
   if (localSolver_->status() != OptimizationStatus::Optimal)
      return false;

   pt = localSolver_->bestPoint();
   if (!node.box()->contains(pt))
      return false;

   return true;
}

} // namespace realpaver
