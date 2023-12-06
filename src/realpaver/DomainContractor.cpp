///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainContractor.hpp"

namespace realpaver {

DomainContractor::DomainContractor()
      : s_()
{}

DomainContractor::DomainContractor(Variable v)
      : s_()
{
   insertVar(v);
}

DomainContractor::DomainContractor(const std::initializer_list<Variable>& l)
      : s_()
{
   for (auto v : l)
      insertVar(v);
}

size_t DomainContractor::nbVars() const
{
   return s_.size();
}

void DomainContractor::insertVar(Variable v)
{
   ASSERT(!v.getDomain()->isConnected(),
          "DomainContractor applied to a continuous variable " << v.getName());

   s_.insert(v);
}

Proof DomainContractor::contract(IntervalRegion& reg)
{
   for (auto v : s_)
   {
      Interval x = reg.get(v);
      v.getDomain()->contract(x);
      reg.set(v, x);

      if (x.isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

void DomainContractor::print(std::ostream& os) const
{
   os << "integral: ";
   for (auto v : s_) os << v.getName() << " ";
}

Scope DomainContractor::scope() const
{
   return s_;
}

} // namespace
