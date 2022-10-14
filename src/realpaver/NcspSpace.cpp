///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

NcspSpace::NcspSpace() : vnode_(), feasible_(false)
{}

NcspSpace::~NcspSpace() {}

size_t NcspSpace::nbSolutionNodes() const
{
   return vnode_.size();
}

SharedNcspNode NcspSpace::getSolutionNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a final node in a CSP space");

   return vnode_[i];
}

void NcspSpace::pushSolutionNode(const SharedNcspNode& node)
{
   vnode_.push_back(node);

   Proof p = node->getProof();
   if (p == Proof::Inner || p == Proof::Feasible)
      feasible_ = true;
}

SharedNcspNode NcspSpace::popSolutionNode()
{
   SharedNcspNode node = vnode_.back();
   vnode_.pop_back();
   return node;
}

bool NcspSpace::proofFeasible() const
{
   return feasible_;
}


} // namespace
