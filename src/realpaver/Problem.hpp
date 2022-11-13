///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PROBLEM_HPP
#define REALPAVER_PROBLEM_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Constraint.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/Objective.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a problem involving constraints.
///
/// A problem in general has a set of variables, a set of constraints and an
/// objective function. The variables are indexed by the natural numbers
/// 0, 1, 2, ... The variable domains define the initial region, which is
/// an interval vector or Cartesian product of intervals with a scope.
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

   /// Creates a new variable with domain {0, 1}
   /// @param name name of the variable
   /// @return the new variable
   Variable addBinaryVar(const std::string& name = "");

   /// Creates a new integer variable
   /// @param lo lower bound
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addIntVar(int lo, int up, const std::string& name = "");

   /// Creates a new integer variable
   /// @param x variable domain
   /// @param up  bound
   /// @param name name of the variable
   /// @return the new variable
   Variable addIntVar(const Interval& x, const std::string& name = "");

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

   /// Gets the initial region
   /// @return the vector of domains
   ///
   /// The i-th component of the vector is th domain of the i-th variable
   /// for each i;.
   IntervalRegion getDomains() const;

   /// Gets the domain of a variable
   /// @param v a variable
   /// @return the domain of v
   Interval getDomain(Variable v) const;

   /// Sets the initial region
   /// @param reg domains of variables
   void setDomains(const IntervalRegion& reg);

   /// Sets a domain in the initial region
   /// @param v variable
   /// @param x new initial domain of v
   void setDomain(Variable v, const Interval& x);

   /// Sets a domain in the initial region
   /// @param v variable
   /// @param lo lower bound
   /// @param up upper bound
   void setDomain(Variable v, double lo, double up);

   /// @return true if this is a CSP
   bool isCSP() const;

   /// @return true if this is a BOP
   bool isBOP() const;

   /// @return true if this is a COP
   bool isCOP() const;

   /// @return true if this has only continuous variables
   bool isContinuous() const;

   /// @return true if this has only discrete variables
   bool isDiscrete() const;

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

private:
   std::string name_;               // name
   std::vector<Variable> vars_;     // vector of variables
   std::vector<Constraint> ctrs_;   // vector of constraints
   Objective obj_;                  // objective function
   Scope scope_;                    // set of variables

   std::unordered_set<std::string> vname_;

   std::unordered_map<Variable, Interval, VariableHasher> dom_;

   // throws an exception if a name is already used as the name of a symbol
   void checkSymbol(const std::string& name);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Problem& p);

} // namespace

#endif
