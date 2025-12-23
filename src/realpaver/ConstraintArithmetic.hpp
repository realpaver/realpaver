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
 * @file   ConstraintArithmetic.hpp
 * @brief  Arithmetic constraints
 * @author Laurent Granvilliers
 * @date   29 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_ARITHMETIC_HPP
#define REALPAVER_CONSTRAINT_ARITHMETIC_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

class FlatFunction;

/// Base class of arithmetic constraints between two terms
class ConstraintBinary : public ConstraintRep {
public:
   /// Creates a constraint rel(l, r)
   ConstraintBinary(Term l, Term r, ConstraintRelation rel);

   /// Virtual destructor
   virtual ~ConstraintBinary();

   /// Returns the left-hand term
   Term left() const;

   /// Returns the right-hand term
   Term right() const;

   /// Returns the flat function
   std::shared_ptr<FlatFunction> flatFunction() const;

   void print(std::ostream &os) const override;
   bool isConstant() const override;
   bool isEquation() const override;
   bool isInequality() const override;
   bool isLinear() const override;
   bool isBoundConstraint() const override;
   bool isInteger() const override;
   Proof contract(IntervalBox &B) override;
   Proof contract(DomainBox &box) override;

protected:
   std::shared_ptr<FlatFunction> fun_; // flat function

private:
   Term l_, r_; // left-hand term, right-hand term
};

/*----------------------------------------------------------------------------*/

/// Representation of an equation l == r
class ConstraintEq : public ConstraintBinary {
public:
   /// Creates l == r
   ConstraintEq(Term l, Term r);

   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;
};

/// Generates l == r
Constraint operator==(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l <= r
class ConstraintLe : public ConstraintBinary {
public:
   /// Creates l <= r
   ConstraintLe(Term l, Term r);

   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;
};

/// Generates l <= r
Constraint operator<=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l < r
class ConstraintLt : public ConstraintBinary {
public:
   /// Creates l < r
   ConstraintLt(Term l, Term r);

   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;
};

/// Generates l < r
Constraint operator<(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l >= r
class ConstraintGe : public ConstraintBinary {
public:
   /// Creates l >= r
   ConstraintGe(Term l, Term r);

   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;
};

/// Generates l >= r
Constraint operator>=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l > r
class ConstraintGt : public ConstraintBinary {
public:
   /// Creates l > r
   ConstraintGt(Term l, Term r);

   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;
};

/// Generates l > r
Constraint operator>(Term l, Term r);

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of an inequality constraint f in I.
 *
 * It is equivalent to min(I) <= f <= max(I).
 */
class ConstraintIn : public ConstraintBinary {
public:
   /// Creates t in x
   ConstraintIn(Term t, const Interval &x);

   /// Rreturns the bounds of the function
   Interval image() const;

   /// Returns the term
   Term term() const;

   void print(std::ostream &os) const override;
   void acceptVisitor(ConstraintVisitor &vis) const override;
   Proof isSatisfied(const IntervalBox &B) override;
   double violation(const IntervalBox &B) override;
   Proof isSatisfied(const DomainBox &box) override;
   double violation(const DomainBox &box) override;
   ConstraintRep *cloneRoot() const override;

private:
   Interval x_;
};

/// Generates t in x
Constraint in(Term t, const Interval &x);

/// Generates t in [a, b]
Constraint in(Term t, double a, double b);

} // namespace realpaver

#endif
