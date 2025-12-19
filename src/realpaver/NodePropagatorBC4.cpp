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
 * @file   NodePropagatorBC4.hpp
 * @brief  Propagator for nodes based on BC4
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/NodePropagatorBC4.hpp"

namespace realpaver {

NodePropagatorBC4::NodePropagatorBC4(ContractorFactory &facto)
    : NodePropagator()
{
   bc4_ = facto.makeBC4();
}

Proof NodePropagatorBC4::contractImpl(IntervalBox &B, const Bitset &active)
{
   return bc4_->contract(B, active);
}

} // namespace realpaver
