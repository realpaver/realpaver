///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPSpace.hpp"

namespace realpaver {

CSPSpace::~CSPSpace() {}

size_t CSPSpace::nbFinalNodes() const
{
   return vnode_.size();
}

SharedCSPNode CSPSpace::getFinalNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a final node in a CSP space");

   return vnode_[i];
}

void CSPSpace::pushFinalNode(const SharedCSPNode& node)
{
   vnode_.push_back(node);
}

SharedCSPNode CSPSpace::popFinalNode()
{
   SharedCSPNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

} // namespace
