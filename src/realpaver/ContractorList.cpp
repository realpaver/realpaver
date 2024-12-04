/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   ContractorList.cpp
 * @brief  List of contractors applied in sequence
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorList.hpp"

namespace realpaver {

ContractorList::ContractorList(SharedContractorPool pool)
      : pool_(pool)
{
   if (pool == nullptr)
   {
      pool_ = std::make_shared<ContractorPool>();
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
