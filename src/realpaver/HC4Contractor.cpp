///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/HC4Contractor.hpp"

namespace realpaver {

HC4Contractor::HC4Contractor(Dag* dag, size_t i) : f_(nullptr)
{
   ASSERT(dag != nullptr, "Creation of an HC4Contractor from a null pointer");
   ASSERT(i < dag->nbFun(), "Creation of an HC4Contractor given a bad " <<
                            " function index " << i);

   f_ = dag->fun(i);
}

bool HC4Contractor::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Proof HC4Contractor::contract(IntervalRegion& reg)
{
   return f_->hc4Revise(reg);
}

void HC4Contractor::print(std::ostream& os) const
{
   os << "HC4 contractor / function " << " @ " << f_->index();
}

DagFun* HC4Contractor::getFun() const
{
   return f_;
}

Dag* HC4Contractor::getDag() const
{
   return f_->dag();
}

Scope HC4Contractor::scope() const
{
   return f_->scope();
}

} // namespace
