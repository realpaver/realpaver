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
 * @file   ConstraintRelaxor.cpp
 * @brief  Relaxation of equations
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintRelaxor.hpp"

namespace realpaver {

ConstraintRelaxor::ConstraintRelaxor(double nu)
      : nu_(nu),
        relaxed_(nullptr)
{
   ASSERT(nu_ > 0.0, "Bad relaxation value " << nu);
}

Constraint ConstraintRelaxor::getRelaxedCtr() const
{
   return relaxed_;
}

void ConstraintRelaxor::apply(const ArithCtrEq* c)
{
   relaxed_ = in(c->left() - c->right(), Interval(-nu_, nu_));
}

} // namespace
