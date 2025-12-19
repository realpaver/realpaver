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
 * @file   NodePropagatorHC4.cpp
 * @brief  Propagator for nodes based on HC4
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/NodePropagatorHC4.hpp"

namespace realpaver {

NodePropagatorHC4::NodePropagatorHC4(ContractorFactory &facto)
    : NodePropagator()
{
   hc4_ = facto.makeHC4();
}

Proof NodePropagatorHC4::contractImpl(IntervalBox &B, const Bitset &active)
{
   return hc4_->contract(B, active);
}

} // namespace realpaver
