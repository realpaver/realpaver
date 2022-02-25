///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/CIDContractor.hpp"

namespace realpaver {

CidContractor::CidContractor(const SharedContractor& op, const Variable& v,
                             IntervalSlicer* slicer)
         : op_(op),
           v_(v),
           slicer_(slicer),
           bs_()
{
   ASSERT(op_.get() != nullptr, "No operator in a CID contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v <<
                                   " in a CID contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a CID contractor");

   bs_ = op->scope().toBitset();
}

CidContractor::~CidContractor()
{
   delete slicer_;
}

bool CidContractor::dependsOn(const Bitset& bs) const
{
   return bs_.overlaps(bs);
}

Scope CidContractor::scope() const
{
   return op_->scope();
}

Proof CidContractor::contract(IntervalVector& X)
{
   slicer_->apply(X[v_.getId()]);

   if (slicer_->nbSlices() == 1)
      return op_->contract(X);

   IntervalVector* init = X.clone();
   Proof proof = Proof::Empty, certif;

   for (auto it = slicer_->begin(); it != slicer_->end(); ++it)
   {
      IntervalVector* slice = init->clone();
      slice->set(v_.getId(), *it);
      certif = op_->contract(*slice);
   
      if (certif != Proof::Empty)
      {
         if (proof == Proof::Empty)
         {
            X.setOnScope(*slice, scope());
            proof = certif;
         }
         else
         {
            X.hullAssignOnScope(*slice, scope());
            proof = std::min(proof,certif);
         }

      }
      delete slice;
   }

   delete init;
   return proof;
}

void CidContractor::print(std::ostream& os) const
{
   os << "CID contractor on " << v_.getName();
}

} // namespace