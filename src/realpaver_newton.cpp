// This file is part of Realpaver. License: see COPYING file.

#include <limits>
#include "realpaver_newton.hpp"
#include "realpaver_param.hpp"

namespace realpaver {

Newton::Newton() :
   imp_(Param::DefNewtonImprovement()),
   smax_(Param::DefNewtonMaxSteps()),
   prec_(Param::DefNewtonPrecision()),
   inflator_()
{}

Proof Newton::contract(UniFun& f, Interval& x)
{
   Proof proof = Proof::Maybe;
   Interval y = x;
   bool iter = true;
   int steps = 0;

   do
   {
      Interval prev(y);
      Proof p = step(f, y);

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

         if (!imp_.test(y, prev))
            iter = false;
   
         if (++steps >= smax_)
            iter = false;

         if (prec_.test(y))
            iter = false;
      }
   }
   while (iter);

   x = y;
   return proof;
}

Proof Newton::step(UniFun& f, Interval& x)
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

Proof Newton::localSearch(UniFun& f, Interval& x)
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
   
      else if (++steps >= smax_)
      {
         y = x;
         iter = false;
      }

      else if (dcur > dold)
      {
         y = x;
         iter = false;
      }
   }
   while (iter);

   x = y;
   return proof;
}

Proof Newton::localStep(UniFun& f, Interval& x)
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
