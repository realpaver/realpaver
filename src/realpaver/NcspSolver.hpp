///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SOLVER_HPP
#define REALPAVER_NCSP_SOLVER_HPP

#include "realpaver/NcspSpace.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a solver for Numerical CSPs.
///
/// A solver implements a branch-and-prune algorithm prameterized by several
/// components
///////////////////////////////////////////////////////////////////////////////
class NcspSolver {
public:
   /// Creates a solver
   /// @param problem a numerical constraint satisfaction problem
   NcspSolver(Problem& problem);

   /// Destructor
   ~NcspSolver();

   /// No copy constructor
   NcspSolver(const NcspSolver&) = delete;

   /// No assignment
   NcspSolver& operator=(const NcspSolver&) = delete;

   /// Solving method
   /// @return true if a solution is found, false otherwise
   bool solve();

private:
   Problem problem_;    // initial problem
   Problem preprob_;    // problem resulting from preprocessing
   NcspSpace* space_;   // search tree

   Timer ptimer_;    // timer for the preprocessing phase
   Timer stimer_;    // timer for the solving phase

   bool preprocess();
   void branchAndPrune();
};

} // namespace

#endif
