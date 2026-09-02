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

#include "ConstraintRep.hpp"
#include "realpaver/ConstraintRep.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

std::ostream &operator<<(std::ostream &os, ConstraintStatus status)
{
   switch (status)
   {
   case ConstraintStatus::Initial:
      return os << "Initial";
   case ConstraintStatus::Objective:
      return os << "Objective";
   case ConstraintStatus::Redundant:
      return os << "Redundant";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

ConstraintRep::ConstraintRep(const ConstraintRelation &rel)
    : scop_()
    , rel_(rel)
    , id_(-1)
    , status_(ConstraintStatus::Initial)
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
   REALPAVER_ASSERT(id >= 0, "A constraint identifier must be positive: " << id);
   id_ = id;
}

bool ConstraintRep::hasNoId() const
{
   return id_ < 0;
}

ConstraintStatus ConstraintRep::status() const
{
   return status_;
}

void ConstraintRep::setStatus(ConstraintStatus status)
{
   status_ = status;
}

bool ConstraintRep::isAnEquation() const
{
   return false;
}

bool ConstraintRep::isAnInequality() const
{
   return false;
}

bool ConstraintRep::isLinear() const
{
   return false;
}

bool ConstraintRep::isQuadratic() const
{
   return false;
}

bool ConstraintRep::isPolynomial() const
{
   return false;
}

bool ConstraintRep::isABoundConstraint() const
{
   return false;
}

} // namespace realpaver
