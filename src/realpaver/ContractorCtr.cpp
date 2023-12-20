///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorCtr.hpp"

namespace realpaver {

ContractorCtr::ContractorCtr(Constraint c) : c_(c)
{}

Scope ContractorCtr::scope() const
{
   return c_.scope();
}

Proof ContractorCtr::contract(IntervalBox& box)
{
   return c_.contract(box);
}

void ContractorCtr::print(std::ostream& os) const
{
   os << "Contractor of " << c_;
}

Constraint ContractorCtr::getConstraint() const
{
   return c_;
}

} // namespace
