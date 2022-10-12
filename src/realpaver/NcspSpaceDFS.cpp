///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NcspSpaceDFS.hpp"

namespace realpaver {

size_t NcspSpaceDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspSpaceDFS::getPendingNode() const
{
   return vnode_.back();
}

SharedNcspNode NcspSpaceDFS::popPendingNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void NcspSpaceDFS::insertPendingNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);
}

} // namespace
