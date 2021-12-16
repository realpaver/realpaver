// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_bco_node.hpp"
#include "realpaver_double.hpp"

namespace realpaver {

BcoNode::BcoNode(const Box& B)
{
   box_ = new Box(B);
   depth_ = 0;
   lower_ = DBL_LOWEST();
   upper_ = DBL_GREATEAST();
}

BcoNode::BcoNode(const BcoNode& node)
{
   box_ = new Box(*node.box_);
   depth_ = node.depth_;
   lower_ = node.lower_;
   upper_ = node.upper_;
}

BcoNode::~BcoNode()
{
   delete box_;
}

std::ostream& operator<<(std::ostream& os, const BcoNode& node)
{
   os << *node.getBox()
      << " L: " << node.getLower()
      << " U: " << node.getUpper();

   return os;
}


} // namespace
