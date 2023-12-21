///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorConstraint.hpp"

namespace realpaver {

ContractorConstraint::ContractorConstraint(Constraint c) : c_(c)
{}

Scope ContractorConstraint::scope() const
{
   return c_.scope();
}

Proof ContractorConstraint::contract(IntervalBox& box)
{
   return c_.contract(box);
}

void ContractorConstraint::print(std::ostream& os) const
{
   os << "Contractor of " << c_;
}

Constraint ContractorConstraint::getConstraint() const
{
   return c_;
}

} // namespace
