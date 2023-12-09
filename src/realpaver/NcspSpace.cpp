///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

NcspSpace::~NcspSpace() {}

IntervalBox NcspSpace::hullOfPendingNodes() const
{
   size_t n = nbPendingNodes();
   if (n == 0) THROW("No pending node in the space");

   SharedNcspNode node = getPendingNode(0);
   IntervalBox* regnode = node->region();

   IntervalBox h(*regnode);

   for (size_t i=1; i<n; ++i)
   {
      node = getPendingNode(i);
      regnode = node->region();
      h.hullAssignOnScope(*regnode, h.scope());
   }
   return h;
}

void NcspSpace::insertPendingNodes(NcspSplit::iterator first,
                                   NcspSplit::iterator last)
{
   for (auto it = first; it != last; ++it)
      insertPendingNode(*it);
}

} // namespace
