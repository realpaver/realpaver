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
 * @file   Constraint.cpp
 * @brief  Main class of constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#include "realpaver/Constraint.hpp"

namespace realpaver {

Constraint::Constraint(const SharedRep &rep)
    : rep_(rep)
{
}

Constraint::SharedRep Constraint::rep() const
{
   return rep_;
}

size_t Constraint::hashCode() const
{
   return rep_->hashCode();
}

Scope Constraint::scope() const
{
   return rep_->scope();
}

ConstraintRelation Constraint::relation() const
{
   return rep_->relation();
}

int Constraint::id() const
{
   return rep_->id();
}

void Constraint::setId(int id)
{
   rep_->setId(id);
}

bool Constraint::hasNoId() const
{
   return rep_->hasNoId();
}

ConstraintStatus Constraint::status() const
{
   return rep_->status();
}

void Constraint::setStatus(ConstraintStatus status)
{
   rep_->setStatus(status);
}

bool Constraint::isConstant() const
{
   return rep_->isConstant();
}

void Constraint::print(std::ostream &os) const
{
   rep_->print(os);
}

void Constraint::acceptVisitor(ConstraintVisitor &vis) const
{
   rep_->acceptVisitor(vis);
}

Proof Constraint::isSatisfied(const IntervalBox &B)
{
   return rep_->isSatisfied(B);
}

double Constraint::violation(const IntervalBox &B)
{
   return rep_->violation(B);
}

Proof Constraint::contract(IntervalBox &B)
{
   return rep_->contract(B);
}

Proof Constraint::isSatisfied(const DomainBox &box)
{
   return rep_->isSatisfied(box);
}

double Constraint::violation(const DomainBox &box)
{
   return rep_->violation(box);
}

Proof Constraint::contract(DomainBox &box)
{
   return rep_->contract(box);
}

bool Constraint::dependsOn(Variable v) const
{
   return rep_->dependsOn(v);
}

bool Constraint::isAnEquation() const
{
   return rep_->isAnEquation();
}

bool Constraint::isAnInequality() const
{
   return rep_->isAnInequality();
}

bool Constraint::isLinear() const
{
   return rep_->isLinear();
}

bool Constraint::isQuadratic() const
{
   return rep_->isQuadratic();
}

bool Constraint::isPolynomial() const
{
   return rep_->isPolynomial();
}

bool Constraint::isNonLinear() const
{
   return (!isLinear()) && (!isQuadratic()) && (!isPolynomial());
}

bool Constraint::isABoundConstraint() const
{
   return rep_->isABoundConstraint();
}

bool Constraint::isAnIntegerConstraint() const
{
   return rep_->isAnIntegerConstraint();
}

std::ostream &operator<<(std::ostream &os, const Constraint &c)
{
   c.print(os);
   return os;
}

ConstraintRep *Constraint::cloneRoot() const
{
   return rep_->cloneRoot();
}

} // namespace realpaver
