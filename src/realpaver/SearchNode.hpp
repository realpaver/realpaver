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
   SearchNode(int depth = 0);

   /// Virtual destructor
   virtual ~SearchNode();

   /// Default copy constructor
   SearchNode(const SearchNode&) = default;

   /// Default assignment operator
   SearchNode& operator=(const SearchNode&) = default;

   /// @return the depth of this in the search tree
   int getDepth() const;

   /// Sets the depth of this in the search tree
   /// @param depth new depth
   void setDepth(int depth);

   /// Increments the depth of this
   void incrementDepth();

private:
   int depth_;
};

} // namespace

#endif
