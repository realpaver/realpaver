///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Contractor3B.hpp"

namespace realpaver {

Contractor3B::Contractor3B(SharedContractor op, Variable v,
                             std::unique_ptr<IntervalSlicer> slicer)
         : op_(op),
           v_(v),
           slicer_(std::move(slicer))
{
   ASSERT(op_.get() != nullptr, "No operator in a 3B contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v <<
                                   " in a 3B contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a 3B contractor");
}

Contractor3B::Contractor3B(SharedContractor op,
                             std::unique_ptr<IntervalSlicer> slicer)
      : op_(op),
        v_(),
        slicer_(std::move(slicer))
{
   ASSERT(op_.get() != nullptr, "No operator in a 3B contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a 3B contractor");
}

Scope Contractor3B::scope() const
{
   return op_->scope();
}

Variable Contractor3B::getVar() const
{
   return v_;
}

void Contractor3B::setVar(Variable v)
{
   ASSERT(scope().contains(v), "Bad variable " << v << " in a 3B contractor");

   v_ = v;
}

Proof Contractor3B::contract(IntervalBox& box)
{
   Interval dom = box.get(v_);
   slicer_->apply(dom);
   size_t nbs = slicer_->nbSlices();

   if (nbs == 1)
      return op_->contract(box);

   // left to right
   size_t nbl = 0;               // number of left inconsistent facets
   auto it = slicer_->begin();   // iterator on the first slice
   bool iter = true;
   Interval lslice;

   while (iter)
   {
      IntervalBox* facet = box.clone();
      lslice = *it;
      facet->set(v_, lslice);

      Proof certif = op_->contract(*facet);
      if (certif == Proof::Empty)
      {
         ++nbl;
         ++it;
         if (nbl == nbs) iter = false;
      }
      else
         iter = false;

      delete facet;
   }

   if (nbl == nbs) return Proof::Empty;

   // right to left
   size_t nbr = 0;               // number of right inconsistent facets
   auto jt = slicer_->rbegin();
   iter = true;
   Interval rslice;

   while (iter)
   {
      IntervalBox* facet = box.clone();
      rslice = *jt;
      facet->set(v_, rslice);

      Proof certif = op_->contract(*facet);
      if (certif == Proof::Empty)
      {
         ++nbr;
         ++jt;
         if (nbr == nbs - nbl - 1)
         {
            iter = false;
            rslice = *jt;
         }
      }
      else
         iter = false;

      delete facet;
   }

   box.set(v_, Interval(lslice.left(), rslice.right()));

   return Proof::Maybe;
}

void Contractor3B::print(std::ostream& os) const
{
   os << "CID contractor on " << v_.getName();
}

} // namespace
