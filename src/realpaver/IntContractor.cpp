///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntContractor.hpp"

namespace realpaver {

IntContractor::IntContractor()
      : s_(),
        b_()
{}

IntContractor::IntContractor(Variable v)
      : s_(),
        b_()
{
   insert(v);
}

IntContractor::IntContractor(const std::initializer_list<Variable>& l)
      : s_(),
        b_()
{
   for (auto v : l)
      insert(v);
}

void IntContractor::insert(Variable v)
{
   ASSERT(v.isDiscrete(),
          "IntContractor applied to a continuous variable " << v.getName());

   s_.insert(v);
   b_ = s_.toBitset();
}

bool IntContractor::dependsOn(const Bitset& bs) const
{
   return b_.overlaps(bs);
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
