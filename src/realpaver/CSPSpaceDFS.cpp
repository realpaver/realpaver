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
 * @file   CSPSpaceDFS.cpp
 * @brief  Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPSpaceDFS.hpp"

namespace realpaver {

CSPSpaceDFS::CSPSpaceDFS()
      : vsol_(), vnode_()
{}

size_t CSPSpaceDFS::nbSolNodes() const
{
   return vsol_.size();
}

void CSPSpaceDFS::pushSolNode(const SharedNcspNode& node)
{
   vsol_.push_back(node);
}

SharedNcspNode CSPSpaceDFS::popSolNode()
{
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode CSPSpaceDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(),
          "Bad access to a solution node in a NCSP space @ " << i);

   return vsol_[i];
}

bool CSPSpaceDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t CSPSpaceDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNcspNode CSPSpaceDFS::nextPendingNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void CSPSpaceDFS::insertPendingNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);
}

SharedNcspNode CSPSpaceDFS::getPendingNode(size_t i) const
{
   ASSERT(i < vnode_.size(),
          "Bad access to a pending node in a NCSP space @ " << i);

   return vnode_[i];
}

} // namespace
