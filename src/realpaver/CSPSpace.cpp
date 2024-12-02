/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   CSPSpace.cpp
 * @brief  Spaces of NCSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPSpace.hpp"

namespace realpaver {

CSPSpace::~CSPSpace() {}

void CSPSpace::insertPendingNodes(NcspSplit::iterator first,
                                   NcspSplit::iterator last)
{
   for (auto it = first; it != last; ++it)
      insertPendingNode(*it);
}

void CSPSpace::makeSolClusters(double gap)
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
