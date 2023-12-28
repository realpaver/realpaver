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
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorDomain::ContractorDomain()
      : scop_()
{}

ContractorDomain::ContractorDomain(Variable v)
      : scop_()
{
   insertVar(v);
}

ContractorDomain::ContractorDomain(const std::initializer_list<Variable>& l)
      : scop_()
{
   for (const auto& v : l)
      insertVar(v);
}

size_t ContractorDomain::nbVars() const
{
   return scop_.size();
}

void ContractorDomain::insertVar(Variable v)
{
   ASSERT(!v.getDomain()->isConnected(),
          "Domain contractor applied to a continuous variable " << v.getName());

   scop_.insert(v);
}

Proof ContractorDomain::contract(IntervalBox& B)
{
   for (const auto& v : scop_)
   {
      Interval x = B.get(v);

#if LOG_ON
   std::ostringstream os;
   os << "Domain contractor of " << v.getName() << " in " << x;
#endif

      v.getDomain()->contractInterval(x);
      B.set(v, x);

#if LOG_ON
   os << " -> " << x;
   LOG_LOW(os.str());
#endif

      if (x.isEmpty()) return Proof::Empty;
   }
   return Proof::Maybe;
}

void ContractorDomain::print(std::ostream& os) const
{
   os << "Domain contractor: ";
   for (const auto& v : scop_) os << v.getName() << " ";
}

Scope ContractorDomain::scope() const
{
   return scop_;
}

} // namespace
