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
 * @file   ContractorVarCID.cpp
 * @brief  CID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVarCID.hpp"

namespace realpaver {

ContractorVarCID::ContractorVarCID(SharedContractor op, Variable v,
                                   std::unique_ptr<IntervalSlicer> slicer)
         : op_(op),
           v_(v),
           slicer_(std::move(slicer))
{
   ASSERT(op_.get() != nullptr, "No operator in a varCID contractor");
   ASSERT(op->scope().contains(v), 
          "Bad variable " << v << " in a varCID contractor");
   ASSERT(slicer != nullptr, "No slicer given");   
}

ContractorVarCID::ContractorVarCID(SharedContractor op, Variable v, size_t nsCID)
         : op_(op),
           v_(v),
           slicer_(nullptr)
{
   ASSERT(op_.get() != nullptr, "No operator in a varCID contractor");
   ASSERT(op->scope().contains(v), 
          "Bad variable " << v << " in a varCID contractor");
   ASSERT(nsCID > 1, "Bad number of slices in a varCID contractor: " << nsCID);

   slicer_ = std::make_unique<IntervalPartitionMaker>(nsCID);
}

Scope ContractorVarCID::scope() const
{
   return op_->scope();
}

Variable ContractorVarCID::getVar() const
{
   return v_;
}

void ContractorVarCID::setVar(Variable v)
{
   ASSERT(scope().contains(v),
          "Bad variable " << v << " in a varCID contractor");

   v_ = v;
}

Proof ContractorVarCID::contract(IntervalBox& B)
{
   slicer_->apply(B.get(v_));

   if (slicer_->nbSlices() == 1)
      return op_->contract(B);

   IntervalBox* init = B.clone();
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
            B.setOnScope(*slice, scope());
            proof = certif;
         }
         else
         {
            B.glueOnScope(*slice, scope());
            proof = std::min(proof, certif);
         }
      }
      delete slice;
   }

   delete init;
   return proof;
}

void ContractorVarCID::print(std::ostream& os) const
{
   os << "varCID contractor on " << v_.getName();
}

} // namespace
