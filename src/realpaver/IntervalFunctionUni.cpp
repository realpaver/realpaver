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
 * @file   IntervalFunctionUni.cpp
 * @brief  Base class of univariate interval functions
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "AssertDebug.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

namespace realpaver {

IntervalFunctionUni::~IntervalFunctionUni()
{
}

IntervalFunctionUniv::IntervalFunctionUniv(IntervalFunction f)
    : IntervalFunctionUni()
    , f_(f)
    , B_(f.scope())
{
   REALPAVER_ASSERT(f.nbVars() == 1, "Not a univariate function");
}

Interval IntervalFunctionUniv::eval(const Interval &x)
{
   B_.set(f_.scope().var(0), x);
   return f_.eval(B_);
}

Interval IntervalFunctionUniv::diff(const Interval &x)
{
   B_.set(f_.scope().var(0), x);
   IntervalVector G(1);
   f_.diff(B_, G);
   return G[0];
}

} // namespace realpaver
