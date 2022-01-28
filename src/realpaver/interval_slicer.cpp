// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/interval_slicer.hpp"

namespace realpaver {

IntervalSlicer::~IntervalSlicer()
{}

size_t IntervalSlicer::apply(const Interval& x)
{
   cont_.clear();
   applyImpl(x);
   return cont_.size();
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

void IntervalPeeler::setFactor(const double& f)
{
   ASSERT(f<=0.0 || f>=100.0, "bad width factor " << f);

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
            "unable to peel the interval " << x);

   push(Interval(x.left(),c));
   push(Interval(c,d));
   push(Interval(d,x.right()));
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

   return x.strictlyContains(c) ? Interval(x.left(),c) : x;
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

   return x.strictlyContains(c) ? Interval(c,x.right()) : x;
}

///////////////////////////////////////////////////////////////////////////////

IntervalPartitioner::IntervalPartitioner(size_t n)
{
   setArity(n);
}

void IntervalPartitioner::setArity(size_t n)
{
   ASSERT(n >= 2, "interval partitionner with bad arity " << n);

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
