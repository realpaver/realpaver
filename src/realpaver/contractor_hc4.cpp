// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/contractor_hc4.hpp"

namespace realpaver {

Hc4Contractor::Hc4Contractor(Dag* dag, size_t i) : f_(nullptr)
{
   ASSERT(dag != nullptr && i < dag->nbFun(),
          "bad initialization of an Hc4Contractor");

   f_ = dag->fun(i);
}

bool Hc4Contractor::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Proof Hc4Contractor::contract(Box& B)
{
   return f_->hc4Revise(B);
}

void Hc4Contractor::print(std::ostream& os) const
{
   os << "HC4 contractor";
}

} // namespace
