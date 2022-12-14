///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceDMDFS.hpp"

namespace realpaver {

NcspSpaceDMDFS::NcspSpaceDMDFS()
      : vnode_(), vsol_(), stotal_(0)
{}

size_t NcspSpaceDMDFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceDMDFS::pushSolNode(const SharedNcspNode& node)
{
   stotal_++;
   vsol_.push_back(node);

   // update the distances
   for (auto& elem : vnode_)
   {
      IntervalRegion* reg = node->region();
      double d = elem.node->region()->distance(*reg);
      if (d < elem.mindist) elem.mindist = d;
   }
   
   // ascending ordering of the node distances
   std::sort(vnode_.begin(), vnode_.end(), comparator);
}

SharedNcspNode NcspSpaceDMDFS::popSolNode()
{
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode NcspSpaceDMDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

   return vsol_[i];
}

bool NcspSpaceDMDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

void NcspSpaceDMDFS::makeSolClusters(double gap)
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

   // after the clustering phase, it is necessary to update the distance between
   // each pending node and its closest solution.
   for (auto& elem : vnode_)
   {
      IntervalRegion* reg = elem.node->region();
      elem.mindist = Double::inf();

      for (auto& sol : vsol_)
      {
         IntervalRegion* regsol = sol->region();
         double d = reg->distance(*regsol);
         if (d < elem.mindist) elem.mindist = d;
      }
   }
   
   // ascending ordering of the node distances
   std::sort(vnode_.begin(), vnode_.end(), comparator);
}

size_t NcspSpaceDMDFS::nbTotalSolNodes() const
{
   return stotal_;
}

size_t NcspSpaceDMDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspSpaceDMDFS::nextPendingNode()
{
   SharedNcspNode node = vnode_.back().node;
   vnode_.pop_back();
   return node;
}

void NcspSpaceDMDFS::insertPendingNode(const SharedNcspNode& node)
{
   // Finds the distance to the closest solution
   double d = Double::inf();
   for (auto& sol : vsol_)
   {
      IntervalRegion* reg = sol->region();
      double e = node->region()->distance(*reg);
      if (e < d) d = e;
   }

   Elem elem = { node, d };
   vnode_.push_back(elem);
}

SharedNcspNode NcspSpaceDMDFS::getPendingNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a pending node in a CSP space");

   return vnode_[i].node;
}

} // namespace
