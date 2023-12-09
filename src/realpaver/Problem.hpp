///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PROBLEM_HPP
#define REALPAVER_PROBLEM_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Constraint.hpp"
#include "realpaver/Objective.hpp"
#include "realpaver/SearchRegion.hpp"
#include "realpaver/VariableVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a problem involving constraints.
///
/// A problem in general has a set of variables, a set of constraints and an
/// objective function.
///
/// The variables are indexed by consecutive natural numbers that are
/// automatically generated. Given NP the number of problem instances already
/// created and the static variable MAX_NB_VAR initialized to a huge value
/// (one million), the first variable identifier is NP*MAX_NB_VAR.
///
/// The Cartesian product of variable domains can be simply obtained by
/// creating a SearchRegion from the scope of this.
///
/// There are several classes of problems:
/// - Constraint Satisfaction Problems (CSPs);
/// - ...
///////////////////////////////////////////////////////////////////////////////
class Problem {
public:
   /// Creates a problem
   /// @param name name of this problem
   Problem(const std::string& name = "");

   /// Default copy constructor
   Problem(const Problem&) = default;

   /// No assignment
   Problem& operator=(const Problem&) = delete;

   /// Default destructor
   ~Problem() = default;

   /// Creates a new variable with no domain yet
   /// @param name name of the variable
   /// @return the new variable
   Variable addVar(const std::string& name = "");

   /// Creates a new variable with domain {0, 1}
   /// @param name name of the variable
   /// @return the new variable
   Variable addBinaryVar(const std::string& name = "");

   /// Creates a vector of binary variables
   /// @param name base name
   /// @param first index of the first variable
   /// @param last index of the last variable
   /// @return the vector created
   VariableVector addBinaryVarVector(const std::string& name, int first, int last);

   /// Creates a new integer variable
   /// @param lo lower bound
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addIntVar(int lo, int up, const std::string& name = "");

   /// Creates a new integer variable
   /// @param r variable domain
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addIntVar(const Range& r, const std::string& name = "");

   /// Creates a new integer variable
   /// @param u variable domain
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addIntVar(const RangeUnion& u, const std::string& name = "");

   /// Creates a vector of integer variables
   /// @param name base name
   /// @param first index of the first variable
   /// @param last index of the last variable
   /// @param r domain of every variable in this
   /// @return the vector created
   VariableVector addIntVarVector(const std::string& name, int first, int last,
                                  const Range& r = Range::universe());

   /// Creates a new real variable
   /// @param lo lower bound
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addRealVar(double lo, double up, const std::string& name = "");

   /// Creates a new real variable
   /// @param x variable domain
   /// @param name name of the variable
   /// @return the new variable
   Variable addRealVar(const Interval& x, const std::string& name = "");

   /// Creates a new real variable
   /// @param u variable domain
   /// @param name name of the variable
   /// @return the new variable
   Variable addRealVar(const IntervalUnion& u, const std::string& name = "");

   /// Creates a vector of integer variables
   /// @param name base name
   /// @param first index of the first variable
   /// @param last index of the last variable
   /// @param x domain of every variable
   /// @return the vector created
   VariableVector addRealVarVector(const std::string& name, int first, int last,
                                   const Interval& x = Interval::universe());

   /// Creates a new real variable by cloning
   /// @param v a variable (from this or another problem)
   /// @return the new variable (with a new identifier) that is a clone of v
   Variable addClonedVar(Variable v);

   /// @return the number of variables
   size_t nbVars() const;

   /// Gets a variable
   /// @param i an index in 0 .. nbVars()-1
   /// @return the i-th variable of this
   Variable varAt(size_t i) const;

   /// @return the last variable created in this
   Variable lastVar() const;

   /// Checks if a variable is involved in this
   /// @param v a variable
   /// @return true if v a variable of this but it does not occur anywhere
   bool isFakeVar(Variable v) const;

   /// @return the scope of this (sorted set of variables)
   Scope scope() const;

   /// Inserts a constraint
   /// @param c constraint inserted
   void addCtr(Constraint c);

   /// Inserts a list of constraints
   /// @param l constraints inserted
   void addCtr(const std::initializer_list<Constraint>& l);

   /// Adds an objective function
   /// @param obj objective function
   void addObjective(Objective obj);

   /// @return the objective function
   Objective getObjective() const;

   /// @return the number of constraints
   size_t nbCtrs() const;

   /// Gets a constraint
   /// @param i an index in 0 .. nbCtrs()-1
   /// @return the i-th constraint of this
   Constraint ctrAt(size_t i) const;

   /// @return true if this is a CSP
   bool isCSP() const;

   /// @return true if this is a BOP
   bool isBOP() const;

   /// @return true if this is a COP
   bool isCOP() const;

   /// @return true if this has only real variables
   bool isReal() const;

   /// @return true if this has only integer (or binary) variables
   bool isInteger() const;

   /// @return true if this has continuous and discrete variables
   bool isMixed() const;

   /// @return true if this has at least one constraint
   bool isConstrained() const;

   /// @return true if this has at least one constraint and its constraints
   ///         are linear
   bool isLinConstrained() const;

   /// @return true if this has at least one constraint and its constraints
   ///         are equations
   bool isEqConstrained() const;

   /// @return true if this has at least one constraint and its constraints
   ///         are inequality constraints
   bool isIneqConstrained() const;

   /// @return true if this has at least one equation and one inequality
   ///         constraint
   bool isMixedConstrained() const;

   /// @return true if this has a non constant objective function
   bool hasObjective() const;

   /// @return true if this has a non constant and linear objective function   
   bool isLinObjective() const;

   /// @return true if this has no variable, no constraint, no objective
   bool isEmpty() const;

   /// Maximum number of variables in a problem
   /// The default value is huge
   static int MAX_NB_VAR;

private:
   std::string name_;               // name
   std::vector<Variable> vars_;     // vector of variables
   std::vector<Constraint> ctrs_;   // vector of constraints
   Objective obj_;                  // objective function
   Scope scope_;                    // set of variables

   // set of symbols used in this problem
   std::unordered_set<std::string> vname_;

   // throws an exception if a name is already used as the name of a symbol
   void checkSymbol(const std::string& name);

   static int NP;    // problem counter
   int id_;          // problem id
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Problem& p);

} // namespace

#endif
