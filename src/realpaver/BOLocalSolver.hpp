///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_LOCAL_SOLVER_HPP
#define REALPAVER_BO_LOCAL_SOLVER_HPP

#include "realpaver/Common.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///
/// The default implementation is trivial. Given a region, it simply returns
/// its midpoint.
///////////////////////////////////////////////////////////////////////////////
class BOLocalSolver {
public:
   /// Default constructor
   BOLocalSolver();

   /// Default copy constructor
   BOLocalSolver(const BOLocalSolver&) = default;

   /// No assignment
   BOLocalSolver& operator=(const BOLocalSolver&) = delete;

   /// Virtual destructor
   virtual ~BOLocalSolver();

   /// Minimization of a function
   /// @param f a real function
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @param dest final point found by the optimization procedure
   /// @param
   /// @return an optimization status
   ///
   /// Moreover, the values of the function at the initial and final points
   /// must be computed and stored in this.
   virtual OptimizationStatus minimize(RealFunction& f,
                                       const IntervalRegion& reg,
                                       const RealPoint& src,
                                       RealPoint& dest);

   /// @return the time limit for a run of minimize
   double getTimeLimit() const;

   /// Assigns the time limit for a run of minimize
   /// @param val time limit in seconds
   void setTimeLimit(double val);

private:
   double time_limit_;
};

} // namespace

#endif
