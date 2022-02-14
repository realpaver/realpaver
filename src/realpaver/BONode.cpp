///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BONode.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

BONode::BONode(const IntervalVector& X) : SearchNode(0)
{
   reg_ = new IntervalVector(X);
   lower_ = Double::neginf();
   upper_ = Double::inf();
}

BONode::BONode(const BONode& node) : SearchNode(node)
{
   reg_ = new IntervalVector(*node.reg_);
   lower_ = node.lower_;
   upper_ = node.upper_;
}

BONode::~BONode()
{
   delete reg_;
}

double BONode::getLower() const
{
   return lower_;
}

void BONode::setLower(const double& val)
{
   lower_ = val;
}

double BONode::getUpper() const
{
   return upper_;
}

void BONode::setUpper(const double& val)
{
   upper_ = val;
}

IntervalVector* BONode::getRegion() const
{
   return reg_;
}

std::ostream& operator<<(std::ostream& os, const BONode& node)
{
   os << *node.getRegion()
      << " d: " << node.getDepth()
      << " L: " << node.getLower()
      << " U: " << node.getUpper();

   return os;
}

} // namespace
