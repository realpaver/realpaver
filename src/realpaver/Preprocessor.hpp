///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
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

   /// Default destructor
   ~Preprocessor() = default;

   /// Creates a simplified problem from an initial problem
   /// @param src initial problem
   /// @param dest simplified problem
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   ///
   /// It is possible to check if the input problem is notfeasible with
   /// the method isProblemUnfeasible().
   void apply(const Problem& src, Problem& dest);

   /// Creates a simplified problem from an initial problem
   /// @param src initial problem
   /// @param reg domains of variables
   /// @param dest simplified problem
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   ///
   /// It is possible to check if the input problem is notfeasible with
   /// the method isProblemUnfeasible().
   void apply(const Problem& src, IntervalRegion& reg, Problem& dest);

   /// Tests if a variable of the source problem is fixed
   /// @param v a variable of the source problem
   /// @return true if the domain of 'v' is fixed
   bool hasFixedDomain(Variable v) const;

   /// Gets the domain of a fixed variable
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

   /// @return the fixed variables of the source problem
   Scope unfixedScope() const;

   /// @return the number of inactive constraints detected by this
   size_t nbInactiveCtrs() const;

   /// @return the number of fixed variables detected by this
   size_t nbFixedVars() const;

   /// @return the domains of the fixed variables if nbFixedVars() > 0
   IntervalRegion fixedRegion() const;

   /// @return true if the problem preprocessed is not feasible
   bool isUnfeasible() const;

   /// @return true if the input is solved by this
   bool isSolved() const;

   /// @return elapsed time of the preprocessing in seconds
   double elapsedTime() const;

private:
   VarVarMapType vvm_;                 // map for non fixed variables
   VarIntervalMapType vim_;            // map for fixed variables

   std::vector<Constraint> active_;    // constraints not inactive
   size_t nbc_;                        // number of inactive constraints
   bool unfeasible_;                   // true if problem unfeasible

   Timer timer_;

   bool occursInActiveConstraint(const Variable& v) const;
   bool propagate(const Problem& problem, IntervalRegion& reg);
   void applyImpl(const Problem& src, IntervalRegion& reg, Problem& dest);
};

} // namespace

#endif
