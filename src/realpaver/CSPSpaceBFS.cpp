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
 * @file   CSPSpaceBFS.cpp
 * @brief  Breadth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPSpaceBFS.hpp"

namespace realpaver {

CSPSpaceBFS::CSPSpaceBFS()
      : vsol_(), lnode_()
{}

size_t CSPSpaceBFS::nbSolNodes() const
{
   return vsol_.size();
}

void CSPSpaceBFS::pushSolNode(const SharedNcspNode& node)
{
   vsol_.push_back(node);
}

SharedNcspNode CSPSpaceBFS::popSolNode()
{
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode CSPSpaceBFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a NCSP space");

   return vsol_[i];
}

bool CSPSpaceBFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t CSPSpaceBFS::nbPendingNodes() const
{
   return lnode_.size();
}

SharedNcspNode CSPSpaceBFS::nextPendingNode()
{
   SharedNcspNode node = lnode_.back();
   lnode_.pop_back();
   return node;
}

void CSPSpaceBFS::insertPendingNode(const SharedNcspNode& node)
{
   lnode_.push_front(node);
}

SharedNcspNode CSPSpaceBFS::getPendingNode(size_t i) const
{
   ASSERT(i < lnode_.size(), "Bad access to a pending node in a NCSP space");

   auto it = lnode_.begin();
   if (i != 0)
      std::advance(it, i);

   return *it;
}

} // namespace
