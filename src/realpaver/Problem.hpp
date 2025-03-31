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
 * @file   Problem.hpp
 * @brief  Constraint satisfaction and optimization problem
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_PROBLEM_HPP
#define REALPAVER_PROBLEM_HPP

#include "realpaver/Alias.hpp"
#include "realpaver/BoxReporter.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/Objective.hpp"
#include "realpaver/VariableVector.hpp"
#include <unordered_set>
#include <vector>

namespace realpaver {

/**
 * @brief Constraint satisfaction and optimization problem.
 *
 * A problem in general has a set of variables, a set of constraints, an
 * objective function and a set of aliases.
 *
 * The variables are indexed by consecutive natural numbers 0, 1, ... that are
 * automatically generated.
 *
 * The Cartesian product of variable domains can be simply obtained by creating
 * a domain box from the scope of this.
 *
 * Variables and aliases can be reported or not when a solution is displayed
 * or not after a solving process. To this end, a problem manages a vector
 * of reported entities. The defaulkt strategy is to consider every alias or
 * variable as reported. The methods reportAlias and reportVariable can be
 * used to change this behaviour.
 *
 * There are several classes of problems:
 * - Constraint Satisfaction Problems (CSPs);
 * - Bound-constrained Optimization Problems (BOPs);
 * - Constrained Optimization Problems (COPs).
 */
class Problem {
public:
   /// Constructor
   Problem(const std::string &name = "");

   /// Default copy constructor
   Problem(const Problem &) = default;

   /// No assignment
   Problem &operator=(const Problem &) = delete;

   /// Default destructor
   ~Problem() = default;

   /// Creates a new variable with no domain yet
   Variable addVar(const std::string &name = "");

   /// Creates a new variable with domain {0, 1}
   Variable addBinaryVar(const std::string &name = "");

   /**
    * @brief Creates a vector of binary variables.
    *
    * Indexes in the vector: first, first+1, ..., last.
    */
   VariableVector addBinaryVarVector(const std::string &name, int first, int last);

   /// Creates a new integer variable with domain [lo, up]
   Variable addIntVar(int lo, int up, const std::string &name = "");

   /// Creates a new integer variable with domain r
   Variable addIntVar(const Range &r, const std::string &name = "");

   /// Creates a new integer variable with domain u
   Variable addIntVar(const RangeUnion &u, const std::string &name = "");

   /**
    * @brief Creates a vector of integer variables with domain r.
    *
    * Indexes in the vector: first, first+1, ..., last.
    */
   VariableVector addIntVarVector(const std::string &name, int first, int last,
                                  const Range &r = Range::universe());

   /// Creates a new real variable with domain [lo,up]
   Variable addRealVar(double lo, double up, const std::string &name = "");

   /// Creates a new real variable with domain x
   Variable addRealVar(const Interval &x, const std::string &name = "");

   /// Creates a new real variable with domain u
   Variable addRealVar(const IntervalUnion &u, const std::string &name = "");

   /**
    * @brief Creates a vector of real variables with domains x.
    *
    * Indexes in the vector: first, first+1, ..., last.
    */
   VariableVector addRealVarVector(const std::string &name, int first, int last,
                                   const Interval &x = Interval::universe());

   /// Creates a new real variable by cloning
   Variable addClonedVar(Variable v);

   /// Returns the number of variables
   size_t nbVars() const;

   /// Gets the i-th variable of this
   Variable varAt(size_t i) const;

   /// Returns the last variable created in this
   Variable lastVar() const;

   /// Checks if a variable is involved in this but it does not occur anywhere
   bool isFakeVar(Variable v) const;

   /// Returns the scope of this (sorted set of variables)
   Scope scope() const;

   /// Inserts a constraint
   void addCtr(Constraint c);

   /// Inserts a list of constraints
   void addCtr(const std::initializer_list<Constraint> &l);

   /// Adds an objective function
   void addObjective(Objective obj);

   /// Returns the objective function
   Objective getObjective() const;

   /// Returns the number of constraints
   size_t nbCtrs() const;

   /// Gets the i-th constraint of this
   Constraint ctrAt(size_t i) const;

   /// Returns true if this is a CSP
   bool isCSP() const;

   /// Returns true if this is a BOP
   bool isBOP() const;

   /// Returns true if this is a COP
   bool isCOP() const;

   /// Returns true if this has only real variables
   bool isReal() const;

   /// Returns true if this has only integer (or binary) variables
   bool isInteger() const;

   /// Returns true if this has continuous and discrete variables
   bool isMixed() const;

   /// Returns true if this has at least one constraint
   bool isConstrained() const;

   /**
    * @brief Returns true if this has at least one constraint and its
    * constraints are linear.
    */
   bool isLinConstrained() const;

   /**
    * @brief Returns true if this has at least one constraint and its
    * constraints are equations.
    */
   bool isEqConstrained() const;

   /**
    * @brief Returns true if this has at least one constraint and its
    * constraints are inequality constraints.
    */
   bool isIneqConstrained() const;

   /**
    * @brief Returns true if this has at least one equation and one inequality
    * constraint.
    */
   bool isMixedConstrained() const;

   /// Returns true if this has a non constant objective function
   bool hasObjective() const;

   /// Returns true if this has a non constant and linear objective function
   bool isLinObjective() const;

   /// Returns true if this has no variable, no constraint, no objective
   bool isEmpty() const;

   /// Adds an alias in this
   void addAlias(const Alias &a);

   /// Returns the number of aliases
   size_t nbAliases() const;

   /// Access to an alias given its position between 0 and nbAliases()-1
   Alias aliasAt(size_t i) const;

   /// Makes a variable a reported entity if b is true
   void reportVariable(Variable v, bool b = true);

   /// Makes an alias a reported entity if b is true
   void reportAlias(Alias a, bool b = true);

   /// Returns true if v is reported, false otherwise
   bool isVarReported(const Variable &v) const;

   /// Returns true if a is reported, false otherwise
   bool isAliasReported(const Alias &a) const;

private:
   std::string name_;             // name
   std::vector<Variable> vars_;   // vector of variables
   std::vector<Constraint> ctrs_; // vector of constraints
   Objective obj_;                // objective function
   Scope scop_;                   // set of variables
   std::vector<Alias> als_;       // vector of aliases

   // set of symbols used in this problem
   std::unordered_set<std::string> vname_;

   // reported entities (aliases and variables)
   std::shared_ptr<EntityReportedVector> erv_;

   // throws an exception if a name is already used as the name of a symbol
   void checkSymbol(const std::string &name);

   static int NP; // problem counter
   int id_;       // problem id

   size_t nextVarId() const;

   // Adds v in the vector of variables and the scope of this
   void pushVar(Variable v);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Problem &p);

} // namespace realpaver

#endif
