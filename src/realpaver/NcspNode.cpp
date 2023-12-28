///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "NcspNode.hpp"

namespace realpaver {

NcspNode::NcspNode(Scope scop, int depth)
      : scop_(scop),
        box_(nullptr),
        depth_(depth),
        index_(0),
        v_(),
        proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad depth of a NCSP node " << depth);
   ASSERT(!scop.isEmpty(), "Empty scope used to create a NCSP node");

   box_ = new DomainBox(scop);
}

NcspNode::NcspNode(std::unique_ptr<DomainBox> box, int depth)
      : scop_(box->scope()),
        box_(box.release()),
        depth_(depth),
        index_(0),
        v_(),
        proof_(Proof::Maybe)
{
   ASSERT(depth >= 0, "Bad depth of a NCSP node " << depth);
}

NcspNode::NcspNode(const NcspNode& node)
      : scop_(node.scop_),
        box_(nullptr),
        depth_(node.depth_),
        v_(node.v_)
{
   box_ = node.box_->clone();
}

NcspNode::~NcspNode()
{
   delete box_;
}

int NcspNode::depth() const
{
   return depth_;
}

void NcspNode::setDepth(int depth)
{
   ASSERT(depth >= 0, "Bad node depth " << depth);

   depth_ = depth;
}

int NcspNode::index() const
{
   return index_;
}

void NcspNode::setIndex(int id)
{
   index_ = id;
}

void NcspNode::incrementDepth()
{
   ++depth_;
}

DomainBox* NcspNode::box() const
{
   return box_;
}

Scope NcspNode::scope() const
{
   return scop_;
}

Variable NcspNode::splitVariable() const
{
   return v_;
}

void NcspNode::setSplitVariable(Variable v)
{
   ASSERT(scop_.contains(v), "Bad assignment of the last split variable");

   v_ = v;
}

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
   os << *node.box()
      << " proof: " << node.getProof();

   return os;
}

} // namespace
