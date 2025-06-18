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
 * @file   SpaceBFS.cpp
 * @brief  Search space for a Breadth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/SpaceBFS.hpp"

namespace realpaver {

SpaceBFS::SpaceBFS()
    : Space()
    , vsol_()
    , lnode_()
{
}

size_t SpaceBFS::nbSolNodes() const
{
   return vsol_.size();
}

void SpaceBFS::pushSolNode(const SharedNode &node)
{
   vsol_.push_back(node);
}

SharedNode SpaceBFS::popSolNode()
{
   SharedNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNode SpaceBFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a space");

   return vsol_[i];
}

bool SpaceBFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t SpaceBFS::nbPendingNodes() const
{
   return lnode_.size();
}

SharedNode SpaceBFS::nextPendingNode()
{
   SharedNode node = lnode_.back();
   lnode_.pop_back();
   return node;
}

void SpaceBFS::insertPendingNode(const SharedNode &node)
{
   lnode_.push_front(node);
}

SharedNode SpaceBFS::getPendingNode(size_t i) const
{
   ASSERT(i < lnode_.size(), "Bad access to a pending node in a space");

   auto it = lnode_.begin();
   if (i != 0)
      std::advance(it, i);

   return *it;
}

} // namespace realpaver
