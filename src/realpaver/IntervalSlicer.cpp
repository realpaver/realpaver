///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

IntervalSlicer::~IntervalSlicer()
{}

size_t IntervalSlicer::apply(const Interval& x)
{
   cont_.clear();
   applyImpl(x);
   return cont_.size();
}

void IntervalSlicer::push(const Interval& x)
{
   if (!x.isEmpty())
      cont_.push_back(x);
}

void IntervalSlicer::clear()
{
   cont_.clear();
}

size_t IntervalSlicer::nbSlices() const
{
   return cont_.size();
}

IntervalSlicer::iterator IntervalSlicer::begin()
{
   return cont_.begin();
}

IntervalSlicer::iterator IntervalSlicer::end()
{
   return cont_.end();
}

///////////////////////////////////////////////////////////////////////////////

void IntervalBisecter::applyImpl(const Interval& x)
{
   double m = x.midpoint();
   push(Interval(x.left(),m));
   push(Interval(m,x.right()));
}

///////////////////////////////////////////////////////////////////////////////

IntervalPeeler::IntervalPeeler(const double& f) : IntervalSlicer()
{
   setFactor(f);
}

double IntervalPeeler::getFactor() const
{
   return f_;
}

void IntervalPeeler::setFactor(const double& f)
{
   ASSERT(f>=0.0 && f<=100.0, "Bad width factor " << f
                                                  << " in an interval peeler");

   f_ = f;
   p_ = f_ / Interval(100.0);
}

void IntervalPeeler::applyImpl(const Interval& x)
{
   // width of x
   double wx = x.width();

   // slice width
   Interval w(wx);
   w *= p_;

   // splitting points
   Interval a(x.left()), b(x.right());
   a += w;
   b -= w;

   double c = a.left(),
          d = b.right();

   THROW_IF(c<=x.left() || c>=d || d>=x.right(),
            "Peeling of " << x << " impossible");

   push(Interval(x.left(), c));
   push(Interval(c, d));
   push(Interval(d, x.right()));
}

Interval IntervalPeeler::peelLeft(const Interval& x) const
{
   // width of x
   double wx = x.width();

   // slice width
   Interval w(wx);
   w *= p_;

   // splitting point
   Interval y(x.left());
   y += w;
   double c = y.left();

   return x.strictlyContains(c) ? Interval(x.left(), c) : x;
}

Interval IntervalPeeler::peelRight(const Interval& x) const
{
   // width of x
   double wx = x.width();

   // slice width
   Interval w(wx);
   w *= p_;

   // splitting point
   Interval y(x.right());
   y -= w;
   double c = y.right();

   return x.strictlyContains(c) ? Interval(c, x.right()) : x;
}

///////////////////////////////////////////////////////////////////////////////

IntervalPartitioner::IntervalPartitioner(size_t n)
{
   setArity(n);
}

size_t IntervalPartitioner::getArity() const
{
   return n_;
}

void IntervalPartitioner::setArity(size_t n)
{
   ASSERT(n >= 2, "Bad arity " << n << " for an interval partitionner");

   n_ = n;
}

void IntervalPartitioner::applyImpl(const Interval& x)
{
   if (n_ == 2)
   {
      IntervalBisecter slicer;
      slicer.apply(x);

      // get the slices from the other slicer
      for (Interval r : slicer)
         push(r);
   }
   else
   {
      bool cond = true;
      double l, r = x.left(), h = x.width() / n_;
      size_t i = 1;
 
      while (cond && i < n_)
      {
         l = r;
         r = x.left() + i*h;
         if (l >= r)
            cond = false;
         else
         {
            push(Interval(l,r));
            ++i;
         }
      }
      if (r >= x.right())
         cond = false;
      
      else
         push(Interval(r,x.right()));

      if (!cond)
      {
         clear();

         IntervalBisecter slicer;
         slicer.apply(x);

         for (Interval r : slicer)
            push(r);
      }
   }
}

} // namespace
