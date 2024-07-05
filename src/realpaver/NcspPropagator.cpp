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
 * @file   NcspPropagator.cpp
 * @brief  Propagators of NCSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/
#include <algorithm>
#include "realpaver/Logger.hpp"
#include "realpaver/NcspPropagator.hpp"

namespace realpaver {

NcspPropagator::NcspPropagator()
      : B_(nullptr)
{}

NcspPropagator::~NcspPropagator()
{
   if (B_ != nullptr) delete B_;
}

Proof NcspPropagator::contractBox(const IntervalBox& B, DomainBox& box)
{
   for (const auto& v : box.scope())
   {
      Domain* dom = box.get(v);
      dom->contract(B.get(v));

      if (dom->isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

Proof NcspPropagator::contract(NcspNode& node, NcspContext& ctx)
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

NcspHC4::NcspHC4(ContractorFactory& facto)
      : NcspPropagator()
{
   op_ = facto.makeHC4();
}

Proof NcspHC4::contractImpl(NcspNode& node, NcspContext& ctx)
{
   B_ = new IntervalBox(*node.box());
   return op_->contract(*B_);
}

/*----------------------------------------------------------------------------*/

NcspBC4::NcspBC4(ContractorFactory& facto)
      : NcspPropagator()
{
   op_ = facto.makeBC4();
}

Proof NcspBC4::contractImpl(NcspNode& node, NcspContext& ctx)
{
   B_ = new IntervalBox(*node.box());
   return op_->contract(*B_);
}

/*----------------------------------------------------------------------------*/

NcspNewton::NcspNewton(ContractorFactory& facto)
      : NcspPropagator()
{
   op_ = facto.makeNewton();
}

Proof NcspNewton::contractImpl(NcspNode& node, NcspContext& ctx)
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

NcspPolytope::NcspPolytope(ContractorFactory& facto)
      : NcspPropagator()
{
   op_ = facto.makePolytope();
}

Proof NcspPolytope::contractImpl(NcspNode& node, NcspContext& ctx)
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

NcspACID::NcspACID(ContractorFactory& facto)
      : NcspPropagator()
{
   hc4_ = facto.makeHC4();
   op_ = facto.makeACID();
}

Proof NcspACID::contractImpl(NcspNode& node, NcspContext& ctx)
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

NcspHC4Newton::NcspHC4Newton(ContractorFactory& facto)
      : NcspPropagator(),
        hc4_(facto),
        newton_(facto)
{}

Proof NcspHC4Newton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = hc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspBC4Newton::NcspBC4Newton(ContractorFactory& facto)
      : NcspPropagator(),
        bc4_(facto),
        newton_(facto)
{}

Proof NcspBC4Newton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = bc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspACIDNewton::NcspACIDNewton(ContractorFactory& facto)
      : NcspPropagator(),
        acid_(facto),
        newton_(facto)
{}

Proof NcspACIDNewton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = acid_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspHC4Polytope::NcspHC4Polytope(ContractorFactory& facto)
      : NcspPropagator(),
        hc4_(facto),
        poly_(facto)
{}

Proof NcspHC4Polytope::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = hc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = poly_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspBC4Polytope::NcspBC4Polytope(ContractorFactory& facto)
      : NcspPropagator(),
        bc4_(facto),
        poly_(facto)
{}

Proof NcspBC4Polytope::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = bc4_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = poly_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspACIDPolytope::NcspACIDPolytope(ContractorFactory& facto)
      : NcspPropagator(),
        acid_(facto),
        poly_(facto)
{}

Proof NcspACIDPolytope::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = acid_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = poly_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspHC4PolytopeNewton::NcspHC4PolytopeNewton(ContractorFactory& facto)
      : NcspPropagator(),
        hc4poly_(facto),
        newton_(facto)
{}

Proof NcspHC4PolytopeNewton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = hc4poly_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspBC4PolytopeNewton::NcspBC4PolytopeNewton(ContractorFactory& facto)
      : NcspPropagator(),
        bc4poly_(facto),
        newton_(facto)
{}

Proof NcspBC4PolytopeNewton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = bc4poly_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

/*----------------------------------------------------------------------------*/

NcspACIDPolytopeNewton::NcspACIDPolytopeNewton(ContractorFactory& facto)
      : NcspPropagator(),
        acidpoly_(facto),
        newton_(facto)
{}

Proof NcspACIDPolytopeNewton::contractImpl(NcspNode& node, NcspContext& ctx)
{
   Proof proof = acidpoly_.contract(node, ctx);

   if (proof != Proof::Empty)
   {
      proof = newton_.contract(node, ctx);
   }

   return proof;
}

} // namespace
