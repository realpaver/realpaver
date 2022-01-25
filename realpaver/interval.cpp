// This file is part of realpaver. License: see COPYING file.

#include "realpaver/interval.hpp"

namespace realpaver {

Interval::Interval():
   Interval(IntervalImpl::create())
{}

Interval::Interval(const double& l, const double& r):
   Interval(IntervalImpl::create(l,r))
{}

Interval::Interval(const double& a):
   Interval(IntervalImpl::create(a))
{}

Interval::Interval(const char* s):
   Interval(IntervalImpl::create(s))
{}

Interval::Interval(const char* sl, const char* sr):
   Interval(IntervalImpl::create(sl,sr))
{}

Interval Interval::lessThan(const double& a)
{
   return Interval(Interval::universe().left(),a);
}

Interval Interval::moreThan(const double& a)
{
   return Interval(a,Interval::universe().right());   
}

Interval::Interval(const IntervalImpl::interval& x):
   impl_(x)
{}

Interval Interval::universe()
{
   static Interval x(IntervalImpl::universe());
   return x;
}

Interval Interval::positive()
{
   static Interval x(IntervalImpl::positive());
   return x;
}

Interval Interval::negative()
{
   static Interval x(IntervalImpl::negative());
   return x;
}

Interval Interval::minusOnePlusOne()
{
   static Interval x( Interval::minusOne() | Interval::one() );
   return x;
}

Interval Interval::minusOneZero()
{
   static Interval x( Interval::minusOne() | Interval::zero() );
   return x;
}

Interval Interval::zeroPlusOne()
{
   static Interval x( Interval::zero() | Interval::one() );
   return x;
}

Interval Interval::emptyset()
{
   static Interval x(IntervalImpl::emptyset());
   return x;
}

Interval Interval::halfPi()
{
   static Interval x(IntervalImpl::halfPi());
   return x;
}

Interval Interval::pi()
{
   static Interval x(IntervalImpl::pi());
   return x;
}

Interval Interval::twoPi()
{
   static Interval x(IntervalImpl::twoPi());
   return x;
}

Interval Interval::zero()
{
   static Interval x(IntervalImpl::zero());
   return x;
}

Interval Interval::one()
{
   static Interval x(IntervalImpl::one());
   return x;
}

Interval Interval::minusOne()
{
   static Interval x(IntervalImpl::minusOne());
   return x;
}

Interval Interval::minusPiPlusPi()
{
   static Interval x(IntervalImpl::minusPiPlusPi());
   return x;
}

Interval Interval::zeroTwoPi()
{
   static Interval x(IntervalImpl::zeroTwoPi());
   return x;
}

double Interval::infinity()
{
   static double a = IntervalImpl::infinity();
   return a;
}

Interval Interval::inflate(const double& delta, const double& chi) const
{
   ASSERT(delta > 1.0, "bad inflation factor: delta = " << delta);
   ASSERT(chi > 0.0, "bad inflation factor: chi = " << chi);

   if (isEmpty() || isInf())
      return *this;

   Interval m(midpoint());

   return m + delta*(*this - m) + chi*minusOnePlusOne();
}

std::pair<Interval,Interval> complement(const Interval& x)
{
   Interval E(Interval::emptyset());

   if (x.isEmpty())
      return std::make_pair(Interval::universe(),E);

   bool infl = x.isInfLeft(),
        infr = x.isInfRight();

   if (infl)
   {
      if (infr)
         return std::make_pair(E,E);

      else
         return std::make_pair(Interval::moreThan(x.right()),E);
   }
   else
   {
      if (infr)
         return std::make_pair(Interval::lessThan(x.left()),E);

      else
         return std::make_pair(Interval::lessThan(x.left()),
                               Interval::moreThan(x.right()));
   }
}

std::pair<Interval,Interval> setminus(const Interval& x, const Interval& y)
{
   Interval E(Interval::emptyset());

   if (x.isEmpty() || y.isEmpty())
      return std::make_pair(x,E);

   if (y.contains(x))
      return std::make_pair(E,E);

   if (x.contains(y))
   {
      if (y.isSingleton())
         return std::make_pair(x,E);

      if (x.left() == y.left())
      {
         Interval z(y.right(),x.right());
         return std::make_pair(z,E);
      }
      else if (x.right() == y.right())
      {
         Interval z(x.left(),y.left());
         return std::make_pair(z,E);         
      }
      else
      {
         Interval u(x.left(),y.left());
         Interval v(y.right(),x.right());
         return std::make_pair(u,v);
      }
   }

   if (y.contains(x.left()))
   {
      Interval z(y.right(),x.right());
      return std::make_pair(z,E);
   }

   if (y.contains(x.right()))
   {
      Interval z(x.left(),y.left());
      return std::make_pair(z,E);
   }

   // last case: x and y are disjoint
   return std::make_pair(x,E);
}

std::pair<Interval,Interval> extDiv(const Interval& x, const Interval& y)
{
   if (y.strictlyContainsZero())
   {
      Interval z1( x / (y & Interval::negative()) ),
               z2( x / (y & Interval::positive()) );

      if (z1.isDisjoint(z2))
      {
         if (z1.isCertainlyLt(z2))
            return std::make_pair(z1,z2);
         else
            return std::make_pair(z2,z1);
      }
      else
         return std::make_pair(z1|z2,Interval::emptyset());
   }
   else
   {
      return std::make_pair(x/y,Interval::emptyset()); 
   }
}

std::ostream& operator<<(std::ostream& os, const Interval& x)
{
   IntervalImpl::print(os,x.impl_);
   return os;
}

} // namespace
