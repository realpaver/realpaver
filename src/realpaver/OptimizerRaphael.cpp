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
 * @file   Optimizer.hpp
 * @brief  Branch-and-bound algorithm
 * @author Raphaël Chenouard
 * @date   21 August 2025
 */

#ifndef REALPAVER_OPTIMIZER_HPP
#define REALPAVER_OPTIMIZER_HPP

#include "realpaver/DomainBox.hpp"
#include "realpaver/EnvSolver.hpp"
#include "realpaver/NLPSolver.hpp"
#include "realpaver/NodeOptim.hpp"
#include "realpaver/NodePropagator.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ProblemBridge.hpp"
#include "realpaver/Prover.hpp"
#include "realpaver/SpaceOptim.hpp"
#include "realpaver/Split.hpp"
#include "realpaver/Timer.hpp"
#include "realpaver/Tolerance.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Optimizer for (un)constrained problems.
 *
 * An optimizer is a two-phase optimization algorithm. The first phase is
 * a preprocessing step that tries to reduce the variable domains, to
 * fix and then eliminate some variables, to detect inactive constraints
 * or unfeasible constraints. The second phase is an interval-based
 * branch-and-bound algorithm parameterized by several components.
 *
 * The optimizer takes as input a problem. Its components are generated according to
 * a Param object prm that can be assigned as follows:
 *
   @verbatim
   optimizer.getEnv()->setParam(prm);
   @endverbatim
 *
 * After the optimizing phase, it is possible to read the solutions as follows:
 *
   @verbatim
   for (size_t i=0; i<optimizer.nbSolutions(); ++i) {
      std::pair<DomainBox, Proof> sol = optimizer.getSolution(i);
   }
   @endverbatim
 *
 * The scope of each solution is the one of the input problem.
 */
class Optimizer {
public:
   /// Constructor
   Optimizer(const Problem &problem);

   /// Default estructor
   ~Optimizer() = default;

   /// No copy constructor
   Optimizer(const Optimizer &) = delete;

   /// No assignment
   Optimizer &operator=(const Optimizer &) = delete;

   /// Optimizing method
   void optimize();

   /// Returns the solving time in seconds
   double getOptimizingTime() const;

   /// Returns the number of nodes processed
   int getTotalNodes() const;

   /// Returns the environment of this
   std::shared_ptr<EnvSolver> getEnv() const;

   /// Returns the space of this
   std::shared_ptr<SpaceOptim> getSpace() const;

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

   /// Returns the current objective enclosure.
   Interval getObjectiveEnclosure() const;

private:
   SharedProblem problem_; // copy of initial problem
   SharedPreprocessor pp_; // preprocessor

   std::unique_ptr<NodeContext> context_;       // solving context for the BB algorithm
   std::shared_ptr<EnvSolver> env_;             // environment
   std::shared_ptr<SpaceOptim> space_;          // search tree
   SharedDag dag_;                              // dag
   std::unique_ptr<NodePropagator> propagator_; // contraction method
   std::unique_ptr<Split> split_;               // splitting strategy
   std::unique_ptr<ContractorFactory> factory_; // contractor factory
   std::unique_ptr<Prover> prover_;             // solution prover
   Timer stimer_;                               // timer for the solving phase
   int nbnodes_;                                // number of nodes processed
   bool withPreprocessing_;                     // true if preprocessing enabled

   void branchAndBound();
   void makeSpace();
   void makePropagator();
   void makeSplit();
   void bbStep(int depthlimit, bool splitOff);
   void bbStepAux(SharedNodeOptim node, int depthlimit, bool splitOff);
   void certifySolution(SharedNodeOptim &sol);
   Proof checkActivity(NodeOptim &node);

   void findIncumbent(SharedNodeOptim node);
   bool checkOptimality(const IntervalBox &B); // To be merge into certifySolution?

   Variable vobj_;         // Copy of objective variable
   Term obj_;              // Copy of objective expression
   Interval objEnclosure_; // Global evaluation of objective enclosure
   Tolerance objTol_;      // Tolerance on objective function

   std::shared_ptr<NLPSolver> localSolver_;
};

}; // namespace realpaver

#endif
