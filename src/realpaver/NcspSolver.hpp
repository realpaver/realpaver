///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
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
#include "realpaver/Prover.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a solver for Numerical CSPs.
///
/// A solver is a two-phase constraint solving algorithm. The first phase is
/// a preprocessing step that tries to reduce the variable domains, to
/// fix and then eliminate some variables, to detect inactive constraints
/// or unfeasible constraints. The second phase is an interval-based
/// branch-and-prune algorithm prameterized by several components.
///
/// The solver takes as input a problem that must be a NCSP. Its components
/// are generated according to a Param object prm that can be assigned as
/// follows:
/// solver.getEnv()->setParam(prm);
///
/// After the solving phase, it is possible to read the solutions as follows:
/// for (size_t i=0; i<solver.getNbSolutions(); ++i) {
///    std::pair<IntervalBox, Proof> sol = solver.getSolution(i);
/// }
/// The scope of each solution is the one of the input problem.
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

   /// @return the solving time in seconds
   double getSolvingTime() const;

   /// @return the number of nodes processed
   int getTotalNodes() const;

   /// @return the environment of this
   NcspEnv* getEnv() const;

   /// @return the space of this
   NcspSpace* getSpace() const;

   /// @return the preprocessor used by this
   Preprocessor* getPreprocessor() const;

   /// @return the number of solutions after the preprocessing / solving phase
   size_t nbSolutions() const;

   /// Gets a solution after the preprocessing / solving phase
   /// @param i an index between 0 and nbSolutions()-1
   /// @return the i-th solution in this
   ///
   /// The scope of the box is the scope of the initial problem, i.e. it must
   /// include the variables fixed at preprocessing time.
   std::pair<DomainBox, Proof> getSolution(size_t i) const;

   /// @return the number of unexplored boxes after the solving phase
   size_t nbPendingBoxes() const;

   /// Gets a pending box after the solving phase
   /// @param i an index between 0 and nbPendingBoxes()-1
   /// @return the i-th pending box in this
   ///
   /// The scope of the box is the scope of the initial problem, i.e. it must
   /// include the variables fixed at preprocessing time.
   DomainBox getPendingBox(size_t i) const;

private:
   Problem* problem_;            // initial problem
   Problem* preprob_;            // problem resulting from preprocessing
   Preprocessor* preproc_;       // preprocessor

   NcspEnv* env_;                // environment
   NcspSpace* space_;            // search tree
   SharedDag dag_;               // dag
   SharedContractor contractor_; // contraction operator
   NcspSplit* split_;            // splitting strategy

   Prover* prover_;              // solution prover

   Timer stimer_;                // timer for the solving phase
   int nbnodes_;                 // number of nodes processed
   bool withPreprocessing_;      // true if preprocessing enabled

   void branchAndPrune();
   void makeSpace();
   void makeContractor();
   void makeSplit();
   void bpStep(int depthlimit);
   bool isAnInnerRegion(const IntervalBox& box) const;
   void certifySolutions();

//   VariableSelector* makeMaxSmearStrategy();
};

} // namespace

#endif
