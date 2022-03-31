///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SEARCH_NODE_HPP
#define REALPAVER_SEARCH_NODE_HPP

#include "realpaver/IntervalRegion.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for search nodes.
///
/// It just manages the tree depth.
///////////////////////////////////////////////////////////////////////////////
class SearchNode {
public:
   /// Creates a node
   /// @param depth depth of this
   SearchNode(const Scope& scope, const IntervalRegion& reg,  int depth = 0);

   /// Virtual destructor
   virtual ~SearchNode();

   /// Copy constructor
   SearchNode(const SearchNode& node);

   /// No assignment
   SearchNode& operator=(const SearchNode&) = delete;

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

   /// @return the vector of domains of this
   IntervalRegion* region() const;

   /// @return the scope of this
   Scope scope() const;

   /// @return the variable chosen by the last splitting step
   Variable splitVariable() const;

   /// Assigns the variable whose domain has been split
   /// @param v a variablein the scope of this
   void setSplitVariable(Variable v);

private:
   Scope scope_;
   IntervalRegion* reg_;
   int depth_, index_;
   Variable v_;
};

} // namespace

#endif
