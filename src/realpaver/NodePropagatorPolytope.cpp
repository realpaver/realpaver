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
 * @file   NodePropagatorPolytope.cpp
 * @brief  Node propagator based on the polytope hull contractor
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "AssertDebug.hpp"
#include "realpaver/NodePropagatorPolytope.hpp"

namespace realpaver {

NodePropagatorPolytope::NodePropagatorPolytope(ContractorFactory &facto)
    : NodePropagator()
{
   poly_ = facto.makePolytope();
}

Proof NodePropagatorPolytope::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   if (poly_ != nullptr)
      proof = poly_->contract(B, active);
   return proof;
}

} // namespace realpaver
