///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
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

std::ostream& operator<<(std::ostream& os, const Contractor& op)
{
   op.print(os);
   return os;
}

} // namespace