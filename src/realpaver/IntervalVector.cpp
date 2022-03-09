///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/IntervalVector.hpp"

namespace realpaver {

IntervalVector::IntervalVector(size_t n, const Interval& x)
      : NumericVector<Interval>(n, x)
{}

IntervalVector::IntervalVector(const std::initializer_list<Interval>& l)
      : NumericVector<Interval>()
{
   for (auto x : l) push(x);
}

IntervalVector::~IntervalVector()
{}

Interval IntervalVector::get(size_t i) const
{
   return operator[](i);
}

void IntervalVector::set(size_t i, const Interval& x)
{
   operator[](i) = x;
}

bool IntervalVector::isEmpty() const
{
   for (size_t i=0; i<size(); ++i)
      if (get(i).isEmpty()) return true;

   return false;
}

double IntervalVector::width() const
{
   double wid = 0.0, w;

   for (size_t i=0; i<size(); ++i)
   {
      w = get(i).width();
      if (w > wid) wid = w;
   }

   return wid;
}

RealVector IntervalVector::midpoint() const
{
   RealVector mid(size());
   toMidpoint(mid);
   return mid;
}

void IntervalVector::toMidpoint(RealVector& mid) const
{
   ASSERT(size() == mid.size(),
          "Midpoint vector with a bad size " << mid.size());

   for (size_t i=0; i<size(); ++i)
      mid.set(i, get(i).midpoint());
}

RealVector IntervalVector::lCorner() const
{
   RealVector lc(size());

   for (size_t i=0; i<size(); ++i)
      lc.set(i, get(i).left());

   return lc;
}

RealVector IntervalVector::rCorner() const
{
   RealVector rc(size());

   for (size_t i=0; i<size(); ++i)
      rc.set(i, get(i).right());

   return rc;
}

RealVector IntervalVector::corner(const Bitset& bs) const
{
   ASSERT(size() <= bs.size(), "Bad access to the corner of " << (*this) <<
                               " given a bitset " << bs);

   RealVector co(size());

   for (size_t i=0; i<size(); ++i)
      if (bs.get(i))
         co.set(i, get(i).left());
      else
         co.set(i, get(i).right());

   return co;
}

RealVector IntervalVector::oppositeCorner(const Bitset& bs) const
{
   ASSERT(size() <= bs.size(), "Bad access to the corner of " << (*this) <<
                               " given a bitset " << bs);

   RealVector co(size());

   for (size_t i=0; i<size(); ++i)
      if (bs.get(i))
         co.set(i, get(i).right());
      else
         co.set(i, get(i).left());

   return co;
}

bool IntervalVector::contains(const IntervalVector& X) const
{
   ASSERT(size() >= X.size(), "Bad test " << (*this) << " contains " << X);

   for (size_t i=0; i<X.size(); ++i)
      if (!get(i).contains(X[i])) return false;

   return true;
}

bool IntervalVector::strictlyContains(const IntervalVector& X) const
{
   ASSERT(size() >= X.size(), "Bad test " << (*this) <<
                              " strictly contains " << X);

   for (size_t i=0; i<X.size(); ++i)
      if (!get(i).strictlyContains(X[i])) return false;

   return true;
}

bool IntervalVector::contains(const RealVector& X) const
{
   ASSERT(size() >= X.size(), "Bad test " << (*this) << " contains " << X);

   for (size_t i=0; i<X.size(); ++i)
      if (!get(i).contains(X[i])) return false;

   return true;
}

bool IntervalVector::strictlyContains(const RealVector& X) const
{
   ASSERT(size() >= X.size(), "Bad test " << (*this) <<
                              " strictly contains " << X);

   for (size_t i=0; i<X.size(); ++i)
      if (!get(i).strictlyContains(X[i])) return false;

   return true;
}

bool IntervalVector::containsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!get(i).containsZero()) return false;

   return true;
}

bool IntervalVector::strictlyContainsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!get(i).strictlyContainsZero()) return false;

   return true;
}

bool IntervalVector::isDisjoint(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad test " << (*this) <<
                              " is disjoint from " << X);

   for (size_t i=0; i<size(); ++i)
      if (get(i).isDisjoint(X[i])) return true;

   return false;
}

bool IntervalVector::overlaps(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad test " << (*this) << " overlaps " << X);

   for (size_t i=0; i<size(); ++i)
      if (!get(i).overlaps(X[i])) return false;

   return true;
}

double IntervalVector::oneNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(norm, get(i).mag());

   return norm;
}

double IntervalVector::infNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
   {
      double m = get(i).mag();
      if (m > norm) norm = m;
   }

   return norm;
}

void IntervalVector::interAssign(const IntervalVector& X)
{
   ASSERT(size() == X.size(), "Bad assignment " << (*this) << " &= " << X);

   for (size_t i=0; i<size(); ++i)
      set(i, get(i) & X[i]);
}

void IntervalVector::hullAssign(const IntervalVector& X)
{
   ASSERT(size() == X.size(), "Bad assignment " << (*this) << " |= " << X);

   for (size_t i=0; i<size(); ++i)
      set(i, get(i) | X[i]);
}

IntervalVector* IntervalVector::clone() const
{
   return new IntervalVector(*this);
}

} // namespace
