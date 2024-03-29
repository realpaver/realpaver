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
 * @date   2022-5-6
*/

#ifndef REALPAVER_CONSTRAINT_HPP
#define REALPAVER_CONSTRAINT_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

class ConstraintVisitor;

/** @brief Enumeration of relation symbols */
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

/** @brief Output on a stream */
std::ostream& operator<<(std::ostream& os, RelSymbol rel);

/*----------------------------------------------------------------------------*/

/** @brief Base class of the hierarchy of constraint representations */
class ConstraintRep {
public:
   /**
    * @brief Creates a representation
    * @param rel relation symbol
    */
   ConstraintRep(RelSymbol rel);

   /** @brief Default copy constructor */
   ConstraintRep(const ConstraintRep&) = default;

   /** @brief No assignment */
   ConstraintRep& operator=(const ConstraintRep&) = delete;

   /** @brief Virtual destructor */
   virtual ~ConstraintRep();

   /** @return the hash code of this */
   size_t hashCode() const;

   /** @return the scope of this */
   Scope scope() const;

   /**
    * @brief Dependency test
    * @param v a variable
    * @return true if this depends on v
    */
   bool dependsOn(Variable v) const;

   /** @return the relation symbol of this */
   RelSymbol relSymbol() const;

   /** @return true if this constraint is variable free */
   virtual bool isConstant() const = 0;

   /**
    * @brief Satisfaction test
    * @param B domains of variables
    * @return a certificate of proof
    */
   virtual Proof isSatisfied(const IntervalBox& B) = 0;

   /**
    * @brief Magnitude of violation
    * @param B domains of variables
    * @return 0 if the constraint is satisfied, otherwise a positive real
    *         number that indicates how much the constraint is violated
    */
   virtual double violation(const IntervalBox& B) = 0;

   /**
    * @brief Contraction of domains
    * @param B domains of variables
    * @return a certificate of proof
    */
   virtual Proof contract(IntervalBox& B) = 0;

   /** @brief Output on a stream */
   virtual void print(std::ostream& os) const = 0;

   /** @brief Visitor pattern */
   virtual void acceptVisitor(ConstraintVisitor& vis) const = 0;

   /** @return true if this is an equation */
   virtual bool isEquation() const;

   /** @return true if this is an inequality constraint (default: false) */
   virtual bool isInequality() const;

   /** @return true if this is linear */
   virtual bool isLinear() const;

   /** @return true if this is an integer arithmetic constraint */
   virtual bool isInteger() const = 0;

   /** @return true if this just bounds a variable */
   virtual bool isBoundConstraint() const;

   /** @return a new representation such that the root of this is cloned */
   virtual ConstraintRep* cloneRoot() const = 0;

protected:
   /** @brief type of shared constraint representation */
   typedef std::shared_ptr<ConstraintRep> SharedRep;

   friend class Constraint;

   /**
    * @brief Sets the scope */
   void setScope(Scope scop);

private:
   Scope scop_;      // scope
   RelSymbol rel_;   // relation symbol

protected:
   size_t hcode_;    // hash code
};

/*----------------------------------------------------------------------------*/

/**
 * @brief The main Constraint class
 * 
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class Constraint {
public:
   /** @return the hash code of this */
   size_t hashCode() const;

   /** @return the scope of this */
   Scope scope() const;

   /** @return the relation symbol of this */
   RelSymbol relSymbol() const;

   /** @return true if this is variable free */
   bool isConstant() const;

   /**
    * @brief Satisfaction test
    * @param B domains of variables
    * @return a certificate of proof
    */
   Proof isSatisfied(const IntervalBox& B);

   /**
    * @brief Magnitude of violation
    * @param B domains of variables
    * @return 0 if the constraint is satisfied, otherwise a positive real
    *         number that indicates how much the constraint is violated
    */
   double violation(const IntervalBox& B);

   /**
    * @brief Contraction of domains
    * @param B domains of variables
    * @return a certificate of proof
    */
   Proof contract(IntervalBox& B);

   /** @brief Output on a stream */
   void print(std::ostream& os) const;

   /** @brief Visitor pattern */
   void acceptVisitor(ConstraintVisitor& vis) const;

   /**
    * @brief Dependency test
    * @param v a variable
    * @return true if this depends on v
    */
   bool dependsOn(Variable v) const;

   /** @return true if this is an equation */
   bool isEquation() const;

   /** @return true if this is an inequality constraint */
   bool isInequality() const;

   /** @return true if this is linear */
   bool isLinear() const;

   /** @return true if this just bounds a variable */
   bool isBoundConstraint() const;

   /** @return true if this is an integer constraint */
   bool isInteger() const;

   /** @return a new representation such that the root of this is cloned */
   ConstraintRep* cloneRoot() const;

public:
   /** @brief type of the shared representation */
   typedef ConstraintRep::SharedRep SharedRep;

   /**
    * @brief Constructor
    * @param rep the representation of this
    */
   Constraint(const SharedRep& rep = nullptr);

   /** @return the representation of this */
   SharedRep rep() const;

private:
   SharedRep rep_;   // shared representation
};

/** @brief Output on a stream */
std::ostream& operator<<(std::ostream& os, Constraint c);

/*----------------------------------------------------------------------------*/

/** @brief Base class of arithmetic constraints between two terms */
class ArithCtrBinary : public ConstraintRep {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    * @param rel relation symbol
    */
   ArithCtrBinary(Term l, Term r, RelSymbol rel);

   /** @brief Virtual destructor */
   virtual ~ArithCtrBinary();

   /** @return the left-hand term */
   Term left() const;

   /** @return the right-hand term */
   Term right() const;

   void print(std::ostream& os) const override;
   bool isConstant() const override;
   bool isEquation() const override;
   bool isInequality() const override;
   bool isLinear() const override;
   bool isBoundConstraint() const override;
   bool isInteger() const override;

private:
   Term l_, r_;   // left-hand term, right-hand term
};

/*----------------------------------------------------------------------------*/

/** @brief Class of equations l == r */
class ArithCtrEq : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    */
   ArithCtrEq(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;
};

/**
 * Generator
 * @param l left-hand side
 * @param r right-hand side
 * @return l == r
 */
Constraint operator==(Term l, Term r);

/*----------------------------------------------------------------------------*/

/** @brief Class of inequality constraint l <= r */
class ArithCtrLe : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    */
   ArithCtrLe(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;
};

/**
 * Generator
 * @param l left-hand side
 * @param r right-hand side
 * @return l <= r
 */
Constraint operator<=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/** @brief Class of inequality constraint l < r */
class ArithCtrLt : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    */
   ArithCtrLt(Term l, Term r);
   
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;
};


/**
 * Generator
 * @param l left-hand side
 * @param r right-hand side
 * @return l < r
 */
Constraint operator<(Term l, Term r);

/*----------------------------------------------------------------------------*/

/** @brief Class of inequality constraint l >= r */
class ArithCtrGe : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    */
   ArithCtrGe(Term l, Term r);
   
   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;
};

/**
 * Generator
 * @param l left-hand side
 * @param r right-hand side
 * @return l >= r
 */
Constraint operator>=(Term l, Term r);

/*----------------------------------------------------------------------------*/

/** @brief Class of inequality constraint l > r */
class ArithCtrGt : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param l left-hand side
    * @param r right-hand side
    */
   ArithCtrGt(Term l, Term r);

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;
};

/**
 * Generator
 * @param l left-hand side
 * @param r right-hand side
 * @return l > r
 */
Constraint operator>(Term l, Term r);

/*----------------------------------------------------------------------------*/

/**
 * @brief Class of inequality constraint f in I
 * 
 * The constraint f in I is equivalent to min(I) <= f <= max(I).
 */
class ArithCtrIn : public ArithCtrBinary {
public:
   /**
    * @brief Constructor
    * @param t term representing a function
    * @param x the bounds
    */
   ArithCtrIn(Term t, const Interval& x);

   /** @return the bounds of the function */
   Interval image() const;

   /** @return the term */
   Term term() const;

   void acceptVisitor(ConstraintVisitor& vis) const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   ConstraintRep* cloneRoot() const override;

private:
   Interval x_;
};

/**
 * Generator
 * @param t term representing a function
 * @param x the bounds
 * @return the constraint t in x
 */
Constraint in(Term t, const Interval& x);

/** Generator
 * @param t term representing a function
 * @param a the lower bound
 * @param b the upper bound
 * @return the constraint t in [a, b]
 */
Constraint in(Term t, double a, double b);

/*----------------------------------------------------------------------------*/

/**
 * @brief Column of a table constraint
 * 
 * A column is a variable with a list of values.
 */
class TableCtrCol {
public:
   /**
    * @brief Constructor
    * @param v a variable
    */
   TableCtrCol(Variable v);

   /**
    * @brief Constructor
    * @param v a variable
    * @param l list of values of the variable
    */
   TableCtrCol(Variable v, const std::initializer_list<Interval>& l);

   /** @return the number of values of the variable */
   size_t size() const;

   /**
    * @brief Gets a value
    * @param i an index between 0 and size()
    * @return the i-th value of this
    */
   Interval getVal(size_t i) const;

   /** @return the variable */
   Variable getVar() const;

   /**
    * @brief Adds a value at the end
    * @param x value added
    */
   void addValue(const Interval& x);

   /** @return true if this is an integer variable with integer values */
   bool isInteger() const;

private:
   Variable v_;                  // variable
   std::vector<Interval> vval_;  // list of values
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Table constraint.
 * 
 * Example: x  y  z
 *          0  1  2
 *          3  4  5
 */
class TableCtr : public ConstraintRep {
public:
   /** @brief Constructor of an empty constraint */
   TableCtr();

   /// Constructor
   /// @param l list of columns of this
   TableCtr(const std::initializer_list<TableCtrCol>& l);

   /**
    * @brief Constructor
    * @param vars a list of variables
    * @param values list of values representing a row oriented matrix
    */
   TableCtr(const std::initializer_list<Variable>& vars,
            const std::initializer_list<Interval>& values);   

   /** @return the number of columns (variables) */
   size_t nbCols() const;

   /** @return the number of rows (assignments) */
   size_t nbRows() const;

   /**
    * @brief Gets a variable
    * @param j a column index between 0 and nbCols()
    * @return the j-th variable
    */
   Variable getVar(size_t j) const;

   /**
    * @brief Gets a value
    * @param i a row index between 0 and nbRows()-1
    * @param j a column index between 0 and nbCols()-1
    * @return the value at position (i, j)
    */
   Interval getVal(size_t i, size_t j) const;

   /**
    * Gets a column
    * @param j a column index between 0 and nbCols()-1
    * @return the j-th column of this
    */
   TableCtrCol getCol(size_t j) const;

   /**
    * Inserts a column at the end
    * @param col column added to this
    */
   void addCol(const TableCtrCol& col);

   bool isConstant() const override;
   bool isInteger() const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   void acceptVisitor(ConstraintVisitor& vis) const override;
   ConstraintRep* cloneRoot() const override;

private:
   std::vector<TableCtrCol> vcol_; // vector of columns

   void makeScopeAndHashCode();
   bool isRowConsistent(size_t i, const IntervalBox& B) const;
   double rowViolation(const IntervalBox& B, size_t i);
};

/**
 * @brief Generator
 * @param vars a list of variables
 * @param values a list of values representing the list of tuples assigned
 *        to the variables, i.e. we have the first tuple in the list,
 *        followed by the second tuple, and so on.
 * @return the constraint
 */
Constraint table(const std::initializer_list<Variable>& vars,
                 const std::initializer_list<Interval>& values);

/**
 * @brief Generator
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
 * @brief Conditional constraint guard -> body
 * 
 * The guard must be an integer constraint or an inequality constraint, i.e.
 * a constraint that can be certainly satisfied in a box. The body is any
 * constraint.
 */
class CondCtr : public ConstraintRep {
public:
   /**
    * @brief Constructor
    * @param guard the guard
    * @param body the bofy
    */
   CondCtr(Constraint guard, Constraint body);

   /** @brief Default copy constructor */
   CondCtr(CondCtr&) = default;

   /** @brief No assignment */
   CondCtr& operator=(CondCtr&) = delete;

   /** @brief Defaut destructor */
   ~CondCtr() = default;

   /** @return the guard */
   Constraint guard() const;

   /** @return the body */
   Constraint body() const;

   bool isConstant() const override;
   bool isInteger() const override;
   Proof isSatisfied(const IntervalBox& B) override;
   double violation(const IntervalBox& B) override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   void acceptVisitor(ConstraintVisitor& vis) const override;
   ConstraintRep* cloneRoot() const override;

private:
   Constraint guard_;   // guard
   Constraint body_;    // body
};

/**
 * @brief Generator
 * @param guard left-hand constraint
 * @param body right-hand constraint
 * @return the constraint guard -> body
 */
Constraint cond(Constraint guard, Constraint body);

/*----------------------------------------------------------------------------*/

/** @brief Base class of visitors of constraint representations */
class ConstraintVisitor {
public:
   /** @brief Virtual destructor */
   virtual ~ConstraintVisitor();

   /** @name Visit methods */
   /** @{ */
   virtual void apply(const ArithCtrEq* c);
   virtual void apply(const ArithCtrLe* c);
   virtual void apply(const ArithCtrLt* c);
   virtual void apply(const ArithCtrGe* c);
   virtual void apply(const ArithCtrGt* c);
   virtual void apply(const ArithCtrIn* c);
   virtual void apply(const TableCtr* c);
   virtual void apply(const CondCtr* c);
   /** @} */
};

} // namespace

#endif
