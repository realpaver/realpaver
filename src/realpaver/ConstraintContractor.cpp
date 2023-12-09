///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintContractor.hpp"

namespace realpaver {

ConstraintContractor::ConstraintContractor(Constraint c) : c_(c)
{}

Scope ConstraintContractor::scope() const
{
   return c_.scope();
}

Proof ConstraintContractor::contract(IntervalBox& B)
{
   return c_.contract(B);
}

void ConstraintContractor::print(std::ostream& os) const
{
   os << "contractor of " << c_;
}

Constraint ConstraintContractor::getConstraint() const
{
   return c_;
}

} // namespace
