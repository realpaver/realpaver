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
 * @brief  Propagators of for search tree nodes
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NODE_PROPAGATOR_HPP
#define REALPAVER_NODE_PROPAGATOR_HPP

#include "realpaver/ContractorFactory.hpp"
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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

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

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   std::vector<SharedNodePropagator> v_;
};

} // namespace realpaver

#endif
