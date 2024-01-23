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
#include "realpaver/PropagatorACID.hpp"

namespace realpaver {

PropagatorACID::PropagatorACID(std::shared_ptr<IntervalSmearSumRel> ssr,
                               SharedContractor op,
                               size_t nbs)
      : Contractor(),
        ssr_(ssr),
        op_(op)
{
   ASSERT(ssr_ != nullptr, "No smear sum rel object in an ACID propagator");
   ASSERT(op_ != nullptr, "No operator in an ACID propagator");
   ASSERT(op->scope().contains(ssr->scope()),
          "Bad scopes in an ACID propagator");

   setNbSlices(nbs);
   setNbCID(ssr->nbVars());
}

Scope PropagatorACID::scope() const
{
   return ssr_->scope();
}

size_t PropagatorACID::nbSlices() const
{
   return nbs_;
}

void PropagatorACID::setNbSlices(size_t nbs)
{
   ASSERT(nbs > 1, "Bad number of slices in ACID: " << nbs);
   nbs_ = nbs;
}

size_t PropagatorACID::nbCID() const
{
   return nbcid_;
}

void PropagatorACID::setNbCID(size_t nbcid)
{
   ASSERT(nbcid > 0, "Bad number of CID contractors applied in ACID: " << nbcid);
   nbcid_ = nbcid;
}

Proof PropagatorACID::contract(IntervalBox& B)
{
   ssr_->calculate(B);
   ssr_->sort();

   for (size_t i=0; i<nbcid_; ++i)
   {
      Variable v = ssr_->getVar(i);
      ContractorCID cid(op_, v, nbs_);

      Proof proof = cid.contract(B);
      if (proof == Proof::Empty)
         return proof;
   }
   return Proof::Maybe;
}

void PropagatorACID::print(std::ostream& os) const
{
   os << "ACID propagator";
}

} // namespace
