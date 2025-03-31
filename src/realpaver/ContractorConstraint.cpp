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
 * @file   ContractorConstraint.cpp
 * @brief  Contractor associated with a constraint
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

ContractorConstraint::ContractorConstraint(Constraint c)
    : c_(c)
{
}

Scope ContractorConstraint::scope() const
{
   return c_.scope();
}

Proof ContractorConstraint::contract(IntervalBox &B)
{
   return c_.contract(B);
}

void ContractorConstraint::print(std::ostream &os) const
{
   os << "Contractor of " << c_;
}

Constraint ContractorConstraint::getConstraint() const
{
   return c_;
}

} // namespace realpaver
