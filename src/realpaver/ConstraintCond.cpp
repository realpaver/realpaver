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
 * @file   ConstraintCond.cpp
 * @brief  Conditional constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintVisitor.hpp"

namespace realpaver {

ConstraintCond::ConstraintCond(Constraint guard, Constraint body)
    : ConstraintRep(ConstraintRelation::Cond)
    , guard_(guard)
    , body_(body)
{
   THROW_IF(!(guard.isInteger() || guard.isInequality()),
            "Bad guard of conditional constraint: " << guard);

   hcode_ = guard.hashCode();
   hcode_ = hash2(body.hashCode(), hcode_);

   Scope scop;
   scop.insert(guard.scope());
   scop.insert(body.scope());
   setScope(scop);
}

Constraint ConstraintCond::guard() const
{
   return guard_;
}

Constraint ConstraintCond::body() const
{
   return body_;
}

bool ConstraintCond::isConstant() const
{
   return guard_.isConstant() && body_.isConstant();
}

Proof ConstraintCond::isSatisfied(const IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.isSatisfied(B);

   return p;
}

double ConstraintCond::violation(const IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);
   return (p == Proof::Inner) ? body_.violation(B) : 0.0;
}

Proof ConstraintCond::contract(IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.contract(B);

   return p;
}

Proof ConstraintCond::isSatisfied(const DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.isSatisfied(box);

   return p;
}

double ConstraintCond::violation(const DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);
   return (p == Proof::Inner) ? body_.violation(box) : 0.0;
}

Proof ConstraintCond::contract(DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.contract(box);

   return p;
}

void ConstraintCond::print(std::ostream &os) const
{
   os << guard_ << " -> " << body_;
}

void ConstraintCond::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

ConstraintRep *ConstraintCond::cloneRoot() const
{
   return new ConstraintCond(guard_, body_);
}

bool ConstraintCond::isInteger() const
{
   return guard_.isInteger() && body_.isInteger();
}

Constraint cond(Constraint guard, Constraint body)
{
   return Constraint(std::make_shared<ConstraintCond>(guard, body));
}

} // namespace realpaver
