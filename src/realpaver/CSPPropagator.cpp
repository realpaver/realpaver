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
 * @file   CSPPropagator.cpp
 * @brief  Propagators of CSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/CSPPropagator.hpp"

namespace realpaver {

CSPPropagator::CSPPropagator()
{
}

CSPPropagator::~CSPPropagator()
{
}

Proof CSPPropagator::contractBox(const IntervalBox &B, DomainBox &box)
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

Proof CSPPropagator::contract(CSPNode &node, CSPContext &ctx)
{
   // creates an interval box from the domain box in the given node
   IntervalBox B(*node.box());

   // contractor of sub-class
   Proof proof = contractImpl(B);

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

CSPPropagatorHC4::CSPPropagatorHC4(ContractorFactory &facto)
    : CSPPropagator()
{
   hc4_ = facto.makeHC4();
}

Proof CSPPropagatorHC4::contractImpl(IntervalBox &B)
{
   return hc4_->contract(B);
}

/*----------------------------------------------------------------------------*/

CSPPropagatorBC4::CSPPropagatorBC4(ContractorFactory &facto)
    : CSPPropagator()
{
   bc4_ = facto.makeBC4();
}

Proof CSPPropagatorBC4::contractImpl(IntervalBox &B)
{
   return bc4_->contract(B);
}

/*----------------------------------------------------------------------------*/

CSPPropagatorNewton::CSPPropagatorNewton(ContractorFactory &facto)
    : CSPPropagator()
{
   nwt_ = facto.makeNewton();
}

Proof CSPPropagatorNewton::contractImpl(IntervalBox &B)
{
   Proof proof = Proof::Maybe;
   if (nwt_ != nullptr)
      proof = nwt_->contract(B);
   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorPolytope::CSPPropagatorPolytope(ContractorFactory &facto)
    : CSPPropagator()
{
   poly_ = facto.makePolytope();
}

Proof CSPPropagatorPolytope::contractImpl(IntervalBox &B)
{
   Proof proof = Proof::Maybe;
   if (poly_ != nullptr)
      proof = poly_->contract(B);
   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorACID::CSPPropagatorACID(ContractorFactory &facto)
    : CSPPropagator()
{
   hc4_ = facto.makeHC4();
   acid_ = facto.makeACID();
}

Proof CSPPropagatorACID::contractImpl(IntervalBox &B)
{
   // applies HC4
   Proof proof = hc4_->contract(B);
   if (proof == Proof::Empty)
      return proof;

   // applies ACID
   if (acid_ != nullptr)
      proof = acid_->contract(B);

   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorList::CSPPropagatorList()
    : CSPPropagator()
    , v_()
{
}

void CSPPropagatorList::pushBack(CSPPropagAlgo alg, ContractorFactory &facto)
{
   SharedCSPPropagator op;
   switch (alg)
   {
   case CSPPropagAlgo::HC4:
      op = std::make_shared<CSPPropagatorHC4>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case CSPPropagAlgo::BC4:
      op = std::make_shared<CSPPropagatorBC4>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case CSPPropagAlgo::ACID:
      op = std::make_shared<CSPPropagatorACID>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case CSPPropagAlgo::Polytope:
      op = std::make_shared<CSPPropagatorPolytope>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case CSPPropagAlgo::Newton:
      op = std::make_shared<CSPPropagatorNewton>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;
   }
}

size_t CSPPropagatorList::size() const
{
   return v_.size();
}

Proof CSPPropagatorList::contractImpl(IntervalBox &B)
{
   Proof proof = Proof::Maybe;
   for (int i = 0; i < v_.size(); ++i)
   {
      proof = v_[i]->contractImpl(B);
      if (proof == Proof::Empty)
         return proof;
   }
   return proof;
}

} // namespace realpaver
