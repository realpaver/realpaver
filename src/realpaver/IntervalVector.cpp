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
 * @file   IntervalVector.cpp
 * @brief  Vector of intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

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

IntervalVector::IntervalVector(const RealVector& pt)
      : NumericVector<Interval>(pt.size())
{
   for (size_t i=0; i<pt.size(); ++i)
      set(i, pt[i]);
}

IntervalVector::~IntervalVector()
{}

bool IntervalVector::equals(const IntervalVector& X) const
{
   if (isEmpty() || X.isEmpty()) return false;
   if (size() != X.size()) return false;

   for (size_t i=0; i<size(); ++i)
      if (get(i).isSetNeq(X.get(i))) return false;

   return true;
}

Interval IntervalVector::get(size_t i) const
{
   return operator[](i);
}

void IntervalVector::set(size_t i, const Interval& x)
{
   operator[](i) = x;
}

void IntervalVector::setEmpty()
{
   set(0, Interval::emptyset());
}

bool IntervalVector::isEmpty() const
{
   for (size_t i=0; i<size(); ++i)
      if (get(i).isEmpty()) return true;

   return false;
}

bool IntervalVector::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (!get(i).isFinite()) return false;

   return true;
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
   
   for (size_t i=0; i<size(); ++i)
      mid.set(i, get(i).midpoint());

   return mid;
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
   ASSERT(size() == bs.size(), "Bad access to the corner of " << (*this) <<
                               " given a bitset " << bs);

   ASSERT(bs.first() == 0, "Bad access to the corner of " << (*this) <<
                           " given a bitset whose first index is not 0");

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
   ASSERT(size() == bs.size(), "Bad access to the corner of " << (*this) <<
                               " given a bitset " << bs);

   ASSERT(bs.first() == 0, "Bad access to the corner of " << (*this) <<
                           " given a bitset whose first index is not 0");

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
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   for (size_t i=0; i<size(); ++i)
      if (!get(i).contains(X[i])) return false;

   return true;
}

bool IntervalVector::strictlyContains(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   for (size_t i=0; i<size(); ++i)
      if (!get(i).strictlyContains(X[i])) return false;

   return true;
}

bool IntervalVector::contains(const RealVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   for (size_t i=0; i<size(); ++i)
      if (!get(i).contains(X[i])) return false;

   return true;
}

bool IntervalVector::strictlyContains(const RealVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   for (size_t i=0; i<size(); ++i)
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

bool IntervalVector::overlaps(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   for (size_t i=0; i<size(); ++i)
      if (!get(i).overlaps(X[i])) return false;

   return true;
}

double IntervalVector::l1Norm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(norm, get(i).mag());

   return norm;
}

double IntervalVector::linfNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
   {
      double m = get(i).mag();
      if (m > norm) norm = m;
   }

   return norm;
}

IntervalVector* IntervalVector::clone() const
{
   return new IntervalVector(*this);
}

double IntervalVector::distance(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   double d = 0.0;
   for (size_t i=0; i<size(); ++i)
   {
      double e = get(i).distance(X.get(i));
      if (e > d) d = e;
   }
   return d;
}

double IntervalVector::gap(const IntervalVector& X) const
{
   ASSERT(size() == X.size(), "Bad dimensions: " << (*this) << ", " << X);

   double gap = 0.0;
   for (size_t i=0; i<size(); ++i)
   {
      double e = get(i).gap(X.get(i));
      if (e > gap) gap = e;
   }
   return gap;
}

void IntervalVector::inflate(double delta, double chi)
{
   ASSERT(delta > 1.0, "Bad parameter delta of inflation: " << delta);
   ASSERT(chi > 0.0, "Bad parameter chi of inflation: " << chi);

   for (size_t i=0; i<size(); ++i)
      set(i, get(i).inflate(delta, chi));
}

Interval IntervalVector::scalarProduct(const IntervalVector& X) const
{
   ASSERT(size() == X.size(),
          "Scalar product of vectors having different sizes");

   Interval prod = get(0)*X.get(0);
   for (size_t i=1; i<size(); ++i)
      prod += get(i)*X.get(i);

   return prod;
}

IntervalVector operator*(const RealMatrix& A, const IntervalVector& X)
{
   ASSERT(A.ncols() == X.size(), "Bad dimensions " << A << " * " << X);

   IntervalVector Y(A.nrows());

   for (size_t i=0; i<Y.size(); ++i)
   {
      Interval z(0.0);

      for (size_t j=0; j<A.ncols(); ++j)
         z += A.get(i, j) * X.get(j);

      Y.set(i, z);
   }

   return Y;
}

IntervalVector operator-(const IntervalVector& X)
{
   IntervalVector res(X.size());
   IntervalVector::BaseType::usb(X, res);
   return res;
}

IntervalVector operator-(const IntervalVector& X, const IntervalVector& Y)
{
   IntervalVector res(X.size());
   IntervalVector::BaseType::sub(X, Y, res);
   return res;
}

std::ostream&
operator<<(std::ostream& os, const IntervalVector& X)
{
   X.print(os);
   return os;
}

} // namespace
