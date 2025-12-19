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
 * @file   NodePropagatorACID.hpp
 * @brief  Node propagator based on ACID
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_ACID_HPP
#define REALPAVER_NODE_PROPAGATOR_ACID_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"
#include <vector>

namespace realpaver {

/// ACID algorithm based on HC4 contractors
class NodePropagatorACID : public NodePropagator {
public:
   /// Constructor
   NodePropagatorACID(ContractorFactory &facto);

   /// Default copy constructor
   NodePropagatorACID(const NodePropagatorACID &) = default;

   /// No assignment
   NodePropagatorACID &operator=(const NodePropagatorACID &) = delete;

   /// Default destructor
   ~NodePropagatorACID() = default;

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedContractor hc4_;
   SharedContractor acid_;
};

} // namespace realpaver

#endif
