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
   /// Default constructor
   Preprocessor() = default;

   /// No copy
   Preprocessor(const Preprocessor&) = delete;

   /// No assignment
   Preprocessor& operator=(const Preprocessor&) = delete;

   /// Default destructor
   ~Preprocessor() = default;

   /// Crestes a simplified problem from an initial problem
   /// @param src initial problem
   /// @param dest simplified problem
   /// @return false if the problem is proved to be unsatisfiable
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   bool apply(const Problem& src, Problem& dest);

   /// Crestes a simplified problem from an initial problem
   /// @param src initial problem
   /// @param X the variable domains
   /// @param dest simplified problem
   /// @return false if the problem given X is proved to be unsatisfiable
   ///
   /// It removes the fixed variables and the inactive constraints and it
   /// creates a new problem with a new set of variables.
   bool apply(const Problem& src, const IntervalVector& X, Problem& dest);


private:
   typedef ConstraintFixer::VarVarMapType VarVarMapType;
   typedef ConstraintFixer::VarIntervalMapType VarIntervalMapType;

   VarVarMapType vvm_;
   VarIntervalMapType vim_;
};

} // namespace

#endif
