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

BONode::BONode(const Scope& scope, const Variable& objvar,
               const IntervalRegion& reg)
      : SearchNode(0),
        scope_(scope),
        objvar_(objvar),
        reg_(nullptr),
        lower_(Double::neginf()),
        upper_(Double::inf())
{
   reg_ = new IntervalRegion(reg);
}

BONode::BONode(const BONode& node)
      : SearchNode(node),
        scope_(node.scope_),
        objvar_(node.objvar_),
        reg_(nullptr),
        lower_(node.lower_),
        upper_(node.upper_)
{
   reg_ = new IntervalRegion(*node.reg_);
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

IntervalRegion* BONode::getRegion() const
{
   return reg_;
}

Scope BONode::getScope() const
{
   return scope_;
}

Variable BONode::getObjVar() const
{
   return objvar_;
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
