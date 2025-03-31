/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   IntervalNewtonUni.cpp
 * @brief  Univariate interval Newton operator
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/IntervalNewtonUni.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Tolerance.hpp"
#include <stack>

namespace realpaver {

IntervalNewtonUni::IntervalNewtonUni()
    : maxiter_(Param::GetIntParam("UNI_NEWTON_ITER_LIMIT"))
    , tol_(Param::GetDblParam("NEWTON_TOL"))
    , inflator_()
{
}

size_t IntervalNewtonUni::getMaxIter() const
{
   return maxiter_;
}

void IntervalNewtonUni::setMaxIter(size_t n)
{
   ASSERT(n > 0, "bad parameter in the interval Interval Newton method");

   maxiter_ = n;
}

double IntervalNewtonUni::getTol() const
{
   return tol_;
}

void IntervalNewtonUni::setTol(const double &tol)
{
   ASSERT(tol >= 0.0 && tol <= 1.0, "A relative tolerance must belong to [0, 1]");
   tol_ = tol;
}

Inflator &IntervalNewtonUni::getInflator()
{
   return inflator_;
}

void IntervalNewtonUni::setInflator(const Inflator &inflator)
{
   inflator_ = inflator;
}

Proof IntervalNewtonUni::contract(IntervalFunctionUni &f, Interval &x)
{
   LOG_LOW("Interval Newton: contract " << x << " (" << tol_ << ")");

   Proof proof = Proof::Maybe;
   Interval y = x;
   bool iter = true;
   size_t nbiter = 0;

   do
   {
      Interval prev(y);
      Proof p = step(f, y);

      LOG_LOW("  step > " << p << " " << y);

      if (p == Proof::Empty)
      {
         proof = p;
         y.setEmpty();
         iter = false;
      }
      else
      {
         if (p == Proof::Feasible)
            proof = p;

         if (++nbiter >= maxiter_)
            iter = false;

         if (!y.improves(prev, tol_))
            iter = false;
      }
   } while (iter);

   LOG_LOW("> " << proof << " " << y);

   x = y;
   return proof;
}

Proof IntervalNewtonUni::step(IntervalFunctionUni &f, Interval &x)
{
   Interval fx = f.eval(x);
   Interval dx = f.diff(x);

   if (fx.isEmpty() || (!fx.containsZero()))
      return Proof::Empty;

   if (dx.isInf() || dx.isZero())
      return Proof::Maybe;

   double c = x.midpoint();
   Interval fc = f.eval(Interval(c));

   if (fc.isEmpty())
      return Proof::Maybe;

   Proof proof;

   if (dx.strictlyContainsZero())
   {
      std::pair<Interval, Interval> q = extDiv(fc, dx);

      Interval xx1 = c - q.second, xx2 = c - q.first, nx = (x & xx1) | (x & xx2);

      if (nx.isEmpty())
         proof = Proof::Empty;

      else
         proof = Proof::Maybe;

      x = nx;
   }
   else
   {
      Interval xx = c - fc / dx, nx = x & xx;

      if (nx.isEmpty())
         proof = Proof::Empty;

      else if (x.contains(xx))
         proof = Proof::Feasible;

      else
         proof = Proof::Maybe;

      x = nx;
   }

   return proof;
}

Proof IntervalNewtonUni::contractWithSearch(IntervalFunctionUni &f, Interval &x)
{
   Proof proof = contract(f, x);
   if (proof != Proof::Maybe)
      return proof;
   if (x.isCanonical())
      return proof;

   Proof pl = shrinkLeft(f, x);
   if (pl == Proof::Empty)
      return proof;

   Proof pr = shrinkRight(f, x);

   if (pl == Proof::Feasible)
      proof = pl;
   if (pr == Proof::Feasible)
      proof = pr;
   return proof;
}

Proof IntervalNewtonUni::shrinkLeft(IntervalFunctionUni &f, Interval &x)
{
   std::stack<Interval> stak;
   stak.push(x);
   Proof proof;

   while (!stak.empty())
   {
      Interval y = stak.top();
      stak.pop();
      proof = contract(f, y);

      if (proof == Proof::Empty)
         continue;

      if (proof == Proof::Feasible)
      {
         x.setLeft(y.left());
         return proof;
      }

      if (y.isCanonical())
      {
         x.setLeft(y.left());
         return Proof::Maybe;
      }

      double c = y.midpoint();
      stak.push(Interval(c, y.right()));
      stak.push(Interval(y.left(), c));
   }

   return Proof::Empty;
}

Proof IntervalNewtonUni::shrinkRight(IntervalFunctionUni &f, Interval &x)
{
   std::stack<Interval> stak;
   stak.push(x);
   Proof proof;

   while (!stak.empty())
   {
      Interval y = stak.top();
      stak.pop();
      proof = contract(f, y);

      if (proof == Proof::Empty)
         continue;

      if (proof == Proof::Feasible)
      {
         x.setRight(y.right());
         return proof;
      }

      if (y.isCanonical())
      {
         x.setRight(y.right());
         return Proof::Maybe;
      }

      double c = y.midpoint();
      stak.push(Interval(y.left(), c));
      stak.push(Interval(c, y.right()));
   }

   return Proof::Empty;
}

Proof IntervalNewtonUni::localSearch(IntervalFunctionUni &f, Interval &x)
{
   Proof proof = Proof::Maybe;
   Interval y = x.midpoint();
   bool iter = true;
   size_t nbiter = 0;

   // given x_(k-2), x_(k-1), x_k three intervals from the sequence calculated
   // by this method, dprev is the distance between x_(k-2) and x_(k-1), dcurr
   // is the distance between x_(k-1) and x_k ; the method diverges if we have
   // dcurr > dprev
   double dprev = Double::inf(), dcurr;

   LOG_INTER("Local search using the univariate Newton operator on " << x);

   do
   {
      Interval prev(y);
      Proof p = localStep(f, y);

      dcurr = prev.distance(y);

      LOG_LOW("Step: " << p << ", " << y);

      if (p == Proof::Empty)
      {
         y = x;
         iter = false;
         LOG_LOW("Stopson local step -> empty");
      }

      else if (p == Proof::Feasible)
      {
         proof = p;
         iter = false;

         LOG_LOW("Stops on feasibility");
      }

      else if (dcurr > dprev)
      {
         y = x;
         iter = false;
         LOG_LOW("Stops since the method diverges");
      }

      else if (++nbiter >= maxiter_)
      {
         y = x;
         iter = false;
         LOG_LOW("Stops on a maximum number of iterations");
      }

      dprev = dcurr;
   } while (iter);

   x = y;

   LOG_INTER(" -> " << proof);
   return proof;
}

Proof IntervalNewtonUni::localStep(IntervalFunctionUni &f, Interval &x)
{
   Interval ix = inflator_.inflate(x);
   Interval fix = f.eval(ix);
   Interval dix = f.diff(ix);

   if (fix.isEmpty())
      return Proof::Empty;

   if (dix.isInf() || dix.containsZero())
      return Proof::Maybe;

   double ic = ix.midpoint();
   Interval fic = f.eval(Interval(ic));

   if (fic.isEmpty())
      return Proof::Maybe;

   Proof proof;

   Interval nix = ic - fic / dix;

   if (ix.contains(nix))
      proof = Proof::Feasible;

   else
      proof = Proof::Maybe;

   x = nix;

   return proof;
}

} // namespace realpaver
