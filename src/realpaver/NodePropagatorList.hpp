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
 * @file   NodePropagatorList.hpp
 * @brief  Node propagator applying a sequence of propagators
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#ifndef REALPAVER_NODE_PROPAGATOR_LIST_HPP
#define REALPAVER_NODE_PROPAGATOR_LIST_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/NodePropagator.hpp"
#include <vector>

namespace realpaver {

/// List of propagators applied in sequence to contract a node
class NodePropagatorList : public NodePropagator {
public:
   /// Constructor
   NodePropagatorList();

   /// Default copy constructor
   NodePropagatorList(const NodePropagatorList &) = default;

   /// No assignment
   NodePropagatorList &operator=(const NodePropagatorList &) = delete;

   /// Default destructor
   ~NodePropagatorList() = default;

   /// Returns the number of contractors
   size_t size() const;

   /// Inserts a new algorithm in the last place
   void pushBack(NodePropagAlgo alg, ContractorFactory &facto);

   /// Inserts a propagator in the last place
   void pushBack(SharedNodePropagator op);

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   std::vector<SharedNodePropagator> v_;
};

} // namespace realpaver

#endif
