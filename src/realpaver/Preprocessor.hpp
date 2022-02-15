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
   /// @param X the variable domains
   /// @param dest simplified problem
   /// @return false if the problem given X is proved to be unsatisfiable
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   bool apply(const Problem& src, const IntervalVector& X, Problem& dest);

   /// Tests if a variable of the source problem is fixed
   /// @param v a variable of the source problem
   /// @return true if the domain of 'v' is fixed
   bool hasFixedDomain(const Variable& v) const;

   /// @return true if all the variables of the source problem are fixed
   bool areAllVarFixed() const;

   /// Gets the domain of a fixed variable
   /// @param v a variable
   /// @return the domain of v
   Interval getFixedDomain(const Variable& v) const;

   /// Gets a variable of the destination problem that is associated
   /// to a variable of the source problem
   /// @param v a variable of the source problem
   /// @return the variable of the destination problem associated with 'v'
   Variable srcToDestVar(Variable v) const;

   /// @return the number of variables fixed in the last problem preprocessed
   size_t getNbVarFixed() const;

   /// @return the number of constraints removed from the last problem
   ///         preprocessed
   size_t getNbCtrRemoved() const;

private:
   VarVarMapType vvm_;
   VarIntervalMapType vim_;

   size_t nbv_;   // number of variables fixed
   size_t nbc_;   // number of variables removed
};

} // namespace

#endif
