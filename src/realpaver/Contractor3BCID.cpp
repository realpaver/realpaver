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
      : op_(op),
        v_(v),
        slicer3B_(std::move(slicer3B)),
        slicerCID_(std::move(slicerCID))
{
   ASSERT(op_.get() != nullptr, "No operator in a 3BCID contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v <<
                                   " in a 3BCID contractor");
   ASSERT(slicer3B_ != nullptr, "No slicer in a 3BCID contractor");
   ASSERT(slicerCID_ != nullptr, "No slicer in a 3BCID contractor");
}

Contractor3BCID::Contractor3BCID(SharedContractor op,
                                 std::unique_ptr<IntervalSlicer> slicer3B,
                                 std::unique_ptr<IntervalSlicer> slicerCID)
      : op_(op),
        v_(),
        slicer3B_(std::move(slicer3B)),
        slicerCID_(std::move(slicerCID))
{
   ASSERT(op_.get() != nullptr, "No operator in a 3BCID contractor");
   ASSERT(slicer3B_ != nullptr, "No slicer in a 3BCID contractor");
   ASSERT(slicerCID_ != nullptr, "No slicer in a 3BCID contractor");
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
   return op_->scope();
}

Proof Contractor3BCID::contract(IntervalBox& box)
{
   // TODO
}

void Contractor3BCID::print(std::ostream& os) const
{
   os << "3BCID contractor on " << v_.getName();
}
   
} // namespace
