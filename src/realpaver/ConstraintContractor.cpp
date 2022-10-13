///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ConstraintContractor.hpp"

namespace realpaver {

ConstraintContractor::ConstraintContractor(const Constraint& c)
      : c_(c)
{}

bool ConstraintContractor::dependsOn(const Bitset& bs) const
{
   return c_.dependsOn(bs);
}

Scope ConstraintContractor::scope() const
{

}

Proof ConstraintContractor::contract(IntervalRegion& reg)
{

}

void ConstraintContractor::print(std::ostream& os) const
{

}

Constraint ConstraintContractor::getConstrant() const
{

}

} // namespace
