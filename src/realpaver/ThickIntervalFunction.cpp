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

ThickIntervalFunction::ThickIntervalFunction(Dag* dag, size_t i, size_t iv)
      : f_(nullptr),
        if_(i),
        iv_(iv)
{
   f_ = dag->fun(i);
}

DagFun* ThickIntervalFunction::getFun() const
{
   return f_;
}

bool ThickIntervalFunction::dependsOn(const Bitset& bs) const
{
   return f_->dependsOn(bs);
}

Interval ThickIntervalFunction::eval(const Interval& x)
{
   return f_->evalOnly(iv_, x);
}

Interval ThickIntervalFunction::diff(const Interval& x)
{
   return f_->diffOnly(iv_, x) ? f_->deriv(iv_) : Interval::universe();
}

std::pair<Interval, Interval>
ThickIntervalFunction::evalDiff(const Interval& x)
{
   Interval e = f_->evalOnly(iv_, x),
            dv = f_->diffOnly(iv_) ? f_->deriv(iv_) : Interval::universe();

   return std::make_pair(e, dv);
}

Interval ThickIntervalFunction::update(const IntervalVector& X)
{
   return f_->eval(X);
}

size_t ThickIntervalFunction::getVarIndex() const
{
   return iv_;
}

} // namespace
