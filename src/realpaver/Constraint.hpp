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
 * @file   Constraint.hpp
 * @brief  Main class of constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_HPP
#define REALPAVER_CONSTRAINT_HPP

#include "realpaver/ConstraintRep.hpp"

namespace realpaver {

/**
 * @brief The main Constraint class.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class Constraint {
public:
   /// ReturnS the hash code of this
   size_t hashCode() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Returns the relation symbol of this
   ConstraintRelation relation() const;

   /// Returns the identifier of this
   int id() const;

   /// Assigns the identifier of this
   void setId(int id);

   /// Returns true if the identifier of this has not been initiamized
   bool hasNoId() const;

   /// Returns true if this belongs to the initial problem, false otherwise
   bool isInitial() const;

   /// Assigns the status of the constraint: initial or not (redundant, ...)
   void setInitial(bool b = true);

   /// Returns true if this is variable free
   bool isConstant() const;

   /// Checks the satisfaction of this on a box
   Proof isSatisfied(const IntervalBox &B);

   /// Checks the satisfaction of this on a box
   Proof isSatisfied(const DomainBox &box);

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   double violation(const IntervalBox &B);

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   double violation(const DomainBox &box);

   /// Contracts a box and returns a certificate of proof
   Proof contract(IntervalBox &B);

   /// Contracts a box and returns a certificate of proof
   Proof contract(DomainBox &box);

   /// Output on a stream
   void print(std::ostream &os) const;

   /// Visitor pattern
   void acceptVisitor(ConstraintVisitor &vis) const;

   /// Returns true if v belongs to the scope of this
   bool dependsOn(Variable v) const;

   /** @return true if this is an equation */
   bool isEquation() const;

   /// Returns true if this is an inequality constraint
   bool isInequality() const;

   /// Returns true if this is linear
   bool isLinear() const;

   /// Returns true if this is a bound constraint
   bool isBoundConstraint() const;

   /// Returns true if this is an integer constraint
   bool isInteger() const;

   /// Returns a new representation such that only the root of this is cloned
   ConstraintRep *cloneRoot() const;

public:
   /// Type of the shared representation
   using SharedRep = ConstraintRep::SharedRep;

   /// Constructor
   Constraint(const SharedRep &rep = nullptr);

   /// Returns the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_; // shared representation
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Constraint &c);

} // namespace realpaver

#endif
