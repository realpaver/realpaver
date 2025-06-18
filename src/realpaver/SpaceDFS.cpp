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
 * @file   SpaceDFS.cpp
 * @brief  Search space for a Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/SpaceDFS.hpp"

namespace realpaver {

SpaceDFS::SpaceDFS()
    : Space()
    , vsol_()
    , vnode_()
{
}

size_t SpaceDFS::nbSolNodes() const
{
   return vsol_.size();
}

void SpaceDFS::pushSolNode(const SharedNode &node)
{
   vsol_.push_back(node);
}

SharedNode SpaceDFS::popSolNode()
{
   SharedNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNode SpaceDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a space @ " << i);

   return vsol_[i];
}

bool SpaceDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t SpaceDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNode SpaceDFS::nextPendingNode()
{
   SharedNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void SpaceDFS::insertPendingNode(const SharedNode &node)
{
   vnode_.push_back(node);
}

SharedNode SpaceDFS::getPendingNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a pending node in a space @ " << i);

   return vnode_[i];
}

} // namespace realpaver
