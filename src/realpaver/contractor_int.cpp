// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/contractor_int.hpp"

namespace realpaver {

IntContractor::IntContractor() : s_(), b_()
{}

IntContractor::IntContractor(const Variable& v) : s_(), b_()
{
   insert(v);
}

IntContractor::IntContractor(const std::initializer_list<Variable>& l) :
   s_(), b_()
{
   for (auto v : l)
      insert(v);
}

void IntContractor::insert(const Variable& v)
{
   ASSERT(v.isDiscrete(), "Not an integer variable: " << v.getName());

   s_.insert(v);
   b_ = s_.toBitset();
}

bool IntContractor::dependsOn(const Bitset& bs) const
{
   return b_.overlaps(bs);
}

Proof IntContractor::contract(IntervalVector& X)
{
   for (auto v : s_)
   {
      Interval rnd = round(X[v.getId()]);
      X.set(v.getId(), rnd);

      if (rnd.isEmpty())
         return Proof::Empty;
   }
   return Proof::Maybe;
}

void IntContractor::print(std::ostream& os) const
{
   os << "integral: ";
   for (auto v : s_)
      os << v.getName() << " ";
}

} // namespace
