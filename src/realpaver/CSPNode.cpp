///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/CSPNode.hpp"

namespace realpaver {

CSPNode::CSPNode(const Scope& scope, const IntervalRegion& reg,  int depth)
      : SearchNode(scope, reg, 0),
        proof_(Proof::Maybe)
{}

Proof CSPNode::getProof() const
{
   return proof_;
}

void CSPNode::setproof(Proof p)
{
   proof_ = p;
}

std::ostream& operator<<(std::ostream& os, const CSPNode& node)
{
   os << *node.region()
      << " proof: " << node.getProof();

   return os;
}

} // namespace
