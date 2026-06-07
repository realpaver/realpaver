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
 * @file   NodePropagatorNewton.cpp
 * @brief  Node propagator based on the interval Newton operator
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/NodePropagatorNewton.hpp"

namespace realpaver {

NodePropagatorNewton::NodePropagatorNewton(ContractorFactory &facto)
    : NodePropagator()
{
   nwt_ = facto.makeSquareNewton();
}

Proof NodePropagatorNewton::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   if (nwt_ != nullptr)
      proof = nwt_->contract(B, active);
   return proof;
}

} // namespace realpaver
