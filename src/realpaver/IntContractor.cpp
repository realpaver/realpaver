///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntContractor.hpp"

namespace realpaver {

IntContractor::IntContractor()
      : s_()
{}

IntContractor::IntContractor(Variable v)
      : s_()
{
   insertVar(v);
}

IntContractor::IntContractor(const std::initializer_list<Variable>& l)
      : s_()
{
   for (auto v : l)
      insertVar(v);
}

size_t IntContractor::nbVars() const
{
   return s_.size();
}

void IntContractor::insertVar(Variable v)
{
   ASSERT(v.isInteger(),
          "IntContractor applied to a continuous variable " << v.getName());

   s_.insert(v);
}

Proof IntContractor::contract(IntervalRegion& reg)
{
   for (auto v : s_)
   {
      Interval rnd = round(reg.get(v));
      reg.set(v, rnd);

      if (rnd.isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

void IntContractor::print(std::ostream& os) const
{
   os << "integral: ";
   for (auto v : s_) os << v.getName() << " ";
}

Scope IntContractor::scope() const
{
   return s_;
}

} // namespace
