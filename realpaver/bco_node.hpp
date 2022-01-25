// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_BCO_NODE_HPP
#define REALPAVER_BCO_NODE_HPP

#include "realpaver/box.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of tree nodes for bound-constrained optimization.
 *****************************************************************************/
class BcoNode {
public:
   // constructors
   BcoNode(const Box& B);
   BcoNode(const BcoNode& node);

   // copy protection
   BcoNode& operator=(const BcoNode& node) = delete;

   // destructor
   ~BcoNode();

   // management of the lower bound of the objective function
   double getLower() const;
   void setLower(const double& val);

   // management of the upper bound of the objective function
   double getUpper() const;
   void setUpper(const double& val);

   // management of the depth in the search tree
   size_t getDepth() const;
   void setDepth(size_t d);
   void incrDepth();

   // returns the box enclosed
   Box* getBox() const;

private:
   Box* box_;        // variable domains
   size_t depth_;    // depth in the search tree
   double lower_;    // lower bound of f in this box
   double upper_;    // upper bound of f in this box
};

std::ostream& operator<<(std::ostream& os, const BcoNode& node);

typedef std::shared_ptr<BcoNode> SharedBcoNode;

inline double BcoNode::getLower() const
{
   return lower_;
}

inline void BcoNode::setLower(const double& val)
{
   lower_ = val;
}

inline double BcoNode::getUpper() const
{
   return upper_;
}

inline void BcoNode::setUpper(const double& val)
{
   upper_ = val;
}

inline size_t BcoNode::getDepth() const
{
   return depth_;
}

inline void BcoNode::incrDepth()
{
   ++depth_;
}

inline Box* BcoNode::getBox() const
{
   return box_;
}

inline void BcoNode::setDepth(size_t d)
{
   depth_ = d;
}

} // namespace

#endif
