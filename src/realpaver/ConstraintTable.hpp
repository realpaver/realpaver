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
 * @file   ConstraintTable.hpp
 * @brief  Table constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#ifndef REALPAVER_CONSTRAINT_TABLE_HPP
#define REALPAVER_CONSTRAINT_TABLE_HPP

#include "realpaver/Constraint.hpp"

namespace realpaver {

/**
 * @brief Column of a table constraint.
 *
 * A column is a variable with a list of values (assignments).
 */
class ConstraintTableCol {
public:
   /// Constructor v in {}
   ConstraintTableCol(Variable v);

   /// Constructor v in l
   ConstraintTableCol(Variable v, const std::initializer_list<Interval> &l);

   /// Returns the number of values of the variable
   size_t size() const;

   /// Gets the i-th value
   Interval getVal(size_t i) const;

   /// Returns the variable
   Variable getVar() const;

   /// Adds a value at the end
   void addValue(const Interval &x);

   /// Returns true if this is an integer variable with integer values
   bool isInteger() const;

   /// Removes the i-th value
   void removeVal(size_t i);

private:
   Variable v_;                 // variable
   std::vector<Interval> vval_; // list of values
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Representation of a table constraint.
 *
 * Example: x  y  z
 *          0  1  2
 *          3  4  5
 */
class ConstraintTable : public ConstraintRep {
public:
   /// Creates an empty constraint
   ConstraintTable();

   /// Constructor given a list of columns
   ConstraintTable(const std::initializer_list<ConstraintTableCol> &l);

   /**
    * @brief Constructor.
    *
    * Input: a list of variables vars and a list of values representing a
    * row-oriented matrix.
    */
   ConstraintTable(const std::initializer_list<Variable> &vars,
                   const std::initializer_list<Interval> &values);

   /// Returns the number of columns (variables)
   size_t nbCols() const;

   /// Returns the number of rows (assignments)
   size_t nbRows() const;

   /// Gets the j-th variable (0 <= j < nbCols)
   Variable getVar(size_t j) const;

   /// Gets the value at position (i, j)
   Interval getVal(size_t i, size_t j) const;

   /// Gets the j-th column
   ConstraintTableCol getCol(size_t j) const;

   /// Inserts a column at the end
   void addCol(const ConstraintTableCol &col);

   /// Removes redundant rows
   void removeRedundantRows();

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
   std::vector<ConstraintTableCol> vcol_; // vector of columns

   void makeScopeAndHashCode();
   bool isRowConsistent(size_t i, const DomainBox &box) const;
   bool isRowConsistent(size_t i, const IntervalBox &B) const;
   double rowViolation(const DomainBox &box, size_t i);
   double rowViolation(const IntervalBox &B, size_t i);
   bool areSameRows(size_t i, size_t j) const;
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
Constraint table(const std::initializer_list<Variable> &vars,
                 const std::initializer_list<Interval> &values);

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
Constraint table(const Variable *vars, size_t nvars, const Interval *values,
                 size_t nvalues);

} // namespace realpaver

#endif
