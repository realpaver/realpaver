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
 * @date   28 May 2025
 */

#include "realpaver/ConstraintRep.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ConstraintRep::ConstraintRep(const ConstraintRelation &rel)
    : scop_()
    , rel_(rel)
    , id_(-1)
    , initial_(true)
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

int ConstraintRep::id() const
{
   return id_;
}

void ConstraintRep::setId(int id)
{
   ASSERT(id >= 0, "A constraint identifier must be positive: " << id);
   id_ = id;
}

bool ConstraintRep::hasNoId() const
{
   return id_ < 0;
}

bool ConstraintRep::isInitial() const
{
   return initial_;
}

void ConstraintRep::setInitial(bool b)
{
   initial_ = b;
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
