///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_LOCAL_GRADIENT_HPP
#define REALPAVER_BO_LOCAL_GRADIENT_HPP

#include "realpaver/BOLocalSolver.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This implements the steepest descent method with Armijo line search.
///////////////////////////////////////////////////////////////////////////////
class BOLocalGradient : public BOLocalSolver {
public:
   /// Constructor
   BOLocalGradient();

   /// Overrides (BOLocalSolver)
   OptimizationStatus minimize(RealFunction& f,
                               const IntervalRegion& reg,
                               const RealPoint& src,
                               RealPoint& dest);

   /// Gets the limit on the number of iterations
   size_t getIterLimit() const;

   /// Sets the limit on the number of iterations
   /// @param val new value of the limit
   void setIterLimit(size_t val);

   /// Gets the coefficient used in the Armijo rule
   double getArmijoCoefficient() const;

   /// Sets the coefficient used in the Armijo rule
   /// @param val new value of the coefficient with 0.0 < val < 1.0
   void setArmijoCoefficient(double val);

   /// Gets the tolerance on the step length
   double getStepTol() const;

   /// Sets the tolerance on the step length
   /// @param tol new value of the smalllest step length with 0.0 < tol < 1.0
   void setStepTol(double tol);

   /// Gets the tolerance on the L2-norm of the gradient
   double getGradientTol() const;

   /// Sets the tolerance on the L2-norm of the gradient
   /// @param tol new value with 0.0 < tol
   void setGradientTol(double tol);

private:
   /// Component of a line search procedure that finds a step length
   /// @param f a real function
   /// @param x current point
   /// @param g gradient of f at x
   /// @param p descent direction
   /// @param fx f(x)
   /// @return a positive step in case of success, a negative value otherwise
   double findStep(DiffRealFunction& f, RealVector& x, RealVector& g,
                   RealVector& p, double fx);

   size_t maxiter_;  // limit on the number of iterations
   double alpha_;    // coefficient used in the Armijo rule
   double stol_;     // minimal value of the step length
   double gtol_;     // tolerance on the norm of the gradient
};

} // namespace

#endif
