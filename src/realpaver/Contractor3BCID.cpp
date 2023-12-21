///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Contractor3BCID.hpp"

namespace realpaver {


Contractor3BCID::Contractor3BCID(SharedContractor op, Variable v,
                                 std::unique_ptr<IntervalSlicer> slicer3B,
                                 std::unique_ptr<IntervalSlicer> slicerCID)
      : v_(v),
        ctc3B_(nullptr),
        ctcCID_(nullptr)
{
   ctc3B_ = new Contractor3B(op, v, std::move(slicer3B));
   ctcCID_ = new ContractorCID(op, v, std::move(slicerCID));
}

Contractor3BCID::Contractor3BCID(SharedContractor op, Variable v,
                                 size_t n3B, size_t nCID)
      : v_(v),
        ctc3B_(nullptr),
        ctcCID_(nullptr)
{
   ctc3B_ = new Contractor3B(op, v, n3B);
   ctcCID_ = new ContractorCID(op, v, nCID);
}

Contractor3BCID::~Contractor3BCID()
{
   delete ctc3B_;
   delete ctcCID_;
}

Variable Contractor3BCID::getVar() const
{
   return v_;
}

void Contractor3BCID::setVar(Variable v)
{
   ASSERT(scope().contains(v),
          "Bad variable " << v << " in a 3BCID contractor");

   v_ = v;
}

Scope Contractor3BCID::scope() const
{
   return ctc3B_->scope();
}

Proof Contractor3BCID::contract(IntervalBox& B)
{
   Interval dom = B.get(v_);

   Proof proof = ctc3B_->contract(B);

   if (proof == Proof::Empty)
      return Proof::Empty;

   if (dom.isSetEq(B.get(v_)))
      return Proof::Maybe;

   return ctcCID_->contract(B);
}

void Contractor3BCID::print(std::ostream& os) const
{
   os << "3BCID contractor on " << v_.getName();
}
   
} // namespace
