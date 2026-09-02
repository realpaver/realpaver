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
 * @date   11 Apr 2024
 */

#include "realpaver/ContractorHC4Revise.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorHC4Revise::ContractorHC4Revise(Constraint c)
    : Contractor()
    , ctc_(c)
{
}

Proof ContractorHC4Revise::contract(IntervalBox &B)
{
   LOG_LOW("HC4Revise contractor @ " << ctc_.getConstraint().id() << " on " << B);
   return ctc_.contract(B);
}

Proof ContractorHC4Revise::contract(IntervalBox &B, const Bitset &active)
{
   LOG_LOW("HC4Revise contractor @ " << ctc_.getConstraint().id() << " on " << B);
   return ctc_.contract(B, active);
}

void ContractorHC4Revise::print(std::ostream &os) const
{
   os << "HC4Revise contractor on @ " << ctc_.getConstraint();
}

Scope ContractorHC4Revise::scope() const
{
   return ctc_.scope();
}

} // namespace realpaver
