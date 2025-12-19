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
 * @file   NodePropagatorAffine.hpp
 * @brief  Node propagator based on affine contractors
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_AFFINE_HPP
#define REALPAVER_NODE_PROPAGATOR_AFFINE_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"

namespace realpaver {

/// Constraint propagation algorithm based on affine contractors
class NodePropagatorAffine : public NodePropagator {
public:
   /// Constructor
   NodePropagatorAffine(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorAffine(const NodePropagatorAffine &) = default;

   /// No assignment
   NodePropagatorAffine &operator=(const NodePropagatorAffine &) = delete;

   /// Default destructor
   ~NodePropagatorAffine() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor ctc_;
};

} // namespace realpaver

#endif
