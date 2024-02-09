///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <limits>
#include <stack>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/IntervalNewtonUni.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalNewtonUni::IntervalNewtonUni()
      : maxiter_(Param::GetIntParam("UNI_NEWTON_ITER_LIMIT")),
        tol_(Param::GetDblParam("NEWTON_REL_TOL"), 0.0),
        ltol_(Param::GetDblParam("NEWTON_CERTIFY_REL_TOL"),
              Param::GetDblParam("NEWTON_CERTIFY_ABS_TOL")),
        inflator_()
{}

size_t IntervalNewtonUni::getMaxIter() const
{
   return maxiter_;
}

void IntervalNewtonUni::setMaxIter(size_t n)
{
   ASSERT(n > 0, "bad parameter in the interval Interval Newton method");

   maxiter_ = n;
}

Tolerance IntervalNewtonUni::getTol() const
{
   return tol_;
}

void IntervalNewtonUni::setTol(const Tolerance& tol)
{
   tol_ = tol;
}

Tolerance IntervalNewtonUni::getLocalTol() const
{
   return ltol_;
}

void IntervalNewtonUni::setLocalTol(const Tolerance& tol)
{
   ltol_ = tol;
}

Inflator& IntervalNewtonUni::getInflator()
{
   return inflator_;
}

void IntervalNewtonUni::setInflator(const Inflator& inflator)
{
   inflator_ = inflator;
}

Proof IntervalNewtonUni::contract(IntervalFunctionUni& f,
                                  Interval& x)
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
 
         if (!tol_.testRelativeReduction(prev, y))
            iter = false;
      }
   }
   while (iter);

   LOG_LOW("> " << proof << " " << y);

   x = y;
   return proof;
}

Proof IntervalNewtonUni::step(IntervalFunctionUni& f, Interval& x)
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
      std::pair<Interval,Interval> q = extDiv(fc, dx);

      Interval xx1 = c - q.second,
               xx2 = c - q.first,
               nx = (x & xx1) | (x & xx2);

      if (nx.isEmpty())
         proof = Proof::Empty;

      else
         proof = Proof::Maybe;

      x = nx;
   }
   else
   {
      Interval xx = c - fc / dx,
               nx = x & xx;

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

Proof IntervalNewtonUni::search(IntervalFunctionUni& f, Interval& x)
{
   Proof proof = contract(f, x);
   if (proof != Proof::Maybe) return proof;
   if (x.isCanonical()) return proof;

   Proof pl = shrinkLeft(f, x);
   if (pl == Proof::Empty) return proof;

   Proof pr = shrinkRight(f, x);

   if (pl == Proof::Feasible) proof = pl;
   if (pr == Proof::Feasible) proof = pr;
   return proof;
}

Proof IntervalNewtonUni::shrinkLeft(IntervalFunctionUni& f, Interval& x)
{
   std::stack<Interval> stak;
   stak.push(x);
   Proof proof;

   while (!stak.empty())
   {
      Interval y = stak.top();
      stak.pop();
      proof = contract(f, y);
      
      if (proof == Proof::Empty) continue;

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

Proof IntervalNewtonUni::shrinkRight(IntervalFunctionUni& f, Interval& x)
{
   std::stack<Interval> stak;
   stak.push(x);
   Proof proof;

   while (!stak.empty())
   {
      Interval y = stak.top();
      stak.pop();
      proof = contract(f, y);

      if (proof == Proof::Empty) continue;

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

Proof IntervalNewtonUni::localSearch(IntervalFunctionUni& f, Interval& x)
{
   Proof proof = Proof::Maybe;
   Interval y = x.midpoint();
   bool iter = true;
   size_t nbiter = 0;

   LOG_INTER("Local search using the univariate Newton operator on " << x);

   do
   {
      Interval prev(y);
      Proof p = localStep(f, y);

      LOG_LOW("Step: " << p << ", " << y);

      if (p == Proof::Empty)
      {
         y = x;
         iter = false;
      }

      else if (p == Proof::Feasible)
      {
         proof = p;
         iter = false;
      }
   
      else if (++nbiter >= maxiter_)
      {
         y = x;
         iter = false;
      }

      else if (!ltol_.areClose(prev, y))
      {
         y = x;
         iter = false;
      }
   }
   while (iter);

   x = y;


   LOG_INTER(" -> " << proof);
   return proof;
}

Proof IntervalNewtonUni::localStep(IntervalFunctionUni& f, Interval& x)
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

} // namespace
