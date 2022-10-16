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

   /// Default constructor
   Preprocessor() = default;

   /// No copy
   Preprocessor(const Preprocessor&) = delete;

   /// No assignment
   Preprocessor& operator=(const Preprocessor&) = delete;

   /// Default destructor
   ~Preprocessor() = default;

   /// Creates a simplified problem from an initial problem
   /// @param src initial problem
   /// @param dest simplified problem
   /// @return false if the problem is proved to be unsatisfiable
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   bool apply(const Problem& src, Problem& dest);

   /// Creates a simplified problem from an initial problem
   /// @param src initial problem
   /// @param reg domains of variables
   /// @param dest simplified problem
   /// @return false if the problem given X is proved to be unsatisfiable
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   bool apply(const Problem& src, IntervalRegion& reg, Problem& dest);

   /// Tests if a variable of the source problem is fixed
   /// @param v a variable of the source problem
   /// @return true if the domain of 'v' is fixed
   bool hasFixedDomain(Variable v) const;

   /// Tests if a variable of the source problem is fake
   /// @param v a variable of the source problem
   /// @return true if 'v' does not occur in the constraints or the
   ///         objective function
   bool isFake(Variable v) const;

   /// Gets the domain of a fixed variable
   /// @param v a variable of the source problem
   /// @return the domain of v
   Interval getFixedDomain(Variable v) const;

   /// Gets a variable of the destination problem that is associated
   /// to a variable of the source problem
   /// @param v a variable of the source problem
   /// @return the variable of the destination problem associated with 'v'
   Variable srcToDestVar(Variable v) const;

   /// @return true if all the variables from the source problem are
   ///         either fake or fixed
   bool allVarsRemoved() const;

   /// @return the scope of the source problem minus the fake variables
   Scope trueScope() const;

   /// @return the fixed variables of the source problem
   Scope fixedScope() const;

   /// @return the fixed variables of the source problem
   Scope unfixedScope() const;

   /// @return the fake variables of the source problem
   Scope fakeScope() const;

private:
   VarVarMapType vvm_;                 // map for non fixed variables
   VarIntervalMapType vim_;            // map for fixed variables
   VarIntervalMapType fake_;           // map for fake variables

   std::vector<Constraint> active_;    // constraints not removed
   size_t nbc_;                        // number of constraints removed

   bool occursInActiveConstraint(const Variable& v) const;
   bool propagate(const Problem& problem, IntervalRegion& reg);
};

} // namespace

#endif
