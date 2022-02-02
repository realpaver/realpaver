// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/thick_fun.hpp"

namespace realpaver {

ThickFun::ThickFun(Dag* dag, size_t i, size_t iv) :
   f_(nullptr), iv_(iv)
{
   f_ = dag->fun(i);
}

Interval ThickFun::eval(const Interval& x)
{
   return f_->evalOnly(iv_, x);
}

Interval ThickFun::diff(const Interval& x)
{
   return f_->diffOnly(iv_, x) ? f_->deriv(iv_) : Interval::universe();
}

std::pair<Interval,Interval> ThickFun::evalDiff(const Interval& x)
{
   Interval e = f_->evalOnly(iv_, x),
            dv = f_->diffOnly(iv_) ? f_->deriv(iv_) : Interval::universe();

   return std::make_pair(e, dv);
}

Interval ThickFun::update(const IntervalVector& X)
{
   return f_->eval(X);
}

size_t ThickFun::getVarIndex() const
{
   return iv_;
}

} // namespace
