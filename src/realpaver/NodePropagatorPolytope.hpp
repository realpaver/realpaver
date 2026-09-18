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
 * @file   NodePropagatorPolytope.hpp
 * @brief  Node propagator based on the polytope hull contractor
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_POLYTOPE_HPP
#define REALPAVER_NODE_PROPAGATOR_POLYTOPE_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"
#include <vector>

namespace realpaver {

/// Poly hull contractor
class NodePropagatorPolytope : public NodePropagator {
public:
   /// Constructor
   NodePropagatorPolytope(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorPolytope(const NodePropagatorPolytope &) = default;

   /// No assignment
   NodePropagatorPolytope &operator=(const NodePropagatorPolytope &) = delete;

   /// Default destructor
   ~NodePropagatorPolytope() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor poly_;
};

} // namespace realpaver

#endif
