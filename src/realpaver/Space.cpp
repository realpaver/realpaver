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
 * @file   Space.cpp
 * @brief  Base class of search spaces
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/Space.hpp"
#include "realpaver/UnionFind.hpp"
#include <list>

namespace realpaver {

Space::~Space()
{
}

void Space::insertPendingNodes(Split::iterator first, Split::iterator last)
{
   for (auto it = first; it != last; ++it)
      insertPendingNode(*it);
}

// void Space::makeSolClusters(double gap)
// {
//    size_t nsol = nbSolNodes();

//    // no clustering ?
//    if (gap < 0.0 || nsol <= 1)
//       return;

//    // gets the solution nodes
//    std::vector<SharedNode> vnod(nsol);
//    for (size_t i = 0; i < nsol; ++i)
//       vnod[i] = popSolNode();

//    // makes the clusters using a UnionFind data structure
//    UnionFind uf(nsol);
//    for (size_t i = 0; i < nsol - 1; ++i)
//       for (size_t j = i + 1; j < nsol; ++j)
//          if (vnod[i]->box()->gap(*vnod[j]->box()) <= gap)
//             uf.merge(i, j);

//    // gets the clusters
//    std::vector<std::vector<size_t>> parts = uf.parts();

//    // makes the solutions, one for each part
//    for (size_t i = 0; i < parts.size(); ++i)
//    {
//       std::vector<size_t> part = parts[i];

//       // gets the first node of the part
//       size_t idxnod = part[0];
//       SharedNode sol = vnod[idxnod];

//       // glues the boxes of the other nodes of the part
//       for (size_t j = 1; j < part.size(); ++j)
//       {
//          idxnod = part[j];
//          sol->box()->glueOnScope(*vnod[idxnod]->box(), sol->scope());
//       }
//       pushSolNode(sol);
//    }
// }

void Space::makeSolClusters(double gap)
{
   size_t nsol = nbSolNodes();

   // no clustering?
   if (gap < 0.0 || nsol <= 1)
      return;

   // moves the solution nodes in a list
   std::list<SharedNode> lnode;
   while (nbSolNodes() > 0)
      lnode.push_back(popSolNode());

   // resulting list of nodes
   std::list<SharedNode> res;

   while (!lnode.empty())
   {
      // extracts the first node
      SharedNode node = lnode.front();
      DomainBox *box = node->box();
      lnode.pop_front();

      // finds another node that is close enough
      bool found = false;
      auto it = lnode.begin();
      while (!found && it != lnode.end())
      {
         SharedNode bis = *it;
         DomainBox *boxbis = bis->box();

         if (box->gap(*boxbis) <= gap)
         {
            // merges node in bis and iterates
            boxbis->glueOnScope(*box, node->scope());
            found = true;
         }
         else
            ++it;
      }

      // this is a solution node and no other solution is close enough
      if (!found)
         res.push_back(node);
   }

   // pushes the solution nodes in this
   for (const auto &node : res)
      pushSolNode(node);
}

} // namespace realpaver
