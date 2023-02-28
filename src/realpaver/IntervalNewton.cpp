///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/IntervalNewton.hpp"

namespace realpaver {

IntervalNewton::IntervalNewton(IntervalFunctionVector F)
      : Contractor(),
        F_(F),
        jac_(F.nbFuns(), F.nbVars()),
        y_(F.nbVars()),
        b_(F.nbFuns())
{
   ASSERT(F.nbVars() == F.nbFuns(),
          "Interval Newton defined with a non-square system");
}

Scope IntervalNewton::scope() const
{
   return F_.scope();
}

Proof IntervalNewton::contract(IntervalRegion& reg)
{
   // TODO
}

void IntervalNewton::print(std::ostream& os) const
{
   os << "Interval Newton";
}

} // namespace
