///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_LOCAL_CONJUGATE_HPP
#define REALPAVER_BO_LOCAL_CONJUGATE_HPP

#include "realpaver/BOLocalSolver.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This implements the conjugate gradient method.
///////////////////////////////////////////////////////////////////////////////
class BOLocalConjugate : public BOLocalSolver {
public:
   BOLocalConjugate();


   /// Overrides the method of BOLocalSolver
   OptimizationStatus minimize(RealFunction& f, IntervalVector& region,
                               const RealVector& initialPoint,
                               RealVector& finalPoint);

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
   /// @param val new value of the smalllest step length with 0.0 < val < 1.0
   void setStepTol(double val);

private:
   /// Component of a line search procedure that finds a step length
   /// @param f a real function
   /// @param x current point
   /// @param p descent direction
   /// @param s steepest descent direction
   /// @param fx f(x)
   /// @return a positive step in case of success, a negative value otherwise
   double findStep(RealFunction& f, RealVector& x, RealVector& p,
                   RealVector& s, double fx);

   size_t maxiter_;  // limit on the number of iterations
   double carmijo_;  // coefficient used in the Armijo rule
   double tolstep_;  // minimal value of the step length
};

} // namespace

#endif