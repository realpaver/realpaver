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
#include "realpaver/ProspectorSimple.hpp"

namespace realpaver {

ProspectorSimple::ProspectorSimple(std::unique_ptr<DomainBoxPicker> picker,
                                   std::unique_ptr<ProverOptim> prover)
    : Prospector(std::move(prover))
{
   REALPAVER_ASSERT(picker != nullptr, "No picker is given as input");
   picker_ = std::move(picker);
}

bool ProspectorSimple::exploreImpl(Node &node, RealPoint &pt)
{
   picker_->apply(*node.box(), pt);
   return true;
}

} // namespace realpaver
