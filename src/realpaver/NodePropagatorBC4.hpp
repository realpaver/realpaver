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

#ifndef REALPAVER_NODE_PROPAGATOR_BC4_HPP
#define REALPAVER_NODE_PROPAGATOR_BC4_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"

namespace realpaver {

/// BC4 constraint propagation algorithm
class NodePropagatorBC4 : public NodePropagator {
public:
   /// Constructor
   NodePropagatorBC4(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorBC4(const NodePropagatorBC4 &) = default;

   /// No assignment
   NodePropagatorBC4 &operator=(const NodePropagatorBC4 &) = delete;

   /// Default destructor
   ~NodePropagatorBC4() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor bc4_;
};

} // namespace realpaver

#endif
