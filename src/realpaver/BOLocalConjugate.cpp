///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOLocalConjugate.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

BOLocalConjugate::BOLocalConjugate()
      : maxiter_(Param::getIntParam("LS_ITER_LIMIT")),
        carmijo_(Param::getDblParam("LS_ARMIJO_COEF")),
        tolstep_(Param::getDblParam("LS_STEP_TOL"))
{}

size_t BOLocalConjugate::getIterLimit() const
{
   return maxiter_;
}

void BOLocalConjugate::setIterLimit(size_t n)
{
   maxiter_ = n;
}

double BOLocalConjugate::getArmijoCoefficient() const
{
   return carmijo_;
}

void BOLocalConjugate::setArmijoCoefficient(double val)
{
   ASSERT(val > 0.0 && val < 1.0,
          "Bad coefficient for the Armijo rule " << val);

   carmijo_ = val;
}

double BOLocalConjugate::getStepTol() const
{
   return tolstep_;
}

void BOLocalConjugate::setStepTol(double val)
{
   ASSERT(val > 0.0 && val < 1.0,
          "Bad limit on the step length of the conjugate gradient method");

   tolstep_ = val;
}

double BOLocalConjugate::findStep(RealFunction& f, RealVector& x,
                                  RealVector& p, RealVector& s, double fx)
{
   double step = 1.0,   // initial step
          res = -1.0,   // resulting step
          p_s = p.scalarProduct(s);

   bool iter = true;

   while (iter)
   {
      RealVector y = x + step * p;
      double fy = f.rfunEval(y);

      if (!Double::isNan(fy))
      {
         if (fy <= fx - carmijo_ * step * p_s)
         {
            // Armijo rule: stops if the value of f is decreased enough
            iter = false;
            res = step;
         }
         else if (fy <= fx)
         {
            // saves the step and iterates
            res = step;
         }
      }

      step /= 2.0;
      if (step < tolstep_) iter = false;
   }

   return res;
}

OptimizationStatus
BOLocalConjugate::minimize(RealFunction& f, IntervalVector& region,
                           const RealVector& initialPoint,
                           RealVector& finalPoint)
{
   size_t dim = f.rfunArity();

   double uk, uk_1, step;

   RealVector xk(initialPoint),  // current point
              grad(dim),         // gradient
              sk(dim),           // steepest descent direction
              pk(dim),           // search direction
              xk_1(dim),         // next value of xk
              sk_1(dim),         // next value of sk
              pk_1(dim);         // next value of pk

   // evaluates and differentiates f at xk
   f.rfunEvalDiff(xk, grad, uk);
   setInitObjVal(uk);

   DEBUG("\npoint : " << xk << "   " << "grad : " << grad << "   val : " << uk);

   if (Double::isNan(uk) || grad.isNan())
      return OptimizationStatus::Other;

   // line search
   sk = -grad;
   pk = sk;
   step = findStep(f, xk, pk, sk, uk);

   DEBUG("dir : " << pk << "   step : " << step);

   if (step > 0.0)
      xk_1 = xk + step * pk;

   if (step < 0.0 || (!region.contains(xk_1)))
      return OptimizationStatus::Other;

   // loop
   size_t nbiter = 0;
   bool iter = true;

   while (iter)
   {
      f.rfunEvalDiff(xk_1, grad, uk_1);
      sk_1 = -grad;

   DEBUG("\npoint : " << xk_1 << "   " << "grad : " << grad << "   val : " << uk_1);
    
      if (Double::isNan(uk_1) || grad.isNan())
      {
         xk_1 = xk;
         uk_1 = uk;
         iter = false;
      }

      // Polak-Ribiere formula
      double beta = std::max(0.0, sk_1.scalarProduct(sk_1-sk) /
                                  sk.scalarProduct(sk));

      pk_1 = sk_1 + beta * pk;
      step = findStep(f, xk_1, pk_1, sk_1, uk_1);

   DEBUG("dir : " << pk_1 << "   step : " << step);

      if (step > 0.0)
      {
         // update
         xk = xk_1;
         xk_1 = xk_1 + step * pk_1;
         sk = sk_1;
         uk = uk_1;
      }
      else
      {
         iter = false;  // no improvement of the upper bound
      }

      if (iter && (!region.contains(xk_1)))
      {
         xk_1 = xk;
         uk_1 = uk;
         iter = false;         
      }

      if (++nbiter > maxiter_) iter = false;
   }

   finalPoint = xk_1;
   setFinalObjVal(uk_1);

   return OptimizationStatus::Optimal;
}

} // namespace
