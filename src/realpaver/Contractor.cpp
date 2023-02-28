///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Contractor.hpp"

namespace realpaver {

Contractor::~Contractor()
{}

void Contractor::print(std::ostream& os) const
{}

bool Contractor::dependsOn(const Variable& v) const
{
   return scope().contains(v);
}

std::ostream& operator<<(std::ostream& os, const Contractor& op)
{
   op.print(os);
   return os;
}

} // namespace
