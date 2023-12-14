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
#include "realpaver/UniIntervalNewton.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

UniIntervalNewton::UniIntervalNewton() :
   maxiter_(Param::GetIntParam("UNI_NEWTON_ITER_LIMIT")),
   xtol_(Param::GetTolParam("XTOL")),
   dtol_(Param::GetTolParam("NEWTON_DTOL")),
   ldtol_(Param::GetTolParam("NEWTON_CERTIFY_DTOL")),
   inflator_()
{}

size_t UniIntervalNewton::getMaxIter() const
{
   return maxiter_;
}

void UniIntervalNewton::setMaxIter(size_t n)
{
   ASSERT(n > 0, "bad parameter in the interval Interval Newton method");

   maxiter_ = n;
}

Tolerance UniIntervalNewton::getXTol() const
{
   return xtol_;
}

void UniIntervalNewton::setXTol(const Tolerance& tol)
{
   xtol_ = tol;
}

Tolerance UniIntervalNewton::getDTol() const
{
   return dtol_;
}

void UniIntervalNewton::setDTol(const Tolerance& tol)
{
   dtol_ = tol;
}

Tolerance UniIntervalNewton::getLocalDTol() const
{
   return ldtol_;
}

void UniIntervalNewton::setLocalDTol(const Tolerance& tol)
{
   ldtol_ = tol;
}

Inflator& UniIntervalNewton::getInflator()
{
   return inflator_;
}

void UniIntervalNewton::setInflator(const Inflator& inflator)
{
   inflator_ = inflator;
}

Proof UniIntervalNewton::contract(UniIntervalFunction& f,
                                  Interval& x)
{
   LOG_LOW("Interval Newton: contract " << x);
   LOG_LOW("Xtol: " << xtol_ << ", " << "DTol: " << dtol_);

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

         if (xtol_.hasTolerance(y))
            iter = false;
 
         if (dtol_.haveDistTolerance(prev, y))
            iter = false;
      }
   }
   while (iter);

   LOG_LOW("> " << proof << " " << y);

   x = y;
   return proof;
}

Proof UniIntervalNewton::step(UniIntervalFunction& f, Interval& x)
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

Proof UniIntervalNewton::search(UniIntervalFunction& f, Interval& x)
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

Proof UniIntervalNewton::shrinkLeft(UniIntervalFunction& f, Interval& x)
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

Proof UniIntervalNewton::shrinkRight(UniIntervalFunction& f, Interval& x)
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

Proof UniIntervalNewton::localSearch(UniIntervalFunction& f, Interval& x)
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

      else if (!ldtol_.haveDistTolerance(prev, y))
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

Proof UniIntervalNewton::localStep(UniIntervalFunction& f, Interval& x)
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
