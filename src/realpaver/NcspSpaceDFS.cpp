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
 * @file   NcspSpaceDFS.cpp
 * @brief  Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceDFS.hpp"

namespace realpaver {

NcspSpaceDFS::NcspSpaceDFS()
      : vsol_(), vnode_()
{}

size_t NcspSpaceDFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceDFS::pushSolNode(const SharedNcspNode& node)
{
   vsol_.push_back(node);
}

SharedNcspNode NcspSpaceDFS::popSolNode()
{
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode NcspSpaceDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(),
          "Bad access to a solution node in a NCSP space @ " << i);

   return vsol_[i];
}

bool NcspSpaceDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t NcspSpaceDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspSpaceDFS::nextPendingNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

void NcspSpaceDFS::insertPendingNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);
}

SharedNcspNode NcspSpaceDFS::getPendingNode(size_t i) const
{
   ASSERT(i < vnode_.size(),
          "Bad access to a pending node in a NCSP space @ " << i);

   return vnode_[i];
}

} // namespace
