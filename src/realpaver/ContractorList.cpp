///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorList.hpp"

namespace realpaver {

ContractorList::ContractorList(SharedContractorPool pool)
      : pool_(pool)
{
   if (pool == nullptr)
   {
      pool_ = std::make_shared<ContractorVector>();
   }
}

size_t ContractorList::poolSize() const
{
   return pool_->poolSize();
}

SharedContractorPool ContractorList::getPool() const
{
   return pool_;
}

void ContractorList::setPool(SharedContractorPool pool)
{
   pool_ = pool;
}

void ContractorList::push(SharedContractor op)
{
   pool_->push(op);
}

Scope ContractorList::scope() const
{
   return pool_->scope();
}

Proof ContractorList::contract(IntervalBox& B)
{
   Proof proof;

   for (size_t i=0; i<poolSize(); ++i)
   {
      proof = pool_->contractorAt(i)->contract(B);
      if (proof == Proof::Empty) return Proof::Empty;
   }
   return Proof::Maybe;
}

void ContractorList::print(std::ostream& os) const
{
   os << "List of " << poolSize() << " contractors";
}

} // namespace
