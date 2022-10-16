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

#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/Env.hpp"
#include "realpaver/NcspEnv.hpp"
#include "realpaver/NcspSpace.hpp"
#include "realpaver/NcspSplit.hpp"
#include "realpaver/Preprocessor.hpp"
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
   NcspSolver(const Problem& problem);

   /// Destructor
   ~NcspSolver();

   /// No copy constructor
   NcspSolver(const NcspSolver&) = delete;

   /// No assignment
   NcspSolver& operator=(const NcspSolver&) = delete;

   /// Assigns the contractor of this
   /// @param contractor new contractor
   void setContractor(SharedContractor contractor);

   /// Solving method
   void solve();

   /// @return the preprocessing time in seconds
   double getPreprocessingTime() const;

   /// @return the solving time in seconds
   double getSolvingTime() const;

   /// @return the number of nodes processed
   int getTotalNodes() const;

   /// @return the environment of this
   NcspEnv* getEnv() const;

   /// @return the space of this
   NcspSpace* getSpace() const;

private:
   Problem problem_;             // initial problem
   Problem preprob_;             // problem resulting from preprocessing
   Preprocessor preproc_;        // preprocessor
   
   NcspEnv* env_;                // environment
   NcspSpace* space_;            // search tree
   SharedDag dag_;               // dag
   SharedContractor contractor_; // contraction operator
   NcspSplit* split_;            // splitting strategy

   Timer ptimer_;                // timer for the preprocessing phase
   Timer stimer_;                // timer for the solving phase

   int nbnodes_;                 // number of nodes processed

   void preprocess();
   void branchAndPrune();
   void makeSpace();
   void makeContractor();
   void makeSplit();
   void bpStep(int depthlimit);
   bool isAnInnerRegion(const IntervalRegion& reg) const;
};

} // namespace

#endif
