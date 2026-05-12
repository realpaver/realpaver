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
   bool isAnEquation() const override;
   bool isAnInequality() const override;
   bool isLinear() const override;
   bool isQuadratic() const override;
   bool isPolynomial() const override;
   bool isABoundConstraint() const override;
   bool isAnIntegerConstraint() const override;
   Proof contract(IntervalBox &B) override;
   Proof contract(DomainBox &box) override;

   /**
    * @brief Simplifies a term.
    *
    * It tries to rewrite t if it is a linear term to eliminate
    * multiple occurrences of variables.
    */
   static Term simplify(Term t);

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

/**
 * @brief Creates an equation.
 *
 * Generates l == r.
 * Throws an exception if there is no variable in l-r.
 */
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

/**
 * @brief Creates an equation.
 *
 * Generates l <= r.
 * Throws an exception if there is no variable in l-r.
 */
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

/**
 * @brief Creates an equation.
 *
 * Generates l < r.
 * Throws an exception if there is no variable in l-r.
 */
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

/**
 * @brief Creates an equation.
 *
 * Generates l >= r.
 * Throws an exception if there is no variable in l-r.
 */
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

/**
 * @brief Creates an equation.
 *
 * Generates l > r.
 * Throws an exception if there is no variable in l-r.
 */
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

/**
 * @brief Creates an equation.
 *
 * Generates t in x.
 * Throws an exception if there is no variable in t or x is empty.
 */
Constraint in(Term t, const Interval &x);

/**
 * @brief Creates an equation.
 *
 * Generates t in [a,b].
 * Throws an exception if there is no variable in t or [a,b] is empty.
 */
Constraint in(Term t, double a, double b);

} // namespace realpaver

#endif
