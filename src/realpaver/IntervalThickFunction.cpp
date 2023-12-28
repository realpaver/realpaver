///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalThickFunction.hpp"

namespace realpaver {

IntervalThickFunction::IntervalThickFunction(SharedDag dag, size_t i,
                                             Variable v)
      : IntervalFunctionUni(),
        dag_(dag),
        f_(nullptr),
        v_(v)
{
   f_ = dag->fun(i);
}

Interval IntervalThickFunction::eval(const Interval& x)
{
   return f_->intervalEvalOnly(v_, x);
}

Interval IntervalThickFunction::diff(const Interval& x)
{
   return f_->intervalDiffOnly(v_, x) ? f_->intervalDeriv(v_) :
                                        Interval::universe();
}

Interval IntervalThickFunction::update(const IntervalBox& B)
{
   return f_->intervalEval(B);
}

Variable IntervalThickFunction::getVar() const
{
   return v_;
}

DagFun* IntervalThickFunction::getFun() const
{
   return f_;
}

} // namespace
