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
 * @file   ConstraintSet.cpp
 * @brief  Set of constraints
 * @author Laurent Granvilliers
 * @date   2 Apr 2026
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintSet.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

void ConstraintSet::insert(Constraint c)
{
   v_.push_back(c);
   scop_.insert(c.scope());
   scop_ = ScopeBank::getInstance()->insertScope(scop_);
}

size_t ConstraintSet::size() const
{
   return v_.size();
}

Constraint ConstraintSet::ctrAt(size_t i) const
{
   REALPAVER_ASSERT(i < size(), "Bad access in a constraint set");
   return v_[i];
}

Scope ConstraintSet::scope() const
{
   return scop_;
}

ConstraintSet::iterator ConstraintSet::begin()
{
   return v_.begin();
}

ConstraintSet::iterator ConstraintSet::end()
{
   return v_.end();
}

ConstraintSet::const_iterator ConstraintSet::cbegin() const
{
   return v_.cbegin();
}

ConstraintSet::const_iterator ConstraintSet::cend() const
{
   return v_.cend();
}

} // namespace realpaver
