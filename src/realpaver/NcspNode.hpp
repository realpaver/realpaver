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
 * @file   NcspNode.hpp
 * @brief  NCSP search node
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_NCSP_NODE_HPP
#define REALPAVER_NCSP_NODE_HPP

#include <memory>
#include "realpaver/DomainBox.hpp"

namespace realpaver {

/**
 * @brief Search node for the NCSP solver.
 *
 * A node has a scope, a domain box, an index, a tree depth, a proof
 * certificate, and it is possible to register a variable, e.g. the selected
 * variable in the parent node whose domain has been split.
 *
 * The scope is shared. It may be the set of variables considered by some
 * splitting component.
 *
 * The domain box is owned by this. Its scope is assumed to contain
 * the scope of this.
 */
class NcspNode {
public:
   /**
    * @brief Constructor from a scope.
    * 
    * The domains are extracted from the variables of the given scope.
    */
   NcspNode(Scope scop, int depth = 0);

   /// Constructor from a box
   NcspNode(std::unique_ptr<DomainBox> box,  int depth = 0);

   /// Virtual destructor
   virtual ~NcspNode();

   /// Copy constructor
   NcspNode(const NcspNode& node);

   /// No assignment
   NcspNode& operator=(const NcspNode&) = delete;

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
   DomainBox* box() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Returns the proof certificate
   Proof getProof() const;

   /// Sets the proof certificate
   void setProof(Proof p);

private:
   Scope scop_;
   DomainBox* box_;
   int depth_, index_, parent_;
   Proof proof_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const NcspNode& node);

/// Type of shared pointers on CSP nodes
using SharedNcspNode = std::shared_ptr<NcspNode>;

} // namespace

#endif
