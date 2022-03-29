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
      : maxiter_(Param::GetIntParam("LINE_SEARCH_ITER_LIMIT")),
        alpha_(Param::GetDblParam("LINE_SEARCH_ARMIJO")),
        stol_(Param::GetDblParam("LINE_SEARCH_STEP_TOL")),
        gtol_(Param::GetDblParam("GRADIENT_DESCENT_TOL"))
{}

size_t BOLocalGradient::getIterLimit() const
{
   return maxiter_;
}

void BOLocalGradient::setIterLimit(size_t n)
{
   maxiter_ = n;
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
   return stol_;
}

void BOLocalGradient::setStepTol(double tol)
{
   ASSERT(tol > 0.0 && tol < 1.0,
          "Bad limit on the step length of the local solver");

   stol_ = tol;
}

double BOLocalGradient::getGradientTol() const
{
   return gtol_;
}

void BOLocalGradient::setGradientTol(double tol)
{
   ASSERT(tol > 0.0 && tol < 1.0,
          "Bad limit on the L2-norm of the gradient of the local solver");

   gtol_ = tol;
}

double BOLocalGradient::findStep(DiffRealFunction& f, RealVector& x,
                                 RealVector& g, RealVector& p, double fx)
{
   double step = 1.0,   // initial step
          res = -1.0,   // resulting step
          gp = g.scalarProduct(p);

   bool iter = true;
   Scope scope = f.funScope();

   while (iter)
   {
      RealVector y = x + step * p;
      double fy = f.realEval(RealPoint(scope, y));

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

DEBUG("STEP :  " << step);


      if (step < stol_) iter = false;
   }

   return res;
}

OptimizationStatus BOLocalGradient::minimize(RealFunction& f,
                                             const IntervalRegion& reg,
                                             const RealPoint& src,
                                             RealPoint& dest)
{
   DiffRealFunction* h = dynamic_cast<DiffRealFunction*>(&f);

   THROW_IF(h == nullptr, "Function not differentiable in a local solver");

   size_t dim = h->funArity();
   Scope scope = h->funScope();

   RealVector xk(src),           // current point
              gk(dim),           // gradient
              pk(dim),           // steepest descent direction
              xk_next(dim);      // next value of xk

   double uk, step;

   Timer tim;
   tim.start();

   bool iter = true;
   size_t nbiter = 0;
   OptimizationStatus status = OptimizationStatus::Optimal;

   do
   {
      uk = h->realEvalDiff(RealPoint(scope, xk), gk);
      if (Double::isNan(uk) || gk.isNan())
      {
         status = OptimizationStatus::Other;
         iter = false;
      }
      else
      {
         pk = -gk;
         step = findStep(*h, xk, gk, pk, uk);

DEBUG("pk norm : " << pk.l2Norm());


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

         if (pk.l2Norm() < gtol_) iter = false;
         if (++nbiter > maxiter_) iter = false;
         if (tim.elapsedTime() > getTimeLimit()) iter = false;
      }
   }
   while(iter);

   tim.stop();
   dest = RealPoint(scope, xk);
   return status;
}

} // namespace
