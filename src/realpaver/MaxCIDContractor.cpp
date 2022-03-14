///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/MaxCIDContractor.hpp"

namespace realpaver {

MaxCIDContractor::MaxCIDContractor(const SharedContractor& op,
                                   Selector* selector,
                                   IntervalSlicer* slicer)
      : op_(nullptr),
        selector_(selector)
{
   ASSERT(selector != nullptr, "No selector in a MaxCID contractor");

   op_ = new CIDContractor(op, slicer);
}

MaxCIDContractor::~MaxCIDContractor()
{
   if (op_ != nullptr) delete op_;
   if (selector_ != nullptr) delete selector_;
}

bool MaxCIDContractor::dependsOn(const Bitset& bs) const
{
   return op_->dependsOn(bs);
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
