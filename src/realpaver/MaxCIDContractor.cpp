///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/MaxCIDContractor.hpp"

namespace realpaver {

MaxCIDContractor::MaxCIDContractor(SharedContractor op,
                                   std::unique_ptr<VariableSelector> selector,
                                   std::unique_ptr<IntervalSlicer> slicer)
      : op_(nullptr),
        selector_(std::move(selector))
{
   ASSERT(selector_ != nullptr, "No selector in a MaxCID contractor");

   op_ = new CIDContractor(op, std::move(slicer));
}

MaxCIDContractor::~MaxCIDContractor()
{
   if (op_ != nullptr) delete op_;
}

Scope MaxCIDContractor::scope() const
{
   return op_->scope();
}

Proof MaxCIDContractor::contract(IntervalRegion& reg)
{
   std::pair<bool, Variable> sel = selector_->selectVar(reg);

   Proof proof = Proof::Maybe;

   if (sel.first)
   {
      op_->setVar(sel.second);
      proof = op_->contract(reg);
   }

   return proof;
}

void MaxCIDContractor::print(std::ostream& os) const
{
   os << "Max CID contractor";
}

} // namespace
