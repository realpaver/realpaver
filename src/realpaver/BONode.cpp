///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BONode.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

BONode::BONode(const Scope& sco, const Variable& ovar,
               const IntervalRegion& reg)
      : SearchNode(sco, reg, 0),
        objvar_(ovar),
        lower_(Double::neginf()),
        upper_(Double::inf())
{}

double BONode::getLower() const
{
   return lower_;
}

void BONode::setLower(double val)
{
   lower_ = val;
}

double BONode::getUpper() const
{
   return upper_;
}

void BONode::setUpper(double val)
{
   upper_ = val;
}

Variable BONode::getObjVar() const
{
   return objvar_;
}

std::ostream& operator<<(std::ostream& os, const BONode& node)
{
   os << *node.region()
      << " d: " << node.depth()
      << " L: " << node.getLower()
      << " U: " << node.getUpper();

   return os;
}

} // namespace
