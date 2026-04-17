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
 * @file   NodePropagatorACID.cpp
 * @brief  Node propagator based on ACID
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodePropagatorACID.hpp"

namespace realpaver {

NodePropagatorACID::NodePropagatorACID(ContractorFactory &facto)
    : NodePropagator()
{
   hc4_ = facto.makeHC4();
   acid_ = facto.makeACID();
}

Proof NodePropagatorACID::contractImpl(IntervalBox &B, const Bitset &active)
{
   // applies HC4
   Proof proof = hc4_->contract(B, active);
   if (proof == Proof::Empty)
      return proof;

   // applies ACID
   if (acid_ != nullptr)
      proof = acid_->contract(B, active);

   return proof;
}

} // namespace realpaver
