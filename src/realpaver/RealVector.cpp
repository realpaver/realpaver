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

} // namespace
