///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ThickIntervalFunction.hpp"

namespace realpaver {

ThickIntervalFunction::ThickIntervalFunction(SharedDag dag, size_t i,
                                             Variable v)
      : dag_(dag),
        f_(nullptr),
        if_(i),
        v_(v)
{
   f_ = dag->fun(i);
}

bool ThickIntervalFunction::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Interval ThickIntervalFunction::eval(const Interval& x)
{
   return f_->evalOnly(v_, x);
}

Interval ThickIntervalFunction::diff(const Interval& x)
{
   return f_->diffOnly(v_, x) ? f_->intervalDeriv(v_) : Interval::universe();
}

std::pair<Interval, Interval>
ThickIntervalFunction::evalDiff(const Interval& x)
{
   Interval e = f_->evalOnly(v_, x),
            dv = f_->diffOnly(v_) ? f_->intervalDeriv(v_) :
                                    Interval::universe();

   return std::make_pair(e, dv);
}

Interval ThickIntervalFunction::update(const IntervalRegion& reg)
{
   return f_->eval(reg);
}

Variable ThickIntervalFunction::getVar() const
{
   return v_;
}

DagFun* ThickIntervalFunction::getFun() const
{
   return f_;
}


} // namespace
