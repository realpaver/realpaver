///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOCAL_OPTIMIZER_HPP
#define REALPAVER_LOCAL_OPTIMIZER_HPP

#include "realpaver/Common.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealFunctionVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalOptimizer {
public:
   /// Constructor
   /// @param problem a numerical problem
   LocalOptimizer(const Problem& pb);

   /// Constructor
   /// @param obj a function over reals to be minimized
   /// @param ctrs a vector of functions over reals for the constraints
   LocalOptimizer(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Default copy constructor
   LocalOptimizer(const LocalOptimizer&) = default;

   /// No assignment
   LocalOptimizer& operator=(const LocalOptimizer&) = delete;

   /// Virtual destructor
   virtual ~LocalOptimizer();

   /// Minimization of a function
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src) = 0;

   /// @return the time limit for a run of minimize
   double timeLimit() const;

   /// Assigns the time limit for a run of minimize
   /// @param val time limit in seconds
   void timeLimit(double val);

   /// @return the iteration limit for a run of minimize
   size_t iterLimit() const;

   /// Assigns the iteration limit for a run of minimize
   /// @param iter iteration limit
   void iterLimit(size_t iter);

   /// @return the number of variables in the optimization problem
   size_t nbVars() const;

   /// @return the number of constraints in the optimization problem
   size_t nbCtrs() const;

   /// @return the scope of the optimization problem
   Scope scope() const;

   /// @return the scope of the objective function
   Scope obj_scope() const;

   /// @return the a pointer to the objective function
   std::shared_ptr<RealFunction> obj();

//    std::shared_ptr<RealFunctionVector> diff_obj();

   /// @return the pointer to the vector of constraints functions
   std::shared_ptr<RealFunctionVector> ctrs();


//    std::shared_ptr<Dag> ctr_dag();
//    std::shared_ptr<Dag> obj_dag();


   /// @return the pointer to the region in which the optimization is launched
   SharedIntervalRegion region();

   /// Assigns the region for a run of the minimize
   /// @param the interval region
   void region(const IntervalRegion& reg);

   /// @return the pointer to the starting point
   std::shared_ptr<RealPoint> start();

   /// Assigns the starting point to run minimize
   /// @param start the starting point
   void start(const RealPoint& start);

   /// @return the best value for the objective function
   double bestVal() const;

   /// @return the optimal point
   RealPoint bestPoint() const;

   /// @return the pointer to the optimal point
   std::shared_ptr<RealPoint> bestPoint();

   /// Assigns the optimal point
   /// @param best the new optimal point
   void bestPoint(std::shared_ptr<RealPoint> best);

   /// @return the optimization status
   OptimizationStatus status() const;

protected:
   std::shared_ptr<Problem> pb_;
   std::shared_ptr<RealFunction> obj_;            // Objective function on real numbers
//    std::shared_ptr<RealFunctionVector> diff_obj_; // Objective function on real numbers
   std::shared_ptr<RealFunctionVector> ctrs_;     // Vector of functions for the constraints

//    std::shared_ptr<Dag> dag_;                     // DAG to represent and store the problem to solve with a local solver
//    std::shared_ptr<Dag> odag_;                    // DAG to represent and store theobjective function

   size_t n_;     // number of variables
   size_t m_;     // number of constraints
   Scope s_;   // Scope of problem
   Scope os_;  // Scope of objective function
   
   SharedIntervalRegion reg_;                      // Interval region in which search for an optimal value
   std::shared_ptr<RealPoint> start_;              // Starting point

   std::shared_ptr<RealPoint> best_;              // Optimal point
   double best_val_;                              // objective function value for optimal point
   OptimizationStatus status_;

   double time_limit_;                             // Stop criterion based on time spend to optimize
   size_t iter_limit_;
};

} // namespace

#endif
