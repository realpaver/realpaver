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

SearchNode::SearchNode(const Scope& scope, const IntervalRegion& reg,
                       int depth)
      : scope_(scope),
        reg_(nullptr),
        depth_(depth),
        v_()
{
   ASSERT(depth >= 0, "Bad depth of a search node " << depth);
   ASSERT(reg.scope().contains(scope), "Bad scopes in a search node");

   reg_ = new IntervalRegion(reg);
}

SearchNode::SearchNode(const SearchNode& node)
      : scope_(node.scope_),
        reg_(nullptr),
        depth_(node.depth_),
        v_(node.v_)
{
   reg_ = node.reg_->clone();
}

SearchNode::~SearchNode()
{
   delete reg_;
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

void SearchNode::incrementDepth()
{
   ++depth_;
}

IntervalRegion* SearchNode::region() const
{
   return reg_;
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
