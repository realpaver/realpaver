///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/CSPSpaceDFS.hpp"

namespace realpaver {

size_t CSPSpaceDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedCSPNode CSPSpaceDFS::getPendingNode() const
{
   return vnode_.back();
}

SharedCSPNode CSPSpaceDFS::popPendingNode()
{
   SharedCSPNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void CSPSpaceDFS::insertPendingNode(const SharedCSPNode& node)
{
   vnode_.push_back(node);
}

} // namespace
