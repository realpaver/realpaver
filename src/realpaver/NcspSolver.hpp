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
#include "realpaver/NcspSpace.hpp"
#include "realpaver/NcspSplit.hpp"
#include "realpaver/Param.hpp"
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

   /// Assigns the parameters in this
   /// @param prm parameters
   void setParam(const Param& prm);

   /// @return the object that manages the parameter settings
   Param* getParam() const;

   /// Assigns the contractor of this
   /// @param contractor new contractor
   void setContractor(const SharedContractor& contractor);

   /// Solving method
   /// @return true if a solution is found, false otherwise
   bool solve();

   /// @return the preprocessing time in seconds
   double getPreprocessingTime() const;

   /// @return the solving time in seconds
   double getSolvingTime() const;

   /// @return the number of nodes processed
   int getTotalNodes() const;

   /// @return the number of nodes that remains in the search tree
   int getNbPendingNodes() const;

   /// @return the number of solutions found
   int getNbSolutions() const;

   /// Access to a solution
   /// @param i index of solution between 0 and getNbSolutions()-1
   /// @return the i-th solution
   IntervalRegion getSolution(size_t i) const;

   /// @return true if the branch-and-prune algorithm implemented a complete
   ///         search, false otherwise (i.e. a limit has been reached)
   bool proofComplete() const;

   /// @return true if a feasible or inner region has been found, false
   ///         otherwise
   bool proofFeasible() const;

   /// @return true if the time limit is exceeded
   bool overTimeLimit() const;

   /// @return true if the node limit is exceeded
   bool overNodeLimit() const;

   /// @return true if the solution limit is exceeded
   bool overSolutionLimit() const;

private:
   Problem problem_;             // initial problem
   Problem preprob_;             // problem resulting from preprocessing
   Param* param_;                // parameters

   NcspSpace* space_;            // search tree
   SharedDag dag_;               // dag
   SharedContractor contractor_; // contraction operator
   NcspSplit* split_;            // splitting strategy

   Timer ptimer_;                // timer for the preprocessing phase
   Timer stimer_;                // timer for the solving phase

   int nbnodes_;                 // number of nodes processed
   bool isComplete_;             // true if the search is complete
   bool tlim_;                   // true if time limit exceeded
   bool nlim_;                   // true if node limit exceeded
   bool slim_;                   // true if node limit exceeded

   bool preprocess();
   bool branchAndPrune();
   void makeSpace();
   void makeContractor();
   void makeSplit();
   void bpStep(int depthlimit);
   bool isAnInnerRegion(const IntervalRegion& reg) const;
};

} // namespace

#endif
