///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

NcspSpace::~NcspSpace() {}

size_t NcspSpace::nbFinalNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspSpace::getFinalNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a final node in a CSP space");

   return vnode_[i];
}

void NcspSpace::pushFinalNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);
}

SharedNcspNode NcspSpace::popFinalNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

} // namespace
