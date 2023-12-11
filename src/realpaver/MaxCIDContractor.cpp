///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/MaxCIDContractor.hpp"

namespace realpaver {

MaxCIDContractor::MaxCIDContractor(SharedContractor op,
                                   std::unique_ptr<IntervalSlicer> slicer)
      : op_(nullptr)
{
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

Proof MaxCIDContractor::contract(IntervalBox& box)
{
   Variable v = selectMaxDom(box);
   op_->setVar(v);
   return op_->contract(box);
}

void MaxCIDContractor::print(std::ostream& os) const
{
   os << "Max CID contractor";
}

Variable MaxCIDContractor::selectMaxDom(const IntervalBox& box)
{
   Scope::const_iterator it = op_->scope().begin();
   Variable vmax = *it;
   double wmax = box.get(vmax).width();

   ++it;
   while (it != op_->scope().end())
   {
      Variable v = *it;
      double w = box.get(v).width();

      if (w > wmax)
      {
         vmax = v;
         wmax = w;
      }
      
      ++it;
   }

   return vmax;
}

} // namespace
