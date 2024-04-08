/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   IntervalThickFunction.cpp
 * @brief  Thick interval function
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

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
