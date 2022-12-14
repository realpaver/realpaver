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

NcspSpaceBFS::NcspSpaceBFS()
      : vsol_(), lnode_(), stotal_(0)
{}

size_t NcspSpaceBFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceBFS::pushSolNode(const SharedNcspNode& node)
{
   stotal_++;
   vsol_.push_back(node);
}

SharedNcspNode NcspSpaceBFS::popSolNode()
{
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode NcspSpaceBFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

   return vsol_[i];
}

bool NcspSpaceBFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

void NcspSpaceBFS::makeSolClusters(double gap)
{
   // no clustering if the gap is negative
   if (gap < 0.0) return;

   // moves the solution nodes in a list
   std::list<SharedNcspNode> lnode;
   for (auto node : vsol_) lnode.push_back(node);
   vsol_.clear();

   while (!lnode.empty())
   {
      // extracts the first node
      SharedNcspNode node = lnode.front();
      IntervalRegion* regnode = node->region();
      lnode.pop_front();

      // finds another node that is close enough
      bool found = false;
      auto it = lnode.begin();
      while (!found && it != lnode.end())
      {
         SharedNcspNode bis = *it;
         IntervalRegion* regbis = bis->region();

         if (regnode->gap(*regbis) <= gap)
         {
            // merges node in bisnode and iterates
            regbis->hullAssignOnScope(*regnode, node->scope());
            found = true;
         }
         else ++it;
      }

      // this is a solution node and no other solution is close enough
      if (!found) vsol_.push_back(node);
   }
}

size_t NcspSpaceBFS::nbTotalSolNodes() const
{
   return stotal_;
}

size_t NcspSpaceBFS::nbPendingNodes() const
{
   return lnode_.size();
}

SharedNcspNode NcspSpaceBFS::nextPendingNode()
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
