///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceDFS.hpp"

namespace realpaver {

NcspSpaceDFS::NcspSpaceDFS()
      : vsol_(), vnode_(), stotal_(0)
{}

size_t NcspSpaceDFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceDFS::pushSolNode(const SharedNcspNode& node)
{
   stotal_++;
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
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

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

void NcspSpaceDFS::makeSolClusters(double gap)
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

size_t NcspSpaceDFS::nbTotalSolNodes() const
{
   return stotal_;
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
   ASSERT(i < vnode_.size(), "Bad access to a pending node in a CSP space");

   return vnode_[i];
}

} // namespace
