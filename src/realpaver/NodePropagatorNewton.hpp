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
 * @file   NodePropagatorNewton.hpp
 * @brief  Node propagator based on the interval Newton operator
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_NEWTON_HPP
#define REALPAVER_NODE_PROPAGATOR_NEWTON_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"

namespace realpaver {

/// Interval Newton operator
class NodePropagatorNewton : public NodePropagator {
public:
   /// Constructor
   NodePropagatorNewton(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorNewton(const NodePropagatorNewton &) = default;

   /// No assignment
   NodePropagatorNewton &operator=(const NodePropagatorNewton &) = delete;

   /// Default destructor
   ~NodePropagatorNewton() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor nwt_;
};

} // namespace realpaver

#endif
