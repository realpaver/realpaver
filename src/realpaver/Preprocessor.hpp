///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_PREPROCESSOR_HPP
#define REALPAVER_PREPROCESSOR_HPP

#include <unordered_set>
#include <vector>
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a preprocessor of problems.
///
/// It simplifies a problem by removing the fixed variables and the inactive
/// constraints. A new problem is created with a new set of variables.
///////////////////////////////////////////////////////////////////////////////
class Preprocessor {
public:
   /// Type of maps Variable -> Variable
   typedef ConstraintFixer::VarVarMapType VarVarMapType;

   /// Type of maps Variable -> Interval
   typedef ConstraintFixer::VarIntervalMapType VarIntervalMapType;

   /// Cnstructor
   Preprocessor();

   /// No copy
   Preprocessor(const Preprocessor&) = delete;

   /// No assignment
   Preprocessor& operator=(const Preprocessor&) = delete;

   /// Destructor
   ~Preprocessor();

   /// Creates a simplified problem from an initial problem
   /// @param src initial problem
   /// @param dest simplified problem
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   ///
   /// It is possible to check if the input problem is not feasible with
   /// the method isProblemUnfeasible().
   void apply(const Problem& src, Problem& dest);

   /// Tests if a variable of the source problem is fixed
   /// @param v a variable of the source problem
   /// @return true if the domain of 'v' is fixed
   bool hasFixedDomain(Variable v) const;

   /// Gets the domain of a fixed variable of the source problem
   /// @param v a variable of the source problem
   /// @return the domain of v
   Interval getFixedDomain(Variable v) const;

   /// Gets a variable of the destination problem that is associated
   /// to a variable of the source problem
   /// @param v a variable of the source problem
   /// @return the variable of the destination problem associated with 'v'
   Variable srcToDestVar(Variable v) const;

   /// @return true if all the variables from the source problem are fixed
   bool allVarsFixed() const;

   /// @return the fixed variables of the source problem
   Scope fixedScope() const;

   /// @return the unfixed variables of the source problem
   Scope unfixedScope() const;

   /// @return the (unfixed) variables of the destination problem
   ///
   /// It corresponds to unfixedScope() mapped to the variables of the
   /// destination problem
   Scope destScope() const;

   /// @return the number of inactive constraints detected by this
   size_t nbInactiveCtrs() const;

   /// Gets an inactive constraint
   /// @param i an index between 0 and nbInactiveCtrs()-1
   /// @return the i-th inactive constraint in this
   Constraint getInactiveCtr(size_t i) const;

   /// @return the number of fixed variables detected by this
   size_t nbFixedVars() const;

   /// @return the number of unfixed variables after preprocessing
   size_t nbUnfixedVars() const;

   /// Gets a fixed variable of the source problem
   /// @param i an integer between 0 and nbFixedVars() - 1
   /// @return the i-th fixed variable in this
   Variable getFixedVar(size_t i) const;

   /// Gets an unfixed variable of the source problem
   /// @param i an integer between 0 and nbUnfixedVars() - 1
   /// @return the i-th unfixed variable in this
   Variable getUnfixedVar(size_t i) const;

   /// @return the domains of the fixed variables if nbFixedVars() > 0
   ///
   /// The scope of this region is equal to fixedScope() having
   /// only variables of the source problem.
   IntervalBox fixedRegion() const;

   /// @return the domains of the non fixed variables
   ///
   /// The scope of this region is the scope of the destination problem.
   IntervalBox destRegion() const;

   /// @return true if the problem preprocessed is not feasible
   bool isUnfeasible() const;

   /// @return true if the input is solved by this
   bool isSolved() const;

   /// @return elapsed time of the preprocessing stage in seconds
   double elapsedTime() const;

private:
   VarVarMapType vvm_;                 // map for non fixed variables
   VarIntervalMapType vim_;            // map for fixed variables

   IntervalBox* box_;                  // box used for propagation

   std::vector<Constraint> inactive_;  // constraints inactive
   std::vector<Constraint> active_;    // constraints not inactive
   bool unfeasible_;                   // true if problem unfeasible

   Timer timer_;

   bool occursInActiveConstraint(const Variable& v) const;
   bool propagate(const Problem& problem, IntervalBox& box);
   void applyImpl(const Problem& src, Problem& dest);
};

} // namespace

#endif
