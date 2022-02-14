///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "SearchNode.hpp"

namespace realpaver {

SearchNode::SearchNode(int depth)
{
   setDepth(depth);
}

SearchNode::~SearchNode()
{}

int SearchNode::getDepth() const
{
   return depth_;
}

void SearchNode::setDepth(int depth)
{
   ASSERT(depth >= 0, "Bad node depth " << depth);

   depth_ = depth;
}

void SearchNode::incrementDepth()
{
   ++depth_;
}

} // namespace
