///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_SOLVER_HPP
#define REALPAVER_BO_SOLVER_HPP

#include "realpaver/BOLocalSolver.hpp"
#include "realpaver/BOModel.hpp"
#include "realpaver/BOSpace.hpp"
#include "realpaver/BOSplit.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

class BOSolver {
public:
   /// Creates a solver
   /// @param problem a bound optimization problem
   BOSolver(Problem& problem);

   /// Destructor
   ~BOSolver();

   /// Optimization method
   /// @return true if an optimal solution is found
   bool optimize();

   /// @return the status of the last optimization stage
   OptimizationStatus getStatus() const;

   /// @return the enclosure of the optimum after the optimization process
   Interval getObjEnclosure() const;

   /// @return the best point found by the optimization process
   RealVector getBestSolution() const;

   /// @return the preprocessing time in seconds
   double getPreprocessingTime() const;

   /// @return the solving time in seconds
   double getSolvingTime() const;

   /// Sets a time limit of the optimization method
   /// @param t a time limit in seconds
   void setTimeLimit(double t);

   /// @return the time limit of the optimization method in seconds
   double getTimeLimit() const;

   /// @return the maximum number of nodes in the search tree
   size_t getNodeLimit() const;

   /// Sets the maximum number of nodes in the search tree
   /// @param n new node limit
   void setNodeLimit(size_t n);

   /// @return true if the domain of the objective variable is split
   bool isSplitableObj() const;

   /// Sets the splitting status of the objective variable
   /// @param split true if the domain of the objective variable is split
   void setSplitableObj(bool split);

private:
   Problem problem_;    // initial problem
   Problem preprob_;    // problem resulting from preprocessing
   Problem solprob_;    // problem resulting from presolving

   BOModel* model_;
   BOLocalSolver* localSolver_;
   BOSplit* split_;

   // Result of optimization
   OptimizationStatus status_;
   IntervalVector sol_;
   Interval objval_;

   // Auxiliary methods
   void makeLocalSolver();
   void makeSplit();

   bool preprocess();
   bool presolve();

   void solve();
   void branchAndBound();
   bool bbStep(BOSpace& space, BOSpace& sol);
   void findInitialBounds(SharedBONode& node);

   typedef Preprocessor::VarVarMapType VarVarMapType;

   // Maps every variable of the preprocessed prolem to the associated
   // variable in the initial problem
   VarVarMapType vmap21_;

   // Maps every variable of the presolved prolem to the associated
   // variable in the initial problem
   VarVarMapType vmap31_;

   Timer ptimer_;      // timer for the preprocessing phase
   Timer stimer_;      // timer for the solving phase

   double timelimit_;  // time limit in seconds
   size_t nodelimit_;  // node limit
   bool splitobj_;     // true if the domain of the objective variable is split
};

} // namespace

#endif
