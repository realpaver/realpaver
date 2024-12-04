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
 * @file   ContractorHC4Revise.cpp
 * @brief  HC4Revise contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorHC4Revise.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorHC4Revise::ContractorHC4Revise(SharedDag dag, size_t i)
      : dag_(dag),
        idx_(i)
{
   ASSERT(dag != nullptr,
          "Creation of HC4Revise contractor from a null pointer");
   ASSERT(i < dag->nbFuns(), "Creation of HC4Revise contractor given a bad " <<
                             " function index " << i);
}

Proof ContractorHC4Revise::contract(IntervalBox& B)
{
   LOG_LOW("HC4Revise contractor @ " << idx_ << " on " << B);

   Proof proof = dag_->fun(idx_)->hc4Revise(B);

   LOG_LOW(" -> " << proof << ", " << B);

   return proof;
}

void ContractorHC4Revise::print(std::ostream& os) const
{
   os << "HC4Revise contractor / function " << " @ " << idx_;
}

size_t ContractorHC4Revise::getFunIndex() const
{
   return idx_;
}

SharedDag ContractorHC4Revise::getDag() const
{
   return dag_;
}

Scope ContractorHC4Revise::scope() const
{
   return dag_->fun(idx_)->scope();
}

} // namespace
