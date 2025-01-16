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
 * @brief  Classes of constraints
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONSTRAINT_HPP
#define REALPAVER_CONSTRAINT_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

class ConstraintVisitor;
class FlatFunction;

/// Enumeration of relation symbols
enum class RelSymbol {
   Eq,    ///< equation
   Le,    ///< less than
   Lt,    ///< strictly less than
   Ge,    ///< greater than
   Gt,    ///< stricly greater than
   In,    ///< doubly bounded inequality constraint
   Table, ///< table constraint
   Cond   ///< conditional constraint
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, RelSymbol rel);

/*----------------------------------------------------------------------------*/

/**
 * @brief Base class of the hierarchy of constraint representations.
 *
 * The common attributes are a scope, a relation symbol, and an hash code.
 */
class ConstraintRep {
public:
   /// Constructor
   ConstraintRep(RelSymbol rel);

   /// Default copy constructor
   ConstraintRep(const ConstraintRep&) = default;

   /// No assignment
   ConstraintRep& operator=(const ConstraintRep&) = delete;

   /// Virtual destructor
   virtual ~ConstraintRep();

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Returns true if v belongs to the scope of this
   bool dependsOn(Variable v) const;

   /// Returns the relation symbol of this
   RelSymbol relSymbol() const;

   /// Returns true if this constraint is variable free
   virtual bool isConstant() const = 0;

   /// Checks the satisfaction of this on a box
   virtual Proof isSatisfied(const IntervalBox& B) = 0;

   /// Checks the satisfaction of this on a box
   virtual Proof isSatisfied(const DomainBox& box) = 0;

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   virtual double violation(const IntervalBox& B) = 0;

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   virtual double violation(const DomainBox& box) = 0;

   /// Contracts a box and returns a certificate of proof
   virtual Proof contract(IntervalBox& B) = 0;

   /// Contracts a box and returns a certificate of proof
   virtual Proof contract(DomainBox& box) = 0;

   /// Output on a stream
   virtual void print(std::ostream& os) const = 0;

   /// Visitor pattern
   virtual void acceptVisitor(ConstraintVisitor& vis) const = 0;

   /// Returns true if this is an equation
   virtual bool isEquation() const;

   /// Returns true if this is an inequality constraint (default: false)
   virtual bool isInequality() const;

   /// Returns true if this is linear
   virtual bool isLinear() const;

   /// Returns true if this is an integer arithmetic constraint
   virtual bool isInteger() const = 0;

   /// Return true if this just bounds a variable
   virtual bool isBoundConstraint() const;

   /// Returns a new representation such that only the root of this is cloned
   virtual ConstraintRep* cloneRoot() const = 0;

protected:
   /// Type of shared constraint representation
   using SharedRep = std::shared_ptr<ConstraintRep>;

   friend class Constraint;

   /// Sets the scope
   void setScope(Scope scop);

private:
   Scope scop_;      // scope
   RelSymbol rel_;   // relation symbol

protected:
   size_t hcode_;    // hash code
};

/*----------------------------------------------------------------------------*/

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
   RelSymbol relSymbol() const;

   /// Returns true if this is variable free */
   bool isConstant() const;

   /// Checks the satisfaction of this on a box
   Proof isSatisfied(const IntervalBox& B);

   /// Checks the satisfaction of this on a box
   Proof isSatisfied(const DomainBox& box);

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   double violation(const IntervalBox& B);

   /**
    * @brief Returns the violation of this on a box.
    *
    * Returns 0 if the constraint is satisfied, otherwise a positive real
    * number that indicates how much the constraint is violated.
    */
   double violation(const DomainBox& box);

   /// Contracts a box and returns a certificate of proof
   Proof contract(IntervalBox& B);

   /// Contracts a box and returns a certificate of proof
   Proof contract(DomainBox& box);

   /// Output on a stream
   void print(std::ostream& os) const;

   /// Visitor pattern
   void acceptVisitor(ConstraintVisitor& vis) const;

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
   ConstraintRep* cloneRoot() const;

public:
   /// Type of the shared representation
   using SharedRep = ConstraintRep::SharedRep;

   /// Constructor
   Constraint(const SharedRep& rep = nullptr);

   /// Returns the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;   // shared representation
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, Constraint c);

/*----------------------------------------------------------------------------*/

/// Base class of arithmetic constraints between two terms
class ArithCtrBinary : public ConstraintRep {
public:
   /// Creates a constraint rel(l, r)
   ArithCtrBinary(Term l, Term r, RelSymbol rel);

   /// Virtual destructor
   virtual ~ArithCtrBinary();

   /// Returns the left-hand term
   Term left() const;

   /// Returns the right-hand term
   Term right() const;

   /// Returns the flat function
   std::shared_ptr<FlatFunction> flatFunction() const;

   void print(std::ostream& os) const override;
   bool isConstant() const override;
   bool isEquation() const override;
   bool isInequality() const override;
   bool isLinear() const override;
   bool isBoundConstraint() const override;
   bool isInteger() const override;
   Proof contract(IntervalBox& B) override;
   Proof contract(DomainBox& box) override;

protected:
   std::shared_ptr<FlatFunction> fun_;   // flat function

private:
   Term l_, r_;               // left-hand term, right-hand term
};

/*----------------------------------------------------------------------------*/

/// Representation of an equation l == r
class ArithCtrEq : public ArithCtrBinary {
public:
   /// Creates l == r
   ArithCtrEq(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;
};

/// Generator of l == r
Constraint operator==(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l <= r
class ArithCtrLe : public ArithCtrBinary {
public:
   /// Creates l <= r
   ArithCtrLe(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;
};

/// Generates l <= r
Constraint operator<=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l < r
class ArithCtrLt : public ArithCtrBinary {
public:
   /// Creates l < r
   ArithCtrLt(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;
};

/// Generates l < r
Constraint operator<(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l >= r
class ArithCtrGe : public ArithCtrBinary {
public:
   /// Creates l >= r
   ArithCtrGe(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;
};

/// Generates l >= r
Constraint operator>=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/// Representation of an inequality constraint l > r
class ArithCtrGt : public ArithCtrBinary {
public:
   /// Creates l > r
   ArithCtrGt(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;
};

/// Generates l > r
Constraint operator>(Term l, Term r);

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of an inequality constraint f in I.
 *
 * It is equivalent to min(I) <= f <= max(I).
 */
class ArithCtrIn : public ArithCtrBinary {
public:
   /// Creates t in x
   ArithCtrIn(Term t, const Interval& x);

   /// Rreturns the bounds of the function
   Interval image() const;

   /// Returns the term
   Term term() const;

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   ConstraintRep* cloneRoot() const override;

private:
   Interval x_;
};

/// Generates t in x
Constraint in(Term t, const Interval& x);

/// Generates t in [a, b]
Constraint in(Term t, double a, double b);

/*----------------------------------------------------------------------------*/

/**
 * @brief Column of a table constraint.
 *
 * A column is a variable with a list of values (assignments).
 */
class TableCtrCol {
public:
   /// Constructor v in {}
   TableCtrCol(Variable v);

   /// Constructor v in l
   TableCtrCol(Variable v, const std::initializer_list<Interval>& l);

   /// Returns the number of values of the variable
   size_t size() const;

   /// Gets the i-th value
   Interval getVal(size_t i) const;

   /// Returns the variable
   Variable getVar() const;

   /// Adds a value at the end
   void addValue(const Interval& x);

   /// Returns true if this is an integer variable with integer values
   bool isInteger() const;

private:
   Variable v_;                  // variable
   std::vector<Interval> vval_;  // list of values
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a table constraint.
 *
 * Example: x  y  z
 *          0  1  2
 *          3  4  5
 */
class TableCtr : public ConstraintRep {
public:
   /// Creates an empty constraint
   TableCtr();

   /// Constructor given a list of columns
   TableCtr(const std::initializer_list<TableCtrCol>& l);

   /**
    * @brief Constructor.
    *
    * Input: a list of variables vars and a list of values representing a
    * row-oriented matrix.
    */
   TableCtr(const std::initializer_list<Variable>& vars,
            const std::initializer_list<Interval>& values);

   /// Returns the number of columns (variables)
   size_t nbCols() const;

   /// Returns the number of rows (assignments)
   size_t nbRows() const;

   /// Gets the j-th variable (0 <= j < nbCols)
   Variable getVar(size_t j) const;

   /// Gets the value at position (i, j)
   Interval getVal(size_t i, size_t j) const;

   /// Gets the j-th column
   TableCtrCol getCol(size_t j) const;

   /// Inserts a column at the end
   void addCol(const TableCtrCol& col);

   bool isConstant() const override;
   bool isInteger() const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   void print(std::ostream& os) const override;
   void acceptVisitor(ConstraintVisitor& vis) const override;
   ConstraintRep* cloneRoot() const override;

private:
   std::vector<TableCtrCol> vcol_; // vector of columns

   void makeScopeAndHashCode();
   bool isRowConsistent(size_t i, const DomainBox& box) const;
   bool isRowConsistent(size_t i, const IntervalBox& B) const;
   double rowViolation(const DomainBox& box, size_t i);
   double rowViolation(const IntervalBox& B, size_t i);
};

/**
 * @brief Generates a table constraint.
 *
 * @param vars a list of variables
 * @param values a list of values representing the list of tuples assigned
 *        to the variables, i.e. we have the first tuple in the list,
 *        followed by the second tuple, and so on.
 * @return the constraint
 */
Constraint table(const std::initializer_list<Variable>& vars,
                 const std::initializer_list<Interval>& values);

/**
 * @brief Generates a table constraint.
 *
 * @param vars an array of variables
 * @param nvars the size of vars
 * @param values an array of values representing the list of tuples assigned
 *        to the variables, i.e. we have the first tuple in the list,
 *        followed by the second tuple, and so on.
 * @param nvalues the size of values
 * @return the constraint
 */
Constraint table(const Variable* vars, size_t nvars,
                 const Interval* values, size_t nvalues);

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a conditional constraint guard -> body.
 *
 * The guard must be an integer constraint or an inequality constraint, i.e.
 * a constraint that can be certainly satisfied in a box. The body is any
 * constraint.
 */
class CondCtr : public ConstraintRep {
public:
   /// Constructor
   CondCtr(Constraint guard, Constraint body);

   /// Default copy constructor
   CondCtr(CondCtr&) = default;

   /// No assignment
   CondCtr& operator=(CondCtr&) = delete;

   /// Defaut destructor
   ~CondCtr() = default;

   /// Returns the guard
   Constraint guard() const;

   /// Returns the body
   Constraint body() const;

   bool isConstant() const override;
   bool isInteger() const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   Proof isSatisfied(const DomainBox& box) override;
   double violation(const DomainBox& box) override;
   Proof contract(DomainBox& box) override;
   void print(std::ostream& os) const override;
   void acceptVisitor(ConstraintVisitor& vis) const override;
   ConstraintRep* cloneRoot() const override;

private:
   Constraint guard_;   // guard
   Constraint body_;    // body
};

/// Generates the conditional constraint guard -> body
Constraint cond(Constraint guard, Constraint body);

/*----------------------------------------------------------------------------*/

/// Base class of visitors of constraint representations
class ConstraintVisitor {
public:
   /// Virtual destructor
   virtual ~ConstraintVisitor();

   /// @name Visit methods
   ///@{
   virtual void apply(const ArithCtrEq* c);
   virtual void apply(const ArithCtrLe* c);
   virtual void apply(const ArithCtrLt* c);
   virtual void apply(const ArithCtrGe* c);
   virtual void apply(const ArithCtrGt* c);
   virtual void apply(const ArithCtrIn* c);
   virtual void apply(const TableCtr* c);
   virtual void apply(const CondCtr* c);
   ///@}
};

} // namespace

#endif
