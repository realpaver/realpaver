///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LOCAL_SOLVER_HPP
#define REALPAVER_LOCAL_SOLVER_HPP

#include "realpaver/Common.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealFunctionVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class LocalSolver {
public:
   /// Default constructor
   /// @param problem a numerical problem
   LocalSolver(const Problem& pb, bool withobj = false);
   LocalSolver(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Default copy constructor
   LocalSolver(const LocalSolver&) = default;

   /// No assignment
   LocalSolver& operator=(const LocalSolver&) = delete;

   /// Virtual destructor
   virtual ~LocalSolver();

   /// Minimization of a function
   /// @param f a real function
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @param dest final point found by the optimization procedure
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src,
                                       RealPoint& dest) = 0;

   /// @return the time limit for a run of minimize
   double getTimeLimit() const;

   /// Assigns the time limit for a run of minimize
   /// @param val time limit in seconds
   void setTimeLimit(double val);

   double get_best_val() const;
   RealPoint get_best_point() const;
   OptimizationStatus get_status() const;

protected:
   std::shared_ptr<Problem> pb_;
   std::shared_ptr<RealFunction> obj_;            // Objective function on real numbers
   std::shared_ptr<RealFunctionVector> diff_obj_; // Objective function on real numbers
   std::shared_ptr<RealFunctionVector> ctrs_;     // Vector of functions for the constraints

   std::shared_ptr<Dag> dag_;                     // DAG to represent and store the problem to solve with a local solver
   std::shared_ptr<Dag> odag_;                    // DAG to represent and store theobjective function

   std::shared_ptr<RealPoint> best_;              // Optimal point
   double best_val_;                              // objective function value for optimal point
   OptimizationStatus status_;


   int n_;     // number of variables
   int m_;     // number of constraints
   Scope s_;   // Scope of problem
   Scope os_;  // Scope of objective function
   
   Variable z_;         // Variable to represent the objective function and formulate a constraint on the objective
   size_t ic_;          // index of the objective constraint in the DAG

   SharedIntervalRegion reg_;                      // Interval region in which search for an optimal value
   void updateRegion(const IntervalRegion& reg);   // Update of search region

   std::shared_ptr<RealPoint> start_;              // Starting point
   void updateStart(const RealPoint& start);       // Update of starting point

private:
   double time_limit_;                             // Stop criterion based on time spend to optimize
};

} // namespace

#endif
