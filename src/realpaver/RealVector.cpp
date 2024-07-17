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
 * @file   RealVector.cpp
 * @brief  Real vector
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealVector.hpp"

namespace realpaver {

RealVector::RealVector(size_t n, double x) : NumericVector<double>(n, x)
{}

RealVector::RealVector(const std::initializer_list<double>& l)
      : NumericVector<double>()
{
   for (auto x : l)
      push(x);
}

double RealVector::get(size_t i) const
{
   return operator[](i);
}

void RealVector::set(size_t i, double x)
{
   operator[](i) = x;
}

bool RealVector::isNan() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isNan(get(i))) return true;

   return false;
}

void RealVector::setNan()
{
   set(0, Double::nan());
}

bool RealVector::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isNan(get(i)) || Double::isInf(get(i))) return false;

   return true;
}

double RealVector::scalarProduct(const RealVector& V) const
{
   ASSERT(size() == V.size(),
          "Scalar product of vectors having different sizes");

   double res = Double::mul(get(0), V[0]);

   for (size_t i=1; i<size(); ++i)
      Double::addAssign(res, Double::mul(get(i), V[i]));

   return res;
}

double RealVector::l2Norm() const
{
   double sq = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(sq, Double::sqr(get(i)));

   return Double::sqrt(sq);
}

double RealVector::l1Norm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(norm, Double::abs(get(i)));

   return norm;
}

double RealVector::linfNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
   {
      double m = Double::abs(get(i));
      if (m > norm) norm = m;
   }

   return norm;
}

RealVector& RealVector::operator+=(const RealVector& V)
{
   RealVector::BaseType::add(*this, V, *this);
   return *this;
}

RealVector& RealVector::operator-=(const RealVector& V)
{
   RealVector::BaseType::sub(*this, V, *this);
   return *this;
}

RealVector& RealVector::operator*=(double a)
{
   RealVector::BaseType::mulScalar(a, *this, *this);
   return *this;
}

RealVector& RealVector::operator/=(double a)
{
   RealVector::BaseType::divScalar(*this, a, *this);
   return *this;
}

RealVector operator+(const RealVector& V, const RealVector& W)
{
   RealVector res(V.size());
   RealVector::BaseType::add(V, W, res);
   return res;
}

RealVector operator+(const RealVector& V, double w)
{
   RealVector res(V.size());
   for (size_t i=0; i<V.size(); ++i)
   {
      res.set(i, V.get(i)+w);
   }
   return res;
}

RealVector operator-(const RealVector& V, const RealVector& W)
{
   RealVector res(V.size());
   RealVector::BaseType::sub(V, W, res);
   return res;
}

RealVector operator-(const RealVector& V)
{
   RealVector res(V.size());
   RealVector::BaseType::usb(V, res);
   return res;
}

RealVector operator*(double a, const RealVector& V)
{
   RealVector res(V.size());
   RealVector::BaseType::mulScalar(a, V, res);
   return res;
}

RealVector operator*(const RealVector& V, double a)
{
   return a*V;
}

RealVector operator*(const RealMatrix& A, const RealVector& X)
{
   ASSERT(A.ncols() == X.size(), "Bad dimensions " << A << " * " << X);

   RealVector Y(A.nrows());

   for (size_t i=0; i<Y.size(); ++i)
   {
      double z = 0.0;

      for (size_t j=0; j<A.ncols(); ++j)
         z += A.get(i, j) * X.get(j);

      Y.set(i, z);
   }

   return Y;
}

RealVector operator/(const RealVector& V, double a)
{
   RealVector res(V.size());
   RealVector::BaseType::divScalar(V, a, res);
   return res;
}

RealVector* RealVector::clone() const
{
   return new RealVector(*this);
}

std::ostream&
operator<<(std::ostream& os, const RealVector& V)
{
   V.print(os);
   return os;
}

} // namespace
