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
 * @file   ConstraintRep.cpp
 * @brief  Base class of constraint representations
 * @author Laurent Granvilliers
 * @date   28 may 2025
 */

#include "realpaver/ConstraintRep.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ConstraintRep::ConstraintRep(const ConstraintRelation &rel)
    : scop_()
    , rel_(rel)
    , hcode_(0)
{
}

ConstraintRep::~ConstraintRep()
{
}

size_t ConstraintRep::hashCode() const
{
   return hcode_;
}

Scope ConstraintRep::scope() const
{
   return scop_;
}

void ConstraintRep::setScope(Scope scop)
{
   scop_ = ScopeBank::getInstance()->insertScope(scop);
}

bool ConstraintRep::dependsOn(Variable v) const
{
   return scop_.contains(v);
}

ConstraintRelation ConstraintRep::relation() const
{
   return rel_;
}

bool ConstraintRep::isEquation() const
{
   return false;
}

bool ConstraintRep::isInequality() const
{
   return false;
}

bool ConstraintRep::isLinear() const
{
   return false;
}

bool ConstraintRep::isBoundConstraint() const
{
   return false;
}

} // namespace realpaver
