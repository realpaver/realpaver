// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PROBLEM_HPP
#define REALPAVER_PROBLEM_HPP

#include <vector>
#include "realpaver/IntervalVector.hpp"
#include "realpaver/constraint.hpp"
#include "realpaver/obj.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of constrained problems.
 *****************************************************************************/
class Problem {
public:
   // constructor
   Problem(const std::string& name = "");

   // copy protection
   Problem(const Problem&) = delete;
   Problem& operator=(const Problem&) = delete;

   // destructor
   ~Problem() = default;

   // creation of new boolean variable
   Variable addBoolVar(const std::string& name = "");

   // creation of new integer variable
   Variable addIntVar(const int& a, const int& b,
                      const std::string& name = "");

   // creation of new real variable
   Variable addRealVar(const double& a, const double& b,
                       const std::string& name = "");
   Variable addRealVar(const Interval& x, const std::string& name = "");

   // access to the variables
   size_t nbVars() const;
   Variable varAt(size_t i) const;

   // insertion of a constraint
   void addCtr(const Constraint& c);

   // insertion of an objective function
   void addObj(const Obj& obj);

   // access to the objective function
   Obj obj() const;

   // access to the constraints
   size_t nbCtrs() const;
   Constraint ctrAt(size_t i) const;

   // returns the vector of variable domains such that the domain of the i-th
   // variable (i is its identifier) is at position i in the output vector
   IntervalVector getDomains() const;

   // returns the scope of this problem
   Scope scope() const;

   // returns true if v is defined as a variable of this problem but
   // it does not occur anywhere
   bool isFakeVar(const Variable& v) const;

   // simplifies this problem by removing the fixed variables 
   // and the inactive constraints
   // creates another problem with a new set of variables
   // returns false if this problem is proved to be unsatisfiable
   bool preprocess(const IntervalVector& X, Problem& other);
   bool preprocess(Problem& other);

   // tests the variables
   bool isContinuous() const;
   bool isDiscrete() const;
   bool isMixed() const;

   // tests the constraints
   bool isConstrained() const;
   bool isLinConstrained() const;
   bool isEqConstrained() const;
   bool isIneqConstrained() const;
   bool isMixedConstrained() const;

   // tests the objective
   bool hasObjective() const;
   bool isLinObjective() const;

private:
   std::string name_;               // name
   std::vector<Variable> vars_;     // vector of variables
   std::vector<Constraint> ctrs_;   // vector of constraints
   Obj obj_;                        // objective function
   Scope scope_;                    // set of variables
};

std::ostream& operator<<(std::ostream& os, const Problem& p);

} // namespace

#endif
