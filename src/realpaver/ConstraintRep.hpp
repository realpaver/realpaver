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
 * @file   ConstraintRep.hpp
 * @brief  Base class of constraint representations
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_REP_HPP
#define REALPAVER_CONSTRAINT_REP_HPP

#include "realpaver/ConstraintRelation.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

class ConstraintVisitor;

/// Status of a constraint
enum class ConstraintStatus {
   Initial,   ///< constraint of initial problem (default value)
   Objective, ///< constraint representing an objective function
   Redundant  ///< redundant constraint
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, ConstraintStatus status);

/**
 * @brief Base class of the hierarchy of constraint representations.
 *
 * The common attributes are a scope, a relation symbol, an identifier,
 * and an hash code. The identifier is positive if the constraint has been
 * added in a problem, strictly negative otherwise.
 *
 * The identifier is assigned to a negative value at initialization time, which means that
 * a constraint has no identifier.
 *
 * A constraint can be labelled as a constraint of the initial problem (default) or
 * a constraint used to bound an objective function, or a redundant constraint. Use its
 * status for this.
 */
class ConstraintRep {
public:
   /// Constructor
   ConstraintRep(const ConstraintRelation &rel);

   /// Default copy constructor
   ConstraintRep(const ConstraintRep &) = default;

   /// No assignment
   ConstraintRep &operator=(const ConstraintRep &) = delete;

   /// Virtual destructor
   virtual ~ConstraintRep();

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Returns true if v belongs to the scope of this
   bool dependsOn(Variable v) const;

   /// Returns the relation symbol of this
   ConstraintRelation relation() const;

   /// Returns the identifier of this
   int id() const;

   /// Assigns the identifier of this
   void setId(int id);

   /// Returns true if the identifier of this has not been initiamized
   bool hasNoId() const;

   /// Returns the status of this
   ConstraintStatus status() const;

   /// Assigns the status of this
   void setStatus(ConstraintStatus status);

   /// Returns true if this constraint is variable free
   virtual bool isConstant() const = 0;

   /// Checks the satisfaction of this on a box
   virtual Proof isSatisfied(const IntervalBox &B) = 0;

   /// Checks the satisfaction of this on a box
   virtual Proof isSatisfied(const DomainBox &box) = 0;

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   virtual double violation(const IntervalBox &B) = 0;

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   virtual double violation(const DomainBox &box) = 0;

   /// Contracts a box and returns a certificate of proof
   virtual Proof contract(IntervalBox &B) = 0;

   /// Contracts a box and returns a certificate of proof
   virtual Proof contract(DomainBox &box) = 0;

   /// Output on a stream
   virtual void print(std::ostream &os) const = 0;

   /// Visitor pattern
   virtual void acceptVisitor(ConstraintVisitor &vis) const = 0;

   /// Returns true if this is an equation
   virtual bool isAnEquation() const;

   /// Returns true if this is an inequality constraint (default: false)
   virtual bool isAnInequality() const;

   /// Returns true if this is linear
   virtual bool isLinear() const;

   /// Returns true if this is quadratic
   virtual bool isQuadratic() const;

   /// Returns true if this is polynomial, not linear, and not quadratic
   virtual bool isPolynomial() const;

   /// Returns true if this is an integer arithmetic constraint
   virtual bool isAnIntegerConstraint() const = 0;

   /// Return true if this just bounds a variable
   virtual bool isABoundConstraint() const;

   /// Returns a new representation such that only the root of this is cloned
   virtual ConstraintRep *cloneRoot() const = 0;

protected:
   /// Type of shared constraint representation
   using SharedRep = std::shared_ptr<ConstraintRep>;

   friend class Constraint;

   /// Sets the scope
   void setScope(Scope scop);

private:
   Scope scop_;              // scope
   ConstraintRelation rel_;  // relation symbol
   int id_;                  // identifier
   ConstraintStatus status_; // status

protected:
   size_t hcode_; // hash code
};

} // namespace realpaver

#endif
