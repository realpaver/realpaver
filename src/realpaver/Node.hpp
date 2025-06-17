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
 * @file   Node.hpp
 * @brief  Node of search tree
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NODE_HPP
#define REALPAVER_NODE_HPP

#include "realpaver/DomainBox.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Node of search tree.
 *
 * A node has a scope, a domain box, a bitset, an index, a tree depth, a proof
 * certificate, and it is possible to register a variable, e.g. the selected
 * variable in the parent node whose domain has been split.
 *
 * The scope is shared. It may be the set of variables considered by some
 * splitting component.
 *
 * The domain box is owned by this. Its scope is assumed to contain
 * the scope of this.
 *
 * The bitset stores the activity status of the constraints.
 */
class Node {
public:
   /**
    * @brief Creates a node from a scope.
    *
    * The variable domains are extracted from the variables in order to create a box.
    * The number of constraints nbc is used to create a bitset that stores the
    * activity status of the constraints. The depth in the search tree is also given.
    */
   Node(Scope scop, int nbc, int depth = 0);

   /**
    * @brief Creates a node from a box.
    *
    * The given box is cloned. The number of constraints nbc is used to create a bitset
    * that stores the activity status of the constraints. The depth in the search tree is
    * also given.
    */
   Node(std::unique_ptr<DomainBox> box, int nbc, int depth = 0);

   /// Virtual destructor
   virtual ~Node();

   /// Copy constructor
   Node(const Node &node);

   /// No assignment
   Node &operator=(const Node &) = delete;

   /// Returns the depth of this in the search tree
   int depth() const;

   /// Sets the depth of this in the search tree
   void setDepth(int depth);

   /// Returns the index of this in the search tree
   int index() const;

   /// Sets the index of this in the search tree
   void setIndex(int id);

   /// Returns the index of the parent of this in the search tree
   int parent() const;

   /// Sets the index of the parent node
   void setParent(int p);

   /// Increments the depth of this
   void incrementDepth();

   /// Returns the domains of this
   DomainBox *box() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Returns the proof certificate
   Proof getProof() const;

   /// Sets the proof certificate
   void setProof(Proof p);

   /// Returns true if the i-th constraint is active
   bool isActive(size_t i) const;

   /// Assigns the i-th constraint as an active constraint
   void setActive(size_t i);

   /// Returns true if the i-th constraint is inactive
   bool isInactive(size_t i) const;

   /// Assigns the i-th constraint as an inactive constraint
   void setInactive(size_t i);

   /// Returns true if all the constraints are active
   bool areAllActive() const;

   /// Returns true if all the constraints are inactive
   bool areAllInactive() const;

   /// Activates all the constraints
   void setAllActive();

   /// Deactivates all the constraints
   void setAllInactive();

   /// Returns the bitset storing the constraint activities
   const Bitset &active() const;

private:
   Scope scop_;     // set of variables
   DomainBox *box_; // variable domains
   Bitset active_;  // activity of constraints
   int depth_;      // depth in the search tree
   int index_;      // unique identifier
   int parent_;     // index of parent node
   Proof proof_;    // proof status
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Node &node);

/// Type of shared pointers on search nodes
using SharedNode = std::shared_ptr<Node>;

} // namespace realpaver

#endif
