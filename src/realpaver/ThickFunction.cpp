///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ThickFunction.hpp"

namespace realpaver {

ThickFunction::ThickFunction(SharedDag dag, size_t i, Variable v)
      : dag_(dag),
        f_(nullptr),
        v_(v)
{
   f_ = dag->fun(i);
}

Interval ThickFunction::eval(const Interval& x)
{
   return f_->intervalEvalOnly(v_, x);
}

Interval ThickFunction::diff(const Interval& x)
{
   return f_->intervalDiffOnly(v_, x) ? f_->intervalDeriv(v_) :
                                        Interval::universe();
}

Interval ThickFunction::update(const IntervalBox& box)
{
   return f_->intervalEval(box);
}

Variable ThickFunction::getVar() const
{
   return v_;
}

DagFun* ThickFunction::getFun() const
{
   return f_;
}

} // namespace
