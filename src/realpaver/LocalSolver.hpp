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

protected:
   std::shared_ptr<RealFunction> obj_;            // Objective function on real numbers
   std::shared_ptr<RealFunctionVector> ctrs_;     // Vector of functions for the constraints

   std::shared_ptr<RealPoint> best_;

   int n_;
   int m_;
   Scope s_;

private:
   double time_limit_;
};

} // namespace

#endif
