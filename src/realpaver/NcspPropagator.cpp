///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "realpaver/ContractorList.hpp"
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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

NcspNewton::NcspNewton(ContractorFactory& facto)
      : NcspPropagator()
{
   op_ = facto.makeIntervalNewton();
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

///////////////////////////////////////////////////////////////////////////////

NcspACID::NcspACID(ContractorFactory& facto)
      : NcspPropagator(),
        scop_(),
        var3BCID_(),
        kVarCID_()
{
   hc4_ = facto.makeHC4();
   scop_ = hc4_->scope();
   n_ = hc4_->scope().size();
   numVarCID_ = n_;

   acid_ = facto.makeACID();
   THROW_IF(acid_ == nullptr, "Unable to make an ACID propagator");

   cycleLength_ = facto.getEnv()->getParam()->getIntParam("CYCLE_LENGTH_ACID");
   learnLength_ = facto.getEnv()->getParam()->getIntParam("LEARN_LENGTH_ACID");
   ctRatio_ = facto.getEnv()->getParam()->getDblParam("CT_RATIO_ACID");

   int ns3B = facto.getEnv()->getParam()->getIntParam("NB_SLICE_3B");
   int nsCID = facto.getEnv()->getParam()->getIntParam("NB_SLICE_CID");

   for (size_t i=0; i<n_; ++i)
   {
      Variable v = scop_.var(i);
      var3BCID_.push_back(new ContractorVar3BCID(hc4_, v, ns3B, nsCID));
   }

   kVarCID_.insert(kVarCID_.begin(), learnLength_, 0);
   call_ = 0;
}

NcspACID::~NcspACID()
{
   for (auto& op : var3BCID_)
      delete op;
}

int NcspACID::lastSignificantGain(std::vector<double>& ctcGains,
                                  double ctRatio)
{
   int i = ctcGains.size()-1;

   while ((i>=0) && (ctcGains[i]<=ctRatio))
      --i;

   return i+1;
}

size_t NcspACID::avgNbVarCID(std::vector<size_t>& v)
{   
   double s = 0;
   for (const size_t& n : v)
      s += n;

   double a = s / v.size(),
          f = std::floor(a),
          d = a - f,
          res = (d <= 0.5) ? f : f+1;

   return (int)res;
}

Proof NcspACID::contractImpl(NcspNode& node, NcspContext& ctx)
{
   DomainBox* box = node.box();
   B_ = new IntervalBox(*box);

   Proof proof = Proof::Maybe;

   // sorts the variables according to their impact
   std::shared_ptr<IntervalSmearSumRel>
      ssr = acid_->getIntervalSmearSumRel();

   ssr->calculate(*B_);
   ssr->sort();

   size_t mcall = call_ % cycleLength_;

   if (mcall < learnLength_)
   {
      // learning phase
      int nVarCID = std::max(2, 2*numVarCID_);
      std::vector<double> ctcGains(nVarCID);

      int i = 0;
      while ((proof != Proof::Empty) && (i<nVarCID))
      {
         IntervalBox save(*B_);

         size_t j = i % n_;
         Variable v = ssr->getVar(j);
         size_t k = scop_.index(v);

         proof = var3BCID_[k]->contract(*B_);

         if (proof != Proof::Empty)
         {
            ctcGains[i] = B_->gainRatio(save);
            ++i;
         }
      }

      // number of contractors that have been applied till a significant gain
      if (proof == Proof::Empty)
      {
         kVarCID_[mcall] = i+1;
      }
      else
      {
         kVarCID_[mcall] = lastSignificantGain(ctcGains, ctRatio_);
      }

      // end of learning phase
      if (mcall == learnLength_-1)
      {
         numVarCID_ = avgNbVarCID(kVarCID_);
      }
   }
   else
   {
      // exploitation phase
      if (numVarCID_ == 0)
      {
         proof = hc4_->contract(*B_);
      }
      else
      {
         size_t i=0;
         while ((proof != Proof::Empty) && (i<numVarCID_))
         {
            size_t j = i % n_;
            Variable v = ssr->getVar(j);
            size_t k = scop_.index(v);

            proof = var3BCID_[k]->contract(*B_);
            ++i;
         }
      }
   }

   ++call_;

   return proof;
}


///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////
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

} // namespace
