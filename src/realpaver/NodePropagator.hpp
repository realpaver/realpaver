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
 * @file   NodePropagator.hpp
 * @brief  Base class of node propagators
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_NODE_PROPAGATOR_HPP
#define REALPAVER_NODE_PROPAGATOR_HPP

#include "realpaver/IntervalBox.hpp"
#include "realpaver/Node.hpp"
#include "realpaver/NodeContext.hpp"
#include <vector>

namespace realpaver {

/// Type of reduction methods for constraint systems
enum class NodePropagAlgo {
   HC4,      ///< propagation algorithm based on HC4 contractors
   BC4,      ///< propagation algorithm based on BC4 contractors
   AFFINE,   ///< propagation algorithm based on affine contractors
   ACID,     ///< adaptive CID
   Polytope, ///< polytope hull contractor
   Newton    ///< interval Newton operator
};

/**
 * @brief Base class of propagators for tree search nodes.
 *
 * A propagator is an algorithm / operator that contracts the domains of
 * variables occurring in a search node.
 *
 * Concrete propagators are built in contractor factories.
 */
class NodePropagator {
public:
   /// Constructor
   NodePropagator();

   /// Default copy constructor
   NodePropagator(const NodePropagator &) = default;

   /// No assignment
   NodePropagator &operator=(const NodePropagator &) = delete;

   /// Virtual destructor
   virtual ~NodePropagator();

   /**
    * @brief Contraction method.
    *
    * It applies first contractImpl and then the domain contractors for
    * the variables with disconnected domains.
    *
    * The bitset stores the constraint activities.
    *
    * Returns a certificate of proof
    */
   Proof contract(Node &node, NodeContext &ctx);

   /// Reduces box by intersecting it with B
   static Proof contractBox(const IntervalBox &B, DomainBox &box);

   /// Contraction method called by contract, to be overriden in sub-classes
   virtual Proof contractImpl(IntervalBox &B, const Bitset &active) = 0;
};

/// Type of shared pointers on node propagators
using SharedNodePropagator = std::shared_ptr<NodePropagator>;

} // namespace realpaver

#endif
