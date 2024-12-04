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
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalThickFunction.hpp"

namespace realpaver {

IntervalThickFunction::IntervalThickFunction(SharedDag dag, size_t i,
                                             Variable v)
      : IntervalFunctionUni(),
        f_(dag->fun(i)),
        v_(v),
        flat_(nullptr),
        B_({v}),
        G_(1)
{}

IntervalThickFunction::~IntervalThickFunction()
{
   if (flat_ != nullptr) delete flat_;
}

Interval IntervalThickFunction::eval(const Interval& x)
{
   B_.set(v_, x);
   return flat_->iEval(B_);
}

Interval IntervalThickFunction::diff(const Interval& x)
{
   B_.set(v_, x);
   flat_->iDiff(B_, G_);
   return G_[0];
}

void IntervalThickFunction::update(const IntervalBox& B)
{
   if (flat_ != nullptr) delete flat_;
   flat_ = new FlatFunction(f_, B, v_);
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
