///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

NcspSpace::~NcspSpace() {}

void NcspSpace::insertPendingNodes(NcspSplit::iterator first,
                                   NcspSplit::iterator last)
{
   for (auto it = first; it != last; ++it)
      insertPendingNode(*it);
}

void NcspSpace::makeSolClusters(double gap)
{
   // no clustering if the gap is negative
   if (gap < 0.0) return;

   // moves the solution nodes in a list
   std::list<SharedNcspNode> lnode;
   while (nbSolNodes() > 0)
      lnode.push_back(popSolNode());

   // resulting list of nodes
   std::list<SharedNcspNode> res;

   while (!lnode.empty())
   {
      // extracts the first node
      SharedNcspNode node = lnode.front();
      DomainBox* box = node->box();
      lnode.pop_front();

      // finds another node that is close enough
      bool found = false;
      auto it = lnode.begin();
      while (!found && it != lnode.end())
      {
         SharedNcspNode bis = *it;
         DomainBox* boxbis = bis->box();

         if (box->gap(*boxbis) < gap)
         {
            // merges node in bisnode and iterates
            boxbis->glueOnScope(*box, node->scope());
            found = true;
         }
         else ++it;
      }

      // this is a solution node and no other solution is close enough
      if (!found) res.push_back(node);
   }

   // pushes the solution nodes in this
   for (const auto& node : res)
      pushSolNode(node);
}

} // namespace
