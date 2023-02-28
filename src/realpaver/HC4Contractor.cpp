///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/Stat.hpp"

namespace realpaver {

HC4Contractor::HC4Contractor(SharedDag dag, size_t i)
      : dag_(dag),
        if_(i)
{
   ASSERT(dag != nullptr, "Creation of an HC4Contractor from a null pointer");
   ASSERT(i < dag->nbFuns(), "Creation of an HC4Contractor given a bad " <<
                             " function index " << i);
}

Proof HC4Contractor::contract(IntervalRegion& reg)
{
#if STAT_ON
   Timer& tim = Stat::getTimeStat("TIMER_HC4");
   tim.start();
#endif

   Proof proof = dag_->fun(if_)->hc4Revise(reg);

#if STAT_ON
   tim.stop();
#endif

   return proof;
}

void HC4Contractor::print(std::ostream& os) const
{
   os << "HC4 contractor / function " << " @ " << if_;
}

size_t HC4Contractor::getFunIndex() const
{
   return if_;
}

SharedDag HC4Contractor::getDag() const
{
   return dag_;
}

Scope HC4Contractor::scope() const
{
   return dag_->fun(if_)->scope();
}

} // namespace
