///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <limits>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/interval_newton.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/number.hpp"

namespace realpaver {

IntervalNewton::IntervalNewton() :
   maxiter_(Integer::max()),
   xtol_(Tolerance::makeRel(1.0e-8)),
   dtol_(Tolerance::makeRel(1.0e-8)),
   ldtol_(Tolerance::makeRel(1.0e-8)),
   inflator_()
{}

int IntervalNewton::getMaxIterations() const
{
   return maxiter_;
}

void IntervalNewton::setMaxIterations(int n)
{
   ASSERT(n > 0, "bad parameter in the interval Interval Newton method");

   maxiter_ = n;
}

Tolerance IntervalNewton::getXTol() const
{
   return xtol_;
}

void IntervalNewton::setXTol(const Tolerance& tol)
{
   xtol_ = tol;
}

Tolerance IntervalNewton::getDTol() const
{
   return dtol_;
}

void IntervalNewton::setDTol(const Tolerance& tol)
{
   dtol_ = tol;
}

Tolerance IntervalNewton::getLocalDTol() const
{
   return ldtol_;
}

void IntervalNewton::setLocalDTol(const Tolerance& tol)
{
   ldtol_ = tol;
}

Inflator IntervalNewton::getInflator() const
{
   return inflator_;
}

void IntervalNewton::setInflator(const Inflator& inflator)
{
   inflator_ = inflator;
}

Proof IntervalNewton::contract(UniFun& f, Interval& x)
{
   LOG_INTERNAL("\nInterval Newton: contract " << x);
   LOG_INTERNAL("Xtol: " << xtol_ << ", " << "DTol: " << dtol_);

   Proof proof = Proof::Maybe;
   Interval y = x;
   bool iter = true;
   int steps = 0;

   do
   {
      Interval prev(y);
      Proof p = step(f, y);

      LOG_INTERNAL("  step > " << p << " " << y);

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

         if (++steps >= maxiter_)
            iter = false;

         if (xtol_.hasTolerance(y))
            iter = false;
 
         if (dtol_.hasTolerance(prev, y))
            iter = false;
      }
   }
   while (iter);

   LOG_INTERNAL("> " << proof << " " << y);

   x = y;
   return proof;
}

Proof IntervalNewton::step(UniFun& f, Interval& x)
{
   std::pair<Interval,Interval> p = f.evalDiff(x);
   const Interval& fx = p.first;
   const Interval& dx = p.second;

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

Proof IntervalNewton::localSearch(UniFun& f, Interval& x)
{
   Proof proof = Proof::Maybe;
   Interval y = x.midpoint();
   bool iter = true;
   int steps = 0;
   double dold,
          dcur = std::numeric_limits<double>::infinity();

   do
   {
      Interval prev(y);
      Proof p = localStep(f, y);

      dold = dcur;
      dcur = y.distance(prev);

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
   
      else if (++steps >= maxiter_)
      {
         y = x;
         iter = false;
      }

      else if (dcur > dold)
      {
         y = x;
         iter = false;
      }

      else if (ldtol_.hasTolerance(prev, y))
      {
         y = x;
         iter = false;
      }
   }
   while (iter);

   x = y;
   return proof;
}

Proof IntervalNewton::localStep(UniFun& f, Interval& x)
{
   Interval ix = inflator_.inflate(x);

   std::pair<Interval,Interval> p = f.evalDiff(ix);
   const Interval& fix = p.first;
   const Interval& dix = p.second;

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
