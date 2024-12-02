/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   CSPNode.cpp
 * @brief  CSP search node
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPNode.hpp"

namespace realpaver {

CSPNode::CSPNode(Scope scop, int depth)
      : scop_(scop),
        box_(nullptr),
        depth_(depth),
        index_(-1),
        parent_(-1),
        proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad depth of a CSP node " << depth);
   ASSERT(!scop.isEmpty(), "Empty scope used to create a CSP node");

   box_ = new DomainBox(scop);
}

CSPNode::CSPNode(std::unique_ptr<DomainBox> box, int depth)
      : scop_(box->scope()),
        box_(box.release()),
        depth_(depth),
        index_(-1),
        parent_(-1),
        proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad depth of a CSP node " << depth);
}

CSPNode::CSPNode(const CSPNode& node)
      : scop_(node.scop_),
        box_(nullptr),
        depth_(node.depth_),
        index_(-1),
        parent_(-1),
        proof_(Proof::Maybe)
{
   box_ = node.box_->clone();
}

CSPNode::~CSPNode()
{
   delete box_;
}

int CSPNode::depth() const
{
   return depth_;
}

void CSPNode::setDepth(int depth)
{
   ASSERT(depth >= 0, "Bad node depth " << depth);

   depth_ = depth;
}

int CSPNode::index() const
{
   return index_;
}

void CSPNode::setIndex(int id)
{
   index_ = id;
}

int CSPNode::parent() const
{
   return parent_;
}

void CSPNode::setParent(int p)
{
   parent_ = p;
}

void CSPNode::incrementDepth()
{
   ++depth_;
}

DomainBox* CSPNode::box() const
{
   return box_;
}

Scope CSPNode::scope() const
{
   return scop_;
}

Proof CSPNode::getProof() const
{
   return proof_;
}

void CSPNode::setProof(Proof p)
{
   proof_ = p;
}

std::ostream& operator<<(std::ostream& os, const CSPNode& node)
{
   os << *node.box()
      << " proof: " << node.getProof();

   return os;
}

} // namespace
