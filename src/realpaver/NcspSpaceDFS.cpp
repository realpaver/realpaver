///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
