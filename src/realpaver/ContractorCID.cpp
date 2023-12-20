///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorCID.hpp"

namespace realpaver {

ContractorCID::ContractorCID(SharedContractor op, Variable v,
                             std::unique_ptr<IntervalSlicer> slicer)
         : op_(op),
           v_(v),
           slicer_(std::move(slicer))
{
   ASSERT(op_.get() != nullptr, "No operator in a CID contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v <<
                                   " in a CID contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a CID contractor");
}

ContractorCID::ContractorCID(SharedContractor op,
                             std::unique_ptr<IntervalSlicer> slicer)
      : op_(op),
        v_(),
        slicer_(std::move(slicer))
{
   ASSERT(op_.get() != nullptr, "No operator in a CID contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a CID contractor");
}

Scope ContractorCID::scope() const
{
   return op_->scope();
}

Variable ContractorCID::getVar() const
{
   return v_;
}

void ContractorCID::setVar(Variable v)
{
   ASSERT(scope().contains(v), "Bad variable " << v << " in a CID contractor");

   v_ = v;
}

Proof ContractorCID::contract(IntervalBox& box)
{
   slicer_->apply(box.get(v_));

   if (slicer_->nbSlices() == 1)
      return op_->contract(box);

   IntervalBox* init = box.clone();
   Proof proof = Proof::Empty, certif;

   for (auto it = slicer_->begin(); it != slicer_->end(); ++it)
   {
      IntervalBox* slice = init->clone();
      slice->set(v_, *it);
      certif = op_->contract(*slice);
   
      if (certif != Proof::Empty)
      {
         if (proof == Proof::Empty)
         {
            box.setOnScope(*slice, scope());
            proof = certif;
         }
         else
         {
            box.glueOnScope(*slice, scope());
            proof = std::min(proof, certif);
         }
      }
      delete slice;
   }

   delete init;
   return proof;
}

void ContractorCID::print(std::ostream& os) const
{
   os << "CID contractor on " << v_.getName();
}

} // namespace
