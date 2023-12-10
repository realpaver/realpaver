///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SEARCH_NODE_HPP
#define REALPAVER_SEARCH_NODE_HPP

#include "realpaver/IntervalBox.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for search nodes.
///
/// A node has a scope, an interval box, an index, a tree depth, and it is
/// possible to register a variable, e.g. the selected variable in the parent
/// node whose domain has been split.
///
/// The scope is shared. It may be the set of variables considered by some
/// splitting component.
///
/// The interval box is owned by this. Its scope is assumed to contain
/// the scope of this.
///////////////////////////////////////////////////////////////////////////////
class SearchNode {
public:
   /// Creates a node
   /// @param scope set of variables
   /// @param box domains of variables
   /// @param depth depth of this
   SearchNode(Scope scope, const IntervalBox& box,  int depth = 0);

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

   /// @return the domains of this
   IntervalBox* region() const;

   /// @return the scope of this
   Scope scope() const;

   /// @return the variable chosen by the last splitting step
   Variable splitVariable() const;

   /// Assigns the variable whose domain has been split
   /// @param v a variablein the scope of this
   void setSplitVariable(Variable v);

private:
   Scope scope_;
   IntervalBox* box_;
   int depth_, index_;
   Variable v_;
};

} // namespace

#endif
