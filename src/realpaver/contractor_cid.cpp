// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/common.hpp"
#include "realpaver/contractor_cid.hpp"
#include "realpaver/Exception.hpp"

namespace realpaver {

CidContractor::CidContractor(const SharedContractor& op, const Variable& v,
                             IntervalSlicer* slicer) :
   op_(op), v_(v), slicer_(slicer), bs_()
{
   ASSERT(op_.get() != nullptr, "");
   ASSERT(op->scope().contains(v), "");
   ASSERT(slicer_ != nullptr, "");

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

Proof CidContractor::contract(Box& B)
{
   slicer_->apply(B[v_.id()]);

   if (slicer_->size() == 1)
      return op_->contract(B);

   Box* init = B.clone();
   Proof proof = Proof::Empty, certif;

   for (auto it = slicer_->begin(); it != slicer_->end(); ++it)
   {
      Box* slice = init->clone();
      slice->set(v_, *it);
      certif = op_->contract(*slice);
   
      if (certif != Proof::Empty)
      {
         if (proof == Proof::Empty)
         {
            B.set(*slice, scope());
            proof = certif;
         }
         else
         {
            B.setHull(*slice, scope());
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
   os << "CID contractor on " << v_.name();
}

} // namespace
