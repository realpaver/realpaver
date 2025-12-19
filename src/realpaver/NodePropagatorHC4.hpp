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
 * @file   NodePropagatorHC4.hpp
 * @brief  Propagator for nodes based on HC4
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_HC4_HPP
#define REALPAVER_NODE_PROPAGATOR_HC4_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"

namespace realpaver {

/// HC4 constraint propagation algorithm
class NodePropagatorHC4 : public NodePropagator {
public:
   /// Constructor
   NodePropagatorHC4(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorHC4(const NodePropagatorHC4 &) = default;

   /// No assignment
   NodePropagatorHC4 &operator=(const NodePropagatorHC4 &) = delete;

   /// Default destructor
   ~NodePropagatorHC4() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor hc4_;
};

} // namespace realpaver

#endif
