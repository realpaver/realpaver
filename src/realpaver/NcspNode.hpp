///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_NODE_HPP
#define REALPAVER_NCSP_NODE_HPP

#include <memory>
#include "realpaver/DomainBox.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a search node of the NCSP solver.
///
/// A node has a scope, a domain box, an index, a tree depth, a proof
/// certificate, and it is possible to register a variable, e.g. the selected
/// variable in the parent node whose domain has been split.
///
/// The scope is shared. It may be the set of variables considered by some
/// splitting component.
///
/// The domain box is owned by this. Its scope is assumed to contain
/// the scope of this.
///////////////////////////////////////////////////////////////////////////////
class NcspNode {
public:
   /// Creates a node
   /// @param scope set of variables
   /// @param depth depth of this
   ///
   /// The domains are extracted from the variables of the given scope.
   NcspNode(Scope scope, int depth = 0);

   /// Creates a node
   /// @param box the variables and their domains
   /// @param depth depth of this
   NcspNode(std::unique_ptr<DomainBox> box,  int depth = 0);

   /// Virtual destructor
   virtual ~NcspNode();

   /// Copy constructor
   NcspNode(const NcspNode& node);

   /// No assignment
   NcspNode& operator=(const NcspNode&) = delete;

   /// @return the depth of this in the search tree
   int depth() const;

   /// Sets the depth of this in the search tree
   /// @param depth new depth
   void setDepth(int depth);

   /// @return the index of this in the search tree
   int index() const;

   /// Sets the index of this in the search tree
   /// @param id new index
   void setIndex(int id);

   /// Increments the depth of this
   void incrementDepth();

   /// @return the domains of this
   DomainBox* box() const;

   /// @return the scope of this
   Scope scope() const;

   /// @return the variable chosen by the last splitting step
   Variable splitVariable() const;

   /// Assigns the variable whose domain has been split
   /// @param v a variablein the scope of this
   void setSplitVariable(Variable v);

   /// @return the proof certificate
   Proof getProof() const;

   /// Sets the proof certificate
   /// @param p new proof certificate
   void setProof(Proof p);

private:
   Scope scope_;
   DomainBox* box_;
   int depth_, index_;
   Variable v_;
   Proof proof_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const NcspNode& node);

/// Type of shared pointers on CSP nodes
typedef std::shared_ptr<NcspNode> SharedNcspNode;

} // namespace

#endif
