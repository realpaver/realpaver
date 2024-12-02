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
      : B_(nullptr)
{}

CSPPropagator::~CSPPropagator()
{
   if (B_ != nullptr) delete B_;
}

Proof CSPPropagator::contractBox(const IntervalBox& B, DomainBox& box)
{
   for (const auto& v : box.scope())
   {
      Domain* dom = box.get(v);
      dom->contract(B.get(v));

      if (dom->isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

Proof CSPPropagator::contract(CSPNode& node, CSPContext& ctx)
{
   if (B_ != nullptr)
   {
      delete B_;
      B_ = nullptr;
   }

   // contractor of sub-class
   Proof proof = contractImpl(node, ctx);

   // contracts the domains using a reduced interval box from contractImpl
   // if any
   if ((proof != Proof::Empty) && (B_ != nullptr))
   {
      DomainBox* box = node.box();
      auto it = box->scope().begin();

      while ((proof != Proof::Empty) && (it != box->scope().end()))
      {
         Variable v = *it;
         Domain* dom = box->get(v);
         dom->contract(B_->get(v));

         if (dom->isEmpty())
            proof = Proof::Empty;

         else
            ++it;
      }
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorHC4::CSPPropagatorHC4(ContractorFactory& facto)
      : CSPPropagator()
{
   op_ = facto.makeHC4();
}

Proof CSPPropagatorHC4::contractImpl(CSPNode& node, CSPContext& ctx)
{
   B_ = new IntervalBox(*node.box());
   return op_->contract(*B_);
}

/*----------------------------------------------------------------------------*/

CSPPropagatorBC4::CSPPropagatorBC4(ContractorFactory& facto)
      : CSPPropagator()
{
   op_ = facto.makeBC4();
}

Proof CSPPropagatorBC4::contractImpl(CSPNode& node, CSPContext& ctx)
{
   B_ = new IntervalBox(*node.box());
   return op_->contract(*B_);
}

/*----------------------------------------------------------------------------*/

CSPPropagatorNewton::CSPPropagatorNewton(ContractorFactory& facto)
      : CSPPropagator()
{
   op_ = facto.makeNewton();
}

Proof CSPPropagatorNewton::contractImpl(CSPNode& node, CSPContext& ctx)
{
   Proof proof = Proof::Maybe;

   if (op_ != nullptr)
   {
      B_ = new IntervalBox(*node.box());
      proof = op_->contract(*B_);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorACID::CSPPropagatorACID(ContractorFactory& facto)
      : CSPPropagator()
{
   hc4_ = facto.makeHC4();
   op_ = facto.makeACID();
}

Proof CSPPropagatorACID::contractImpl(CSPNode& node, CSPContext& ctx)
{
   B_ = new IntervalBox(*node.box());

   // applies HC4
   Proof proof = hc4_->contract(*B_);
   if (proof == Proof::Empty)
      return proof;

   // applies ACID
   return op_->contract(*B_);
}

/*----------------------------------------------------------------------------*/

CSPPropagatorHC4Newton::CSPPropagatorHC4Newton(ContractorFactory& facto)
      : CSPPropagator(),
        hc4_(facto),
        newton_(facto)
{}

Proof CSPPropagatorHC4Newton::contractImpl(CSPNode& node, CSPContext& ctx)
{
   Proof proof = hc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorBC4Newton::CSPPropagatorBC4Newton(ContractorFactory& facto)
      : CSPPropagator(),
        bc4_(facto),
        newton_(facto)
{}

Proof CSPPropagatorBC4Newton::contractImpl(CSPNode& node, CSPContext& ctx)
{
   Proof proof = bc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

CSPPropagatorACIDNewton::CSPPropagatorACIDNewton(ContractorFactory& facto)
      : CSPPropagator(),
        acid_(facto),
        newton_(facto)
{}

Proof CSPPropagatorACIDNewton::contractImpl(CSPNode& node, CSPContext& ctx)
{
   Proof proof = acid_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

} // namespace
