///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceBFS.hpp"

namespace realpaver {

size_t NcspSpaceBFS::nbPendingNodes() const
{
   return lnode_.size();
}

SharedNcspNode NcspSpaceBFS::extractPendingNode()
{
   SharedNcspNode node = lnode_.back();
   lnode_.pop_back();
   return node;
}

void NcspSpaceBFS::insertPendingNode(const SharedNcspNode& node)
{
   lnode_.push_front(node);
}

SharedNcspNode NcspSpaceBFS::getPendingNode(size_t i) const
{
   ASSERT(i < lnode_.size(), "Bad access to a pending node in a CSP space");

   auto it = lnode_.begin();
   if (i != 0)
      std::advance(it, i);

   return *it;
}

} // namespace
