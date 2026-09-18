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
 * @file   NodePropagatorAffine.cpp
 * @brief  Node propagator based on affine contractors
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/NodePropagatorAffine.hpp"

namespace realpaver {

NodePropagatorAffine::NodePropagatorAffine(ContractorFactory &facto)
    : NodePropagator()
{
   ctc_ = facto.makeAffine();
}

Proof NodePropagatorAffine::contractImpl(IntervalBox &B, const Bitset &active)
{
   return ctc_->contract(B, active);
}

} // namespace realpaver
