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
 * @file   Node.cpp
 * @brief  Node of search tree
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Bitset.hpp"
#include "realpaver/Node.hpp"

namespace realpaver {

Node::Node(Scope scop, int nbc, int depth)
    : scop_(scop)
    , box_(nullptr)
    , active_(nbc)
    , depth_(depth)
    , index_(-1)
    , parent_(-1)
    , proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad node depth: " << depth);
   ASSERT(!scop.isEmpty(), "Empty scope used to create a node");

   box_ = new DomainBox(scop);
   active_.setAllOne();
}

Node::Node(std::unique_ptr<DomainBox> box, int nbc, int depth)
    : scop_(box->scope())
    , box_(box.release())
    , active_(nbc)
    , depth_(depth)
    , index_(-1)
    , parent_(-1)
    , proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad node depth: " << depth);
   active_.setAllOne();
}

Node::Node(const Node &node)
    : scop_(node.scop_)
    , box_(nullptr)
    , active_(node.active_)
    , depth_(node.depth_)
    , index_(-1)
    , parent_(-1)
    , proof_(Proof::Maybe)
{
   box_ = node.box_->clone();
}

Node::~Node()
{
   delete box_;
}

int Node::depth() const
{
   return depth_;
}

void Node::setDepth(int depth)
{
   ASSERT(depth >= 0, "Bad node depth: " << depth);

   depth_ = depth;
}

int Node::index() const
{
   return index_;
}

void Node::setIndex(int id)
{
   index_ = id;
}

int Node::parent() const
{
   return parent_;
}

void Node::setParent(int p)
{
   parent_ = p;
}

void Node::incrementDepth()
{
   ++depth_;
}

DomainBox *Node::box() const
{
   return box_;
}

Scope Node::scope() const
{
   return scop_;
}

Proof Node::getProof() const
{
   return proof_;
}

void Node::setProof(Proof p)
{
   proof_ = p;
}

bool Node::isActive(size_t i) const
{
   ASSERT(i < active_.size(), "Bad access to the activity of a constraint @ " << i);
   return active_.get(i) != 0;
}

void Node::setActive(size_t i)
{
   ASSERT(i < active_.size(), "Bad access to the activity of a constraint @ " << i);
   active_.setOne(i);
}

bool Node::isInactive(size_t i) const
{
   ASSERT(i < active_.size(), "Bad access to the activity of a constraint @ " << i);
   return active_.get(i) == 0;
}

void Node::setInactive(size_t i)
{
   ASSERT(i < active_.size(), "Bad access to the activity of a constraint @ " << i);
   active_.setZero(i);
}

bool Node::areAllActive() const
{
   return active_.areAllOnes();
}

bool Node::areAllInactive() const
{
   return active_.areAllZeros();
}

void Node::setAllActive()
{
   active_.setAllOne();
}

void Node::setAllInactive()
{
   active_.setAllZero();
}

const Bitset &Node::active() const
{
   return active_;
}

std::ostream &operator<<(std::ostream &os, const Node &node)
{
   os << *node.box() << " proof: " << node.getProof();

   return os;
}

} // namespace realpaver
