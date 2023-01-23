///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONSTRAINT_HPP
#define REALPAVER_CONSTRAINT_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

class ConstraintVisitor;   // forward declaration

///////////////////////////////////////////////////////////////////////////////
/// Enumeration of relation symbols.
///////////////////////////////////////////////////////////////////////////////
enum class RelSymbol {
   Eq, Le, Lt, Ge, Gt, In, Table
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, RelSymbol rel);

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hierarchy of representations of constraints.
///////////////////////////////////////////////////////////////////////////////
class ConstraintRep {
public:
   /// Creates a representation
   ConstraintRep();

   /// Default copy constructor
   ConstraintRep(const ConstraintRep&) = default;

   /// No assignment
   ConstraintRep& operator=(const ConstraintRep&) = delete;

   /// Virtual destructor
   virtual ~ConstraintRep();

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return the scope of this
   Scope scope() const;

   /// @return a bitset this depends on
   Bitset bitset() const;

   /// Dependency test
   /// @param v a variable
   /// @return true if this depends on v
   bool dependsOn(Variable v) const;

   /// Tests if this constraint depends on a variable referenced in a bitset
   /// @param bs a bitset
   /// @return the result of the test
   bool dependsOn(const Bitset& bs) const;

   /// @return true if this constraint is variable free
   virtual bool isConstant() const = 0;

   /// Satisfaction test
   /// @param reg domains of variables
   /// @return a certificate of proof
   virtual Proof isSatisfied(const IntervalRegion& reg) = 0;

   /// Calculates the magnitude of violation
   /// @param reg domains of variables
   /// @return 0 if the constraint is satisfied, otherwise a positive real
   ///         number that indicates how much the constraint is violated
   virtual double violation(const IntervalRegion& reg) = 0;

   /// Contraction of domains
   /// @param reg domains of variables
   /// @return a certificate of proof
   virtual Proof contract(IntervalRegion& reg) = 0;

   /// Output on a stream
   /// @param os a stream
   virtual void print(std::ostream& os) const = 0;

   /// Visitor pattern
   /// @param vis a visitor
   virtual void acceptVisitor(ConstraintVisitor& vis) const = 0;

   /// @return true if this is an equation
   virtual bool isEquation() const;

   /// @return true if this is an inequality constraint
   virtual bool isInequality() const;

   /// @return true if this is linear
   virtual bool isLinear() const;

   /// @return true if this is a bound-constraint of the form f R g where
   ///         f is a variable and g is a constant (or f is a constant and
   ///         g is a variable, and R in {=, >, <, >=, <=}.
   virtual bool isBoundConstraint() const;

protected:
   typedef std::shared_ptr<ConstraintRep> SharedRep;
   friend class Constraint;

   /// Scope assignment
   /// @param s scope assigned to this
   void setScope(Scope s);

private:
   Scope scope_;
   Bitset bs_;

protected:
   size_t hcode_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the main class of constraints.
///
/// This encloses a shared pointer to its representation. It is a lightweight
/// object that can be copied and assigned.
///////////////////////////////////////////////////////////////////////////////
class Constraint {
public:
   /// @return the hash code of this
   size_t hashCode() const;

   /// @return the scope of this
   Scope scope() const;

   /// @return a bitset this depends on
   Bitset bitset() const;

   /// @return true if this is variable free
   bool isConstant() const;

   /// Satisfaction test
   /// @param reg domains of variables
   /// @return a certificate of proof
   Proof isSatisfied(const IntervalRegion& reg);

   /// Calculates the magnitude of violation
   /// @param reg domains of variables
   /// @return 0 if the constraint is satisfied, otherwise a positive real
   ///         number that indicates how much the constraint is violated
   double violation(const IntervalRegion& reg);

   /// Contraction of domains
   /// @param reg domains of variables
   /// @return a certificate of proof
   Proof contract(IntervalRegion& reg);

   /// Output on a stream
   /// @param os a stream
   void print(std::ostream& os) const;

   /// Visitor pattern
   /// @param vis a visitor
   void acceptVisitor(ConstraintVisitor& vis) const;

   /// Dependency test
   /// @param v a variable
   /// @return true if this depends on v
   bool dependsOn(Variable v) const;

   /// Tests if this constraint depends on a variable referenced in a bitset
   /// @param bs a bitset
   /// @return the result of the test
   bool dependsOn(const Bitset& bs) const;

   /// @return true if this is an equation
   bool isEquation() const;

   /// @return true if this is an inequality constraint
   bool isInequality() const;

   /// @return true if this is linear
   bool isLinear() const;

   /// @return true if this is a bound-constraint of the form f R g where
   ///         f is a variable and g is a constant (or f is a constant and
   ///         g is a variable, and R in {=, >, <, >=, <=}.
   bool isBoundConstraint() const;

public:
   /// type of the shared representation
   typedef ConstraintRep::SharedRep SharedRep;

   /// Creates a constraint
   /// @param rep the representation of this
   Constraint(const SharedRep& rep = nullptr);

   /// @return the representation of this
   SharedRep rep() const;

private:
   SharedRep rep_;   // shared representation
};

/// output on a stream
std::ostream& operator<<(std::ostream& os, Constraint c);

///////////////////////////////////////////////////////////////////////////////
/// This is a base class of constraints between two terms.
///////////////////////////////////////////////////////////////////////////////
class ConstraintBin : public ConstraintRep {
public:
   /// Creates a binary constraint
   /// @param l left-hand side
   /// @param r right-hand side
   /// @param rel relation symbol
   ConstraintBin(Term l, Term r, RelSymbol rel);

   /// Virtual destructor
   virtual ~ConstraintBin();

   /// @return the left-hand term
   Term left() const;

   /// @return the right-hand term
   Term right() const;

   /// @return the relation symbol
   RelSymbol relSymbol() const;

   ///@{
   void print(std::ostream& os) const override;
   bool isConstant() const override;
   bool isEquation() const override;
   bool isInequality() const override;
   bool isLinear() const override;
   bool isBoundConstraint() const override;
   ///@}

private:
   Term l_, r_;
   RelSymbol rel_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an equation of the form l = r.
///////////////////////////////////////////////////////////////////////////////
class ConstraintEq : public ConstraintBin {
public:
   /// Creates an equation
   /// @param l left-hand side
   /// @param r right-hand side
   ConstraintEq(Term l, Term r);

   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}
};

/// Creates an equation
/// @param l left-hand side
/// @param r right-hand side
/// @return l = r
Constraint operator==(Term l, Term r);

///////////////////////////////////////////////////////////////////////////////
/// This is an inequality constraint of the form l <= r.
///////////////////////////////////////////////////////////////////////////////
class ConstraintLe : public ConstraintBin {
public:
   /// Creates an inequality constraint
   /// @param l left-hand side
   /// @param r right-hand side
   ConstraintLe(Term l, Term r);
   
   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}
};

/// Creates an inequality constraint
/// @param l left-hand side
/// @param r right-hand side
/// @return l <= r
Constraint operator<=(Term l, Term r);

///////////////////////////////////////////////////////////////////////////////
/// This is an inequality constraint of the form l < r.
///////////////////////////////////////////////////////////////////////////////
class ConstraintLt : public ConstraintBin {
public:
   /// Creates an inequality constraint
   /// @param l left-hand side
   /// @param r right-hand side
   ConstraintLt(Term l, Term r);
   
   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}
};

/// Creates an inequality constraint
/// @param l left-hand side
/// @param r right-hand side
/// @return l < r
Constraint operator< (Term l, Term r);

///////////////////////////////////////////////////////////////////////////////
/// This is an inequality constraint of the form l >= r.
///////////////////////////////////////////////////////////////////////////////
class ConstraintGe : public ConstraintBin {
public:
   /// Creates an inequality constraint
   /// @param l left-hand side
   /// @param r right-hand side
   ConstraintGe(Term l, Term r);
   
   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}
};

/// Creates an inequality constraint
/// @param l left-hand side
/// @param r right-hand side
/// @return l >= r
Constraint operator>=(Term l, Term r);

///////////////////////////////////////////////////////////////////////////////
/// This is an inequality constraint of the form l > r.
///////////////////////////////////////////////////////////////////////////////
class ConstraintGt : public ConstraintBin {
public:
   /// Creates an inequality constraint
   /// @param l left-hand side
   /// @param r right-hand side
   ConstraintGt(Term l, Term r);

   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}
};

/// Creates an inequality constraint
/// @param l left-hand side
/// @param r right-hand side
/// @return l > r
Constraint operator>(Term l, Term r);

///////////////////////////////////////////////////////////////////////////////
/// This is a double inequality constraint of the form f in I.
///
/// The constraint f in I is equivalent to min(I) <= f <= max(I).
///////////////////////////////////////////////////////////////////////////////
class ConstraintIn : public ConstraintBin {
public:
   /// Creates a double inequality constraint
   /// @param t term representing a function
   /// @param x the bounds
   ConstraintIn(Term t, const Interval& x);

   /// @return the bounds of the function
   Interval image() const;

   /// @return the term
   Term term() const;

   ///@{
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   ///@}

private:
   Interval x_;
};

/// Creates a double inequality constraint
/// @param t term representing a function
/// @param x the bounds
/// @return l in x
Constraint in(Term t, const Interval& x);

/// Creates a double inequality constraint
/// @param t term representing a function
/// @param a the lower bound
/// @param b the upper bound
/// @return l in [a, b]
Constraint in(Term t, double a, double b);

///////////////////////////////////////////////////////////////////////////////
/// This is a column of a table constraint.
///////////////////////////////////////////////////////////////////////////////
class ConstraintTableCol {
public:
   /// Constructor
   /// @param v a variable
   ConstraintTableCol(Variable v);

   /// Constructor
   /// @param v a variable
   /// @param l list of values of the variable
   ConstraintTableCol(Variable v, const std::initializer_list<Interval>& l);

   /// @return the number of assignments of the variable
   size_t size() const;

   ///@param i an index between 0 and size()
   /// @return the i-th value of this
   Interval getVal(size_t i) const;

   /// @return the variable
   Variable getVar() const;

   /// Adds a value at the end
   /// @param x value added
   void addValue(const Interval& x);

private:
   Variable v_;
   std::vector<Interval> vval_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a table constraint.
///
/// Example: x  y  z
///          0  1  2
///          3  4  5
///
/// In this example, we have 3 variables and 2 assignments.
/// Each column is represented by an instance of ConstraintTableCol.
///////////////////////////////////////////////////////////////////////////////
class ConstraintTable : public ConstraintRep {
public:
   /// Constructor that creates an empty constraint
   ConstraintTable();

   /// Constructor
   /// @param l list of columns of this
   ConstraintTable(const std::initializer_list<ConstraintTableCol>& l);

   /// Constructor
   /// @param vars a list of variables
   /// @param values list of values representing a row oriented matrix
   ConstraintTable(const std::initializer_list<Variable>& vars,
                   const std::initializer_list<Interval>& values);

   /// @return the number of columns (variables)
   size_t nbCols() const;

   /// @return the number of rows (assignments)
   size_t nbRows() const;

   /// @param a column index between 0 and nbCols
   /// @return the j-th variable
   Variable getVar(size_t j) const;

   /// @param i a row index between 0 and nbRows()
   /// @param a column index between 0 and nbCols
   /// @return the value at position (i, j)
   Interval getVal(size_t i, size_t j) const;

   /// Column access
   /// @param j a column index between 0 and nbCols()
   /// @return the j-th column of this
   ConstraintTableCol getCol(size_t j) const;

   /// Inserts a column in the last place
   /// @param col column added to this
   void addCol(const ConstraintTableCol& col);

   ///@{
   bool isConstant() const override;
   Proof isSatisfied(const IntervalRegion& reg) override;
   double violation(const IntervalRegion& reg) override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   void acceptVisitor(ConstraintVisitor& vis) const override;
   ///@}

private:
   std::vector<ConstraintTableCol> vcol_; // vector of columns

   void makeScopeAndHashCode();
   bool isRowConsistent(size_t i, const IntervalRegion& reg) const;
   double rowViolation(const IntervalRegion& reg, size_t i);
};

/// Creates a table constraint
/// @param vars a list of variables
/// @param values a list of values representing the list of tuples assigned
///               to the variables, i.e. we have the first tuple in the list,
///               followed by the second tuple, and so on
/// @return the constraint
Constraint table(const std::initializer_list<Variable>& vars,
                 const std::initializer_list<Interval>& values);

/// Creates a table constraint
/// @param vars an array of variables
/// @param nvars the size of vars
/// @param values an array of values representing the list of tuples assigned
///               to the variables, i.e. we have the first tuple in the list,
///               followed by the second tuple, and so on
/// @param nvalues the size if values
/// @return the constraint
Constraint table(const Variable* vars, size_t nvars,
                 const Interval* values, size_t nvalues);

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor of constraint representations.
///////////////////////////////////////////////////////////////////////////////
class ConstraintVisitor {
public:
   /// Virtual destructor
   virtual ~ConstraintVisitor();

   ///@{
   virtual void apply(const ConstraintEq* c);
   virtual void apply(const ConstraintLe* c);
   virtual void apply(const ConstraintLt* c);
   virtual void apply(const ConstraintGe* c);
   virtual void apply(const ConstraintGt* c);
   virtual void apply(const ConstraintIn* c);
   virtual void apply(const ConstraintTable* c);
   ///@}
};

} // namespace

#endif
