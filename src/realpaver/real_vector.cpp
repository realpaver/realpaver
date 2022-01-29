///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/common.hpp"
#include "realpaver/Exception.hpp"
#include "realpaver/real_vector.hpp"

namespace realpaver {

RealVector::RealVector() : v_()
{}

RealVector::RealVector(size_t n, double x) : v_(n, x)
{}

RealVector::RealVector(const std::initializer_list<double>& l) : v_()
{
   for (auto x : l)
      v_.push_back(x);
}

double RealVector::at(size_t i) const
{
   ASSERT(i < size(), "access out of range in a real vector @ " << i);

   return v_[i];
}

void RealVector::set(size_t i, double x)
{
   ASSERT(i < size(), "access out of range in a real vector @ " << i);

   v_[i] = x;
}

void RealVector::setAll(double x)
{
   for (size_t i=0; i<size(); ++i)
      v_[i] = x;
}

size_t RealVector::hashCode() const
{
   size_t h = 0;

   if (size() != 0)
   {
      h = Hash1(v_[0]);

      for (size_t i=1; i<size(); ++i)
         h = Hash2(h, Hash1(v_[i]));
   }

   return h;
}

size_t RealVector::size() const
{
   return v_.size();
}

void RealVector::push(double x)
{
   v_.push_back(x);
}

double RealVector::operator[](size_t i) const
{
   return v_[i];
}

bool RealVector::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isInf(v_[i]))
         return false;

   return true;
}

bool RealVector::isInf() const
{
   return !isFinite();
}

bool RealVector::isNan() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isNan(v_[i]))
         return true;

   return true;
}

double RealVector::oneNorm() const
{
   double norm = 0.0;

   for (size_t i=0; i<size(); ++i)
      norm += Double::abs(v_[i]);

   return norm;
}

double RealVector::infNorm() const
{
   double norm = 0.0,
          m;

   for (size_t i=0; i<size(); ++i)
   {
      m = Double::abs(v_[i]);
      if (m > norm) norm = m;
   }

   return norm;
}

std::ostream& operator<<(std::ostream& os, const RealVector& v)
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
