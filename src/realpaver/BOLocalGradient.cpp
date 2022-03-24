///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOLocalGradient.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Timer.hpp"

namespace realpaver {

BOLocalGradient::BOLocalGradient()
      : max_iter_(Param::GetIntParam("LINE_SEARCH_ITER_LIMIT")),
        alpha_(Param::GetDblParam("LINE_SEARCH_ARMIJO")),
        tol_(Param::GetDblParam("LINE_SEARCH_STEP_TOL"))
{}

size_t BOLocalGradient::getIterLimit() const
{
   return max_iter_;
}

void BOLocalGradient::setIterLimit(size_t n)
{
   max_iter_ = n;
}

double BOLocalGradient::getArmijoCoefficient() const
{
   return alpha_;
}

void BOLocalGradient::setArmijoCoefficient(double val)
{
   ASSERT(val > 0.0 && val < 1.0,
          "Bad coefficient for the Armijo rule " << val);

   alpha_ = val;
}

double BOLocalGradient::getStepTol() const
{
   return tol_;
}

void BOLocalGradient::setStepTol(double tol)
{
   ASSERT(tol > 0.0 && tol < 1.0,
          "Bad limit on the step length of the conjugate gradient method");

   tol_ = tol;
}

double BOLocalGradient::findStep(RealFunction& f, RealVector& x, RealVector& g,
                                 RealVector& p, double fx)
{
   double step = 1.0,   // initial step
          res = -1.0,   // resulting step
          gp = g.scalarProduct(p);

   bool iter = true;
   Scope scope = f.rfunScope();

   while (iter)
   {
      RealVector y = x + step * p;
      double fy = f.rfunEval(RealPoint(scope, y));

      if (!Double::isNan(fy))
      {
         if (fy <= fx + alpha_ * step * gp)
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
      if (step < tol_) iter = false;
   }

   return res;
}

OptimizationStatus BOLocalGradient::minimize(RealFunction& f,
                                             const IntervalRegion& reg,
                                             const RealPoint& src,
                                             RealPoint& dest)
{
   size_t dim = f.rfunArity();
   Scope scope = f.rfunScope();

   RealVector xk(src),           // current point
              gk(dim),           // gradient
              pk(dim),           // steepest descent direction
              xk_next(dim);      // next value of xk

   double uk, step;

   Timer tim;
   tim.start();

   bool iter = true;
   size_t nb_iter = 0;
   OptimizationStatus status = OptimizationStatus::Optimal;

   do
   {
      f.rfunEvalDiff(RealPoint(scope, xk), gk, uk);
      if (Double::isNan(uk) || gk.isNan())
      {
         status = OptimizationStatus::Other;
         iter = false;
      }
      else
      {
         pk = -gk;
         step = findStep(f, xk, gk, pk, uk);

         if (step > 0.0)
         {
            xk_next = xk + step * pk;
            if (reg.contains(RealPoint(scope, xk_next)))
            {
               xk = xk_next;
            }
            else
            {
               iter = false;
            }
         }
         else
         {
            iter = false;
         }

         if (++nb_iter > max_iter_) iter = false;
         if (tim.elapsedTime() > getTimeLimit()) iter = false;
      }
   }
   while(iter);

   tim.stop();
   dest = RealPoint(scope, xk);
   return status;
}

} // namespace
