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
 * @file   NodePropagator.cpp
 * @brief  Propagators of for search tree nodes
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/NodePropagator.hpp"

namespace realpaver {

NodePropagator::NodePropagator()
{
}

NodePropagator::~NodePropagator()
{
}

Proof NodePropagator::contractBox(const IntervalBox &B, DomainBox &box)
{
   for (const auto &v : box.scope())
   {
      Domain *dom = box.get(v);
      dom->contract(B.get(v));

      if (dom->isEmpty())
         return Proof::Empty;
   }
   return Proof::Maybe;
}

Proof NodePropagator::contract(Node &node, NodeContext &ctx)
{
   // creates an interval box from the domain box in the given node
   IntervalBox B(*node.box());

   // contractor of sub-class
   Proof proof = contractImpl(B, node.active());

   // contracts the domains using a reduced interval box from contractImpl
   // if any
   if (proof != Proof::Empty)
   {
      DomainBox *box = node.box();
      auto it = box->scope().begin();

      while ((proof != Proof::Empty) && (it != box->scope().end()))
      {
         Variable v = *it;
         Domain *dom = box->get(v);
         dom->contract(B.get(v));

         if (dom->isEmpty())
            proof = Proof::Empty;

         else
            ++it;
      }
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NodePropagatorHC4::NodePropagatorHC4(ContractorFactory &facto)
    : NodePropagator()
{
   hc4_ = facto.makeHC4();
}

Proof NodePropagatorHC4::contractImpl(IntervalBox &B, const Bitset &active)
{
   return hc4_->contract(B, active);
}

/*----------------------------------------------------------------------------*/

NodePropagatorBC4::NodePropagatorBC4(ContractorFactory &facto)
    : NodePropagator()
{
   bc4_ = facto.makeBC4();
}

Proof NodePropagatorBC4::contractImpl(IntervalBox &B, const Bitset &active)
{
   return bc4_->contract(B, active);
}

/*----------------------------------------------------------------------------*/

NodePropagatorAffine::NodePropagatorAffine(ContractorFactory &facto)
    : NodePropagator()
{
   ctc_ = facto.makeAffine();
}

Proof NodePropagatorAffine::contractImpl(IntervalBox &B, const Bitset &active)
{
   return ctc_->contract(B, active);
}

/*----------------------------------------------------------------------------*/

NodePropagatorNewton::NodePropagatorNewton(ContractorFactory &facto)
    : NodePropagator()
{
   nwt_ = facto.makeNewton();
}

Proof NodePropagatorNewton::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   if (nwt_ != nullptr)
      proof = nwt_->contract(B, active);
   return proof;
}

/*----------------------------------------------------------------------------*/

NodePropagatorPolytope::NodePropagatorPolytope(ContractorFactory &facto)
    : NodePropagator()
{
   poly_ = facto.makePolytope();
}

Proof NodePropagatorPolytope::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   if (poly_ != nullptr)
      proof = poly_->contract(B, active);
   return proof;
}

/*----------------------------------------------------------------------------*/

NodePropagatorACID::NodePropagatorACID(ContractorFactory &facto)
    : NodePropagator()
{
   hc4_ = facto.makeHC4();
   acid_ = facto.makeACID();
}

Proof NodePropagatorACID::contractImpl(IntervalBox &B, const Bitset &active)
{
   // applies HC4
   Proof proof = hc4_->contract(B, active);
   if (proof == Proof::Empty)
      return proof;

   // applies ACID
   if (acid_ != nullptr)
      proof = acid_->contract(B, active);

   return proof;
}

/*----------------------------------------------------------------------------*/

NodePropagatorList::NodePropagatorList()
    : NodePropagator()
    , v_()
{
}

void NodePropagatorList::pushBack(NodePropagAlgo alg, ContractorFactory &facto)
{
   SharedNodePropagator op;
   switch (alg)
   {
   case NodePropagAlgo::HC4:
      op = std::make_shared<NodePropagatorHC4>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::BC4:
      op = std::make_shared<NodePropagatorBC4>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::AFFINE:
      op = std::make_shared<NodePropagatorAffine>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::ACID:
      op = std::make_shared<NodePropagatorACID>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::Polytope:
      op = std::make_shared<NodePropagatorPolytope>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::Newton:
      op = std::make_shared<NodePropagatorNewton>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;
   }
}

size_t NodePropagatorList::size() const
{
   return v_.size();
}

Proof NodePropagatorList::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   for (int i = 0; i < v_.size(); ++i)
   {
      proof = v_[i]->contractImpl(B, active);
      if (proof == Proof::Empty)
         return proof;
   }
   return proof;
}

} // namespace realpaver
