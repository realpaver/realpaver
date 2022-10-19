///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceDMDFS.hpp"

namespace realpaver {


size_t NcspSpaceDMDFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceDMDFS::pushSolNode(const SharedNcspNode& node)
{
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
