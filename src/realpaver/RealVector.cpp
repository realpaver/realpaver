///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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

   double res = 0.0;

   for (size_t i=0; i<size(); ++i)
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

RealVector operator*(const RealVector& V,double a)
{
   return a*V;
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


} // namespace
