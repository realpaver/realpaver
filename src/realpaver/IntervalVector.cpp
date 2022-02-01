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

IntervalVector::IntervalVector() : NumericVector<Interval>()
{}

IntervalVector::IntervalVector(size_t n, const Interval& x)
      : NumericVector<Interval>(n, x)
{}

IntervalVector::IntervalVector(const std::initializer_list<Interval>& l)
      : NumericVector<Interval>()
{
   for (auto x : l)
      push(x);
}

bool IntervalVector::isEmpty() const
{
   for (size_t i=0; i<size(); ++i)
      if (elemAt(i).isEmpty())
         return true;

   return false;
}

double IntervalVector::width() const
{
   double wid = 0.0, w;

   for (size_t i=0; i<size(); ++i)
   {
      w = elemAt(i).width();
      if (w > wid) wid = w;
   }

   return wid;
}

RealVector IntervalVector::midpoint() const
{
   RealVector V( size() );
   for (size_t i=0; i<size(); ++i)
      V.set(i, elemAt(i).midpoint());
   return V;
}

RealVector IntervalVector::lCorner() const
{
   RealVector V( size() );
   for (size_t i=0; i<size(); ++i)
      V.set(i, elemAt(i).left());
   return V;
}

RealVector IntervalVector::rCorner() const
{
   RealVector V( size() );
   for (size_t i=0; i<size(); ++i)
      V.set(i, elemAt(i).right());
   return V;
}

bool IntervalVector::contains(const IntervalVector& V) const
{
   ASSERT(size() == V.size(),
          "inclusion test of two interval vectors with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!elemAt(i).contains(V[i]))
         return false;

   return true;
}

bool IntervalVector::strictlyContains(const IntervalVector& V) const
{
   ASSERT(size() == V.size(),
          "inclusion test of two interval vectors with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!elemAt(i).strictlyContains(V[i]))
         return false;

   return true;
}

bool IntervalVector::containsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!elemAt(i).containsZero())
         return false;

   return true;
}

bool IntervalVector::strictlyContainsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!elemAt(i).strictlyContainsZero())
         return false;

   return true;
}

bool IntervalVector::isDisjoint(const IntervalVector& V) const
{
   ASSERT(size() == V.size(),
          "disjunction test of two interval boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (elemAt(i).isDisjoint(V[i]))
         return true;

   return false;
}

bool IntervalVector::overlaps(const IntervalVector& V) const
{
   ASSERT(size() == V.size(),
          "intersection test of two interval vectors with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!elemAt(i).overlaps(V[i]))
         return false;

   return true;
}

double IntervalVector::oneNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(norm, elemAt(i).mag());

   return norm;
}

double IntervalVector::infNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
   {
      double m = elemAt(i).mag();
      if (m > norm) norm = m;
   }

   return norm;
}

IntervalVector& IntervalVector::operator&=(const IntervalVector& V)
{
   ASSERT(size() == V.size(),
          "intersection of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      set(i, elemAt(i) & V[i]);

   return *this;
}

IntervalVector operator&(const IntervalVector& V, const IntervalVector& W)
{
   IntervalVector res( V );
   res &= W;
   return res;
}

IntervalVector& IntervalVector::operator|=(const IntervalVector& V)
{
   ASSERT(size() == V.size(), "hull of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      set(i, elemAt(i) | V[i]);

   return *this;   
}

IntervalVector operator|(const IntervalVector& V, const IntervalVector& W)
{
   IntervalVector res( V );
   res |= W;
   return res;
}

std::ostream& operator<<(std::ostream& os, const IntervalVector& v)
{
   os << "(";
   for (size_t i=0; i<v.size(); ++i)
   {
      if (i != 0) os << ", ";
      os << v[i];
   }
   os << ")";
   return os;
}

} // namespace
