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
 * @author Raphaël Chenouard, Laurent Granvilliers
 * @date   6 Oct 2025
 */

#ifndef REALPAVER_OPTIMIZER_HPP
#define REALPAVER_OPTIMIZER_HPP

#include "realpaver/Common.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/EnvOptimizer.hpp"
#include "realpaver/NodeContext.hpp"
#include "realpaver/NodeOptim.hpp"
#include "realpaver/NodePropagator.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Prospector.hpp"
#include "realpaver/RealPoint.hpp"
#include "realpaver/SpaceOptim.hpp"
#include "realpaver/Split.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

/**
 * @brief Branch-and-bound algorithm for solving optimization problems.
 *
 * The branch-and-bound algorithm solves a minimization problem. If the initial problem is
 * a maximization of f(X), it is transformed as min -f(X), and when an enclosure I of the
 * global optimum is found, we just return -I.
 *
 * The following constraint is added in the problem by the contractor factory: z-f(X)>=0
 * given min z = f(X), and z+f(X)>=0 given max z = f(X) <=> min z = -f(X). This constraint
 * is used to find lower bounds.
 */
class Optimizer {
public:
   /// Constructor
   Optimizer(const Problem &pbm);

   /// Default destructor
   ~Optimizer() = default;

   /// No copy constructor
   Optimizer(const Optimizer &) = delete;

   /// No assignment
   Optimizer &operator=(const Optimizer &) = delete;

   /// Returns the solving time in seconds
   double getSolvingTime() const;

   /// Returns the number of nodes processed (reduction, ...)
   int getNbNodes() const;

   /// Returns the number of splitting steps
   int getNbSplits() const;

   /// Returns the environment of this
   std::shared_ptr<EnvOptimizer> getEnv() const;

   /// Returns the space of this
   std::shared_ptr<SpaceOptim> getSpace() const;

   /// Returns the preprocessor used by this
   SharedPreprocessor getPreprocessor() const;

   /// Optimization method
   void optimize();

   /// Returns true if this has been successfully solved
   bool isSolvedOptimal() const;

   /// Returns an enclosure of the optimum
   Interval getOptimum() const;

   /**
    * @brief Returns the best feasible point found.
    *
    * The scope of the result is the one of the initial problem.
    * If no point is found, an exception is thrown.
    */
   RealPoint bestFeasible() const;

   /// Returns the status after optimize()
   OptimizationStatus getStatus() const;

private:
   SharedProblem pbm_;                          // copy of initial problem
   SharedPreprocessor pp_;                      // preprocessor
   std::unique_ptr<NodeContext> context_;       // solving context for the BP algorithm
   std::shared_ptr<EnvOptimizer> env_;          // environment
   std::shared_ptr<SpaceOptim> space_;          // search tree
   SharedDag dag_;                              // dag
   std::unique_ptr<NodePropagator> propagator_; // contraction method
   std::unique_ptr<Split> split_;               // splitting strategy
   std::unique_ptr<ContractorFactory> factory_; // contractor factory
   std::unique_ptr<Prospector> prospector_;     // search for feasible points

   double L_, U_;                    // bounds of the global minimum
   Variable z_;                      // objective variable in the new problem
   Term obj_;                        // objective function to minimize in the new problem
   std::unique_ptr<RealPoint> best_; // best feasible point

   Timer stimer_;              // timer for the solving phase
   int nbnodes_;               // number of nodes processed
   bool withPreprocessing_;    // true if preprocessing enabled
   OptimizationStatus status_; // resulting status

   void branchAndBound();
   void makeSpace();
   void makePropagator();
   void makeSplit();
   void makeProspector();
   void bbStep(int depthlimit);
   void bbStepAux(SharedNodeOptim node, int depthlimit);
   Proof checkActivity(NodeOptim &node);
   bool optimumFound() const;
   void traceOptimum();
};

}; // namespace realpaver

#endif
