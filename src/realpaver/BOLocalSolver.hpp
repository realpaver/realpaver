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
#include "realpaver/IntervalVector.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealVector.hpp"

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
   /// @param region a region
   /// @param initialPoint starting point that belongs to the region
   /// @param finalPoint final point found by the optimization procedure
   /// @return an optimization status
   ///
   /// Moreover, the values of the function at the initial and final points
   /// must be computed and stored in this.
   virtual OptimizationStatus minimize(RealFunction& f,
                                       IntervalVector& region,
                                       const RealVector& initialPoint,
                                       RealVector& finalPoint);

   /// @return the value of the objective function at the initial point
   double getInitObjVal() const;

   /// @return the value of the objective function at the final point
   double getFinalObjVal() const;

protected:
   // management of the values of the objective functions
   void setInitObjVal(double& val);
   void setFinalObjVal(double& val);

private:
   double initObjVal_;
   double finalObjVal_;
};

} // namespace

#endif
