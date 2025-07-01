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
 * @file   Contractor.cpp
 * @brief  Base class of contractors
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/Contractor.hpp"

namespace realpaver {

Contractor::~Contractor()
{
}

void Contractor::print(std::ostream &os) const
{
}

bool Contractor::dependsOn(const Variable &v) const
{
   return scope().contains(v);
}

Proof Contractor::contract(IntervalBox &B, const Bitset &active)
{
   return contract(B);
}

std::ostream &operator<<(std::ostream &os, const Contractor &op)
{
   op.print(os);
   return os;
}

/*----------------------------------------------------------------------------*/

ContractMethod::~ContractMethod()
{
}

Proof ContractSimple::apply(Contractor &ctc, IntervalBox &B) const
{
   return ctc.contract(B);
}

ContractActive::ContractActive(const Bitset &active)
    : ContractMethod()
    , active_(active)
{
}

Proof ContractActive::apply(Contractor &ctc, IntervalBox &B) const
{
   return ctc.contract(B, active_);
}

} // namespace realpaver
