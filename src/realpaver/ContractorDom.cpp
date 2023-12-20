///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <sstream>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorDom.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorDom::ContractorDom()
      : s_()
{}

ContractorDom::ContractorDom(Variable v)
      : s_()
{
   insertVar(v);
}

ContractorDom::ContractorDom(const std::initializer_list<Variable>& l)
      : s_()
{
   for (auto v : l)
      insertVar(v);
}

size_t ContractorDom::nbVars() const
{
   return s_.size();
}

void ContractorDom::insertVar(Variable v)
{
   ASSERT(!v.getDomain()->isConnected(),
          "Domain contractor applied to a continuous variable " << v.getName());

   s_.insert(v);
}

Proof ContractorDom::contract(IntervalBox& box)
{
   for (auto v : s_)
   {
      Interval x = box.get(v);

#if LOG_ON
   std::ostringstream os;
   os << "Domain contractor of " << v.getName() << " in " << x;
#endif

      v.getDomain()->contractInterval(x);
      box.set(v, x);

#if LOG_ON
   os << " -> " << x;
   LOG_LOW(os.str());
#endif

      if (x.isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

void ContractorDom::print(std::ostream& os) const
{
   os << "Domain contractor: ";
   for (auto v : s_) os << v.getName() << " ";
}

Scope ContractorDom::scope() const
{
   return s_;
}

} // namespace
