///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ContractorList.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/NcspPropagator.hpp"

namespace realpaver {

NcspPropagator::~NcspPropagator()
{}

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

///////////////////////////////////////////////////////////////////////////////

NcspHC4::NcspHC4(ContractorFactory& facto)
      : NcspPropagator()
{
   // HC4
   op_ = facto.makeHC4();

   // Contractors for disconnected variable domains
   SharedContractor dop = facto.makeContractorDomain();
   if (dop != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(dop);
      op_ = std::make_shared<ContractorList>(pool);
   }
}

Proof NcspHC4::contract(NcspNode& node, NcspContext& ctx)
{
   DomainBox* box = node.box();
   IntervalBox B(*box);

   Proof proof = op_->contract(B);

   return (proof == Proof::Empty) ? Proof::Empty :
                                    contractBox(B, *box);
}

///////////////////////////////////////////////////////////////////////////////

NcspHC4Newton::NcspHC4Newton(ContractorFactory& facto)
      : NcspPropagator()
{
   // HC4
   op_ = facto.makeHC4();

   // Interval Newton
   SharedContractor newton = facto.makeIntervalNewton();
   if (newton != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(newton);
      op_ = std::make_shared<ContractorList>(pool);
   }

   // Contractors for disconnected variable domains
   SharedContractor dop = facto.makeContractorDomain();
   if (dop != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(op_);
      pool->push(dop);
      op_ = std::make_shared<ContractorList>(pool);
   }
}

Proof NcspHC4Newton::contract(NcspNode& node, NcspContext& ctx)
{
   DomainBox* box = node.box();
   IntervalBox B(*box);

   Proof proof = op_->contract(B);

   return (proof == Proof::Empty) ? Proof::Empty :
                                    contractBox(B, *box);
}

///////////////////////////////////////////////////////////////////////////////

NcspACID::NcspACID(ContractorFactory& facto)
      : NcspPropagator()
{
   hc4_ = facto.makeHC4();
   acid_ = facto.makeACID();
}

Proof NcspACID::contract(NcspNode& node, NcspContext& ctx)
{
   DomainBox* box = node.box();
   IntervalBox B(*box);

   if (acid_ == nullptr)
   {
      Proof proof = hc4_->contract(B);
      return (proof == Proof::Empty) ? Proof::Empty :
                                       contractBox(B, *box);
   }
   else
   {
      // learning or exploitation phase?
      int depth = node.depth();

      // TODO

      bool learning = true;

      if (learning)
      {
         std::shared_ptr<IntervalSmearSumRel>
            ssr = acid_->getIntervalSmearSumRel();

         ssr->calculate(B);
         ssr->sort();

         // TODO



         // TODO
         // il faut enregistrer le nombre de slices pour le noeud
      }
      else
      {
         // TODO
         // on recupere le nombre de slices du noeud  parent
         // acid_->setNbCID(nbs);
         
         
         Proof proof = acid_->contract(B);
         return (proof == Proof::Empty) ? Proof::Empty :
                                          contractBox(B, *box);
      }
   }
}

} // namespace
