///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ListContractor.hpp"

namespace realpaver {

ListContractor::ListContractor(SharedContractorPool pool)
      : pool_(pool)
{}

size_t ListContractor::poolSize() const
{
   return pool_->poolSize();
}

SharedContractorPool ListContractor::getPool() const
{
   return pool_;
}

void ListContractor::setPool(SharedContractorPool pool)
{
   pool_ = pool;
}

Scope ListContractor::scope() const
{
   return pool_->scope();
}

Proof ListContractor::contract(IntervalRegion& reg)
{
   Proof proof;

   for (size_t i=0; i<poolSize(); ++i)
   {
      proof = pool_->contractorAt(i)->contract(reg);
      if (proof == Proof::Empty) return Proof::Empty;
   }
   return Proof::Maybe;
}

void ListContractor::print(std::ostream& os) const
{
   os << "List of " << poolSize() << " contractors";
}

} // namespace
