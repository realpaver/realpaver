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

double RealVector::operator[](size_t i) const
{
   return at(i);
}

void RealVector::set(size_t i, double x)
{
   setAt(i, x);
}

RealVector& RealVector::operator=(const RealVector& V)
{
   ASSERT(size() == V.size(),
          "Assignment of vectors having different sizes");

   for (size_t i=0; i<size(); ++i)
      set(i, V[i]);

   return *this;
}

bool RealVector::isNan() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isNan(at(i))) return true;

   return false;
}

double RealVector::scalarProduct(const RealVector& V) const
{
   ASSERT(size() == V.size(),
          "Scalar product of vectors having different sizes");

   double res = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(res, Double::mul(at(i), V[i]));

   return res;
}

double RealVector::euclideanNorm() const
{
   double sq = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(sq, Double::sqr(at(i)));

   return Double::sqrt(sq);
}

double RealVector::oneNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      Double::addAssign(norm, Double::abs(at(i)));

   return norm;
}

double RealVector::infNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
   {
      double m = Double::abs(at(i));
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

RealVector operator/(const RealVector& V, double a)
{
   RealVector res(V.size());
   RealVector::BaseType::divScalar(V, a, res);
   return res;
}

} // namespace
