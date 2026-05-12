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
 * @file   ConstraintCond.hpp
 * @brief  Conditional constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_COND_HPP
#define REALPAVER_CONSTRAINT_COND_HPP

#include "realpaver/Constraint.hpp"

namespace realpaver {

/**
 * @brief Representation of a conditional constraint guard -> body.
 *
 * The guard must be an integer constraint or an inequality constraint, i.e.
 * a constraint that can be certainly satisfied in a box. The body is an
 * arithmetic constraint.
 */
class ConstraintCond : public ConstraintRep {
public:
   /// Constructor
   ConstraintCond(Constraint guard, Constraint body);

   /// Default copy constructor
   ConstraintCond(ConstraintCond &) = default;

   /// No assignment
   ConstraintCond &operator=(ConstraintCond &) = delete;

   /// Defaut destructor
   ~ConstraintCond() = default;

   /// Returns the guard
   Constraint guard() const;

   /// Returns the body
   Constraint body() const;

   bool isConstant() const override;
   bool isAnIntegerConstraint() const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof contract(IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   Proof contract(DomainBox &box) override;
   void print(std::ostream &os) const override;
   void acceptVisitor(ConstraintVisitor &vis) const override;
   ConstraintRep *cloneRoot() const override;

private:
   Constraint guard_; // guard
   Constraint body_;  // body
};

/// Generates the conditional constraint guard -> body
Constraint cond(Constraint guard, Constraint body);

} // namespace realpaver

#endif
