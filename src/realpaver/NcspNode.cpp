///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/NcspNode.hpp"

namespace realpaver {

NcspNode::NcspNode(const Scope& scope, const IntervalRegion& reg,  int depth)
      : SearchNode(scope, reg, 0),
        proof_(Proof::Maybe)
{}

Proof NcspNode::getProof() const
{
   return proof_;
}

void NcspNode::setProof(Proof p)
{
   proof_ = p;
}

std::ostream& operator<<(std::ostream& os, const NcspNode& node)
{
   os << *node.region()
      << " proof: " << node.getProof();

   return os;
}

} // namespace
