///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "SearchNode.hpp"

namespace realpaver {

SearchNode::SearchNode(Scope scope, const IntervalBox& box, int depth)
      : scope_(scope),
        box_(nullptr),
        depth_(depth),
        index_(0),
        v_()
{
   ASSERT(depth >= 0, "Bad depth of a search node " << depth);
   ASSERT(box.scope().contains(scope), "Bad scopes in a search node");

   box_ = new IntervalBox(box);
}

SearchNode::SearchNode(const SearchNode& node)
      : scope_(node.scope_),
        box_(nullptr),
        depth_(node.depth_),
        v_(node.v_)
{
   box_ = node.box_->clone();
}

SearchNode::~SearchNode()
{
   delete box_;
}

int SearchNode::depth() const
{
   return depth_;
}

void SearchNode::setDepth(int depth)
{
   ASSERT(depth >= 0, "Bad node depth " << depth);

   depth_ = depth;
}

int SearchNode::index() const
{
   return index_;
}

void SearchNode::setIndex(int id)
{
   index_ = id;
}

void SearchNode::incrementDepth()
{
   ++depth_;
}

IntervalBox* SearchNode::region() const
{
   return box_;
}

Scope SearchNode::scope() const
{
   return scope_;
}

Variable SearchNode::splitVariable() const
{
   return v_;
}

void SearchNode::setSplitVariable(Variable v)
{
   ASSERT(scope_.contains(v), "Bad assignment of the last split variable");

   v_ = v;
}

} // namespace
