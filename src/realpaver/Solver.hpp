/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   Solver.hpp
 * @brief  Solver of constraint systems
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_SOLVER_HPP
#define REALPAVER_SOLVER_HPP

#include "realpaver/ContractorFactory.hpp"
#include "realpaver/EnvSolver.hpp"
#include "realpaver/NodePropagator.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Prover.hpp"
#include "realpaver/Space.hpp"
#include "realpaver/Split.hpp"
#include "realpaver/Timer.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Solver for constraint systems.
 *
 * A solver is a two-phase constraint solving algorithm. The first phase is
 * a preprocessing step that tries to reduce the variable domains, to
 * fix and then eliminate some variables, to detect inactive constraints
 * or infeasible constraints. The second phase is an interval-based
 * branch-and-prune algorithm parameterized by several components.
 *
 * The solver takes as input a problem. Its components are generated according to
 * a Param object prm that can be assigned as follows:
 *
   @verbatim
   solver.getEnv()->setParam(prm);
   @endverbatim
 *
 * After the solving phase, it is possible to read the solutions as follows:
 *
   @verbatim
   for (size_t i=0; i<solver.nbSolutions(); ++i) {
      std::pair<DomainBox, Proof> sol = solver.getSolution(i);
   }
   @endverbatim
 *
 * The scope of each solution is the one of the input problem.
 */
class Solver {
public:
   /// Constructor
   Solver(const Problem &problem);

   /// Default estructor
   ~Solver() = default;

   /// No copy constructor
   Solver(const Solver &) = delete;

   /// No assignment
   Solver &operator=(const Solver &) = delete;

   /// Solving method
   void solve();

   /// Returns the solving time in seconds
   double getSolvingTime() const;

   /// Returns the number of nodes processed
   int getTotalNodes() const;

   /// Returns the environment of this
   std::shared_ptr<EnvSolver> getEnv() const;

   /// Returns the space of this
   std::shared_ptr<Space<Node>> getSpace() const;

   /// Returns the preprocessor used by this
   SharedPreprocessor getPreprocessor() const;

   /// Returns the number of solutions after the preprocessing / solving phase
   size_t nbSolutions() const;

   /**
    * @brief Gets the i-th solution after the preprocessing / solving phase.
    *
    * The scope of the box is the scope of the initial problem, i.e. it must
    * include the variables fixed at preprocessing time.
    */
   std::pair<DomainBox, Proof> getSolution(size_t i) const;

   /// Returns the number of unexplored nodes after the solving phase
   size_t nbPendingNodes() const;

   /**
    * @brief Gets the i-th pending box after the solving phase.
    *
    * The scope of the box is the scope of the initial problem, i.e. it must
    * include the variables fixed at preprocessing time.
    */
   DomainBox getPendingBox(size_t i) const;

private:
   SharedProblem problem_;                      // copy of initial problem
   SharedPreprocessor pp_;                      // preprocessor
   std::unique_ptr<NodeContext> context_;       // solving context for the BP algorithm
   std::shared_ptr<EnvSolver> env_;             // environment
   std::shared_ptr<Space<Node>> space_;         // search tree
   SharedDag dag_;                              // dag
   std::unique_ptr<NodePropagator> propagator_; // contraction method
   std::unique_ptr<Split> split_;               // splitting strategy
   std::unique_ptr<ContractorFactory> factory_; // contractor factory
   std::unique_ptr<Prover> prover_;             // solution prover
   Timer stimer_;                               // timer for the solving phase
   int nbnodes_;                                // number of nodes processed
   bool withPreprocessing_;                     // true if preprocessing enabled

   void branchAndPrune();
   void makeSpace();
   void makePropagator();
   void makeSplit();
   void bpStep(int depthlimit, bool splitOff);
   void bpStepAux(SharedNode node, int depthlimit, bool splitOff);
   void certifySolutions();
   Proof checkActivity(Node &node);
};

} // namespace realpaver

#endif
