///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVarCID.hpp"
#include "realpaver/ContractorACID.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorACID::ContractorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                               SharedContractor op,
                               size_t nbs)
      : Contractor(),
        ssr_(ssr),
        op_(op)
{
   ASSERT(ssr_ != nullptr, "No smear sum rel object in an ACID contractor");
   ASSERT(op_ != nullptr, "No operator in an ACID contractor");
   ASSERT(op->scope().contains(ssr->scope()),
          "Bad scopes in an ACID contractor");

   setNbSlices(nbs);
   setNbVarCID(ssr->nbVars());
}

Scope ContractorACID::scope() const
{
   return ssr_->scope();
}

size_t ContractorACID::nbSlices() const
{
   return nbs_;
}

void ContractorACID::setNbSlices(size_t nbs)
{
   ASSERT(nbs > 1, "Bad number of slices in ACID: " << nbs);
   nbs_ = nbs;
}

size_t ContractorACID::nbVarCID() const
{
   return nbVarCID_;
}

void ContractorACID::setNbVarCID(size_t n)
{
   nbVarCID_ = n;
}

SharedContractor ContractorACID::getContractor() const
{
   return op_;
}

std::shared_ptr<IntervalSmearSumRel>
ContractorACID::getIntervalSmearSumRel() const
{
   return ssr_;
}

Proof ContractorACID::contract(IntervalBox& B)
{
   LOG_INTER("ACID on " << B);

   if (nbVarCID_ == 0)
   {
      return op_->contract(B);
   }

   ssr_->calculate(B);
   ssr_->sort();

   for (size_t i=0; i<nbVarCID_; ++i)
   {
      Variable v = ssr_->getVar(i);
      ContractorVarCID cid(op_, v, nbs_);

      Proof proof = cid.contract(B);
      if (proof == Proof::Empty)
         return proof;
   }
   return Proof::Maybe;
}

void ContractorACID::print(std::ostream& os) const
{
   os << "ACID propagator";
}

} // namespace
