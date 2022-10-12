///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NcspDfsSpace.hpp"

namespace realpaver {

size_t NcspDfsSpace::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspDfsSpace::getPendingNode() const
{
   return vnode_.back();
}

SharedNcspNode NcspDfsSpace::popPendingNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void NcspDfsSpace::insertPendingNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);
}

} // namespace
