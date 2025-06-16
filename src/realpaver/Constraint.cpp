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
 * @date   28 may 2025
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

bool Constraint::isEquation() const
{
   return rep_->isEquation();
}

bool Constraint::isInequality() const
{
   return rep_->isInequality();
}

bool Constraint::isLinear() const
{
   return rep_->isLinear();
}

bool Constraint::isBoundConstraint() const
{
   return rep_->isBoundConstraint();
}

bool Constraint::isInteger() const
{
   return rep_->isInteger();
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
