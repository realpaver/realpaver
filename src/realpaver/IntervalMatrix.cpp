///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalMatrix.hpp"

namespace realpaver {

IntervalMatrix::IntervalMatrix(size_t nrows, size_t ncols,
                               const Interval& x)
      : NumericMatrix<Interval>(nrows, ncols, x)
{}

IntervalMatrix::IntervalMatrix(
   const std::initializer_list<std::initializer_list<Interval>>& l)
      :  NumericMatrix<Interval>(0, 0)
{
   size_t nrows = l.size();

   ASSERT(nrows > 0, "Bad initialization of interval matrix");

   auto it = l.begin();
   size_t ncols = it->size();

   ASSERT(ncols > 0, "Bad initialization of interval matrix");

   setNrows(nrows);
   setNcols(ncols);

   for (auto r : l)
   {
      THROW_IF(r.size() != ncols, "Bad initialization of interval matrix");

      for (Interval x : r) push(x);
   }
}

IntervalMatrix::IntervalMatrix(const RealMatrix& A)
      : NumericMatrix<Interval>(A.nrows(), A.ncols())
{
   for (size_t i=0; i<nrows(); ++i)
      for (size_t j=0; j<ncols(); ++j)
         set(i, j, A.get(i, j));
}

bool IntervalMatrix::isEmpty() const
{
   for (size_t i=0; i<nrows(); ++i)
      for (size_t j=0; j<ncols(); ++j)
         if (get(i, j).isEmpty()) return true;

   return false;
}

Interval IntervalMatrix::get(size_t i, size_t j) const
{
   return operator()(i, j);
}

void IntervalMatrix::set(size_t i, size_t j, const Interval& x)
{
   operator()(i, j) = x;
}

double IntervalMatrix::l1Norm() const
{
   double norm = 0.0;
   for (size_t j=0; j<ncols(); ++j)
   {
      double s = 0.0;

      for (size_t i=0; i<nrows(); ++i)
         s += get(i, j).mag();

      if (s > norm) norm = s;
   }
   return norm;   
}

double IntervalMatrix::linfNorm() const
{
   double norm = 0.0;
   for (size_t i=0; i<nrows(); ++i)
   {
      double s = 0.0;
      for (size_t j=0; j<ncols(); ++j)
         s += get(i, j).mag();

      if (s > norm) norm = s;
   }
   return norm;
}

RealMatrix IntervalMatrix::midpoint() const
{
   size_t n = nrows(),
          m = ncols();
   RealMatrix res(n, m);

   for (size_t i=0; i<n; ++i)
      for (size_t j=0; j<m; ++j)
         res.set(i, j, get(i, j).midpoint());

   return res;
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrix& A)
{
   A.print(os);
   return os;
}

IntervalMatrix& IntervalMatrix::operator+=(const IntervalMatrix& A)
{
   IntervalMatrix::BaseType::add(*this, A, *this);
   return *this;
}

IntervalMatrix& IntervalMatrix::operator-=(const IntervalMatrix& A)
{
   IntervalMatrix::BaseType::sub(*this, A, *this);
   return *this;   
}

IntervalMatrix& IntervalMatrix::operator*=(double a)
{
   IntervalMatrix::BaseType::mulScalar(a, *this, *this);
   return *this;
}

IntervalMatrix& IntervalMatrix::operator/=(double a)
{
   IntervalMatrix::BaseType::divScalar(*this, a, *this);
   return *this;
}

IntervalMatrix operator+(const IntervalMatrix& A, const IntervalMatrix& B)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::add(A, B, res);
   return res;
}

IntervalMatrix operator-(const IntervalMatrix& A, const IntervalMatrix& B)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::sub(A, B, res);
   return res;
}

IntervalMatrix operator-(const IntervalMatrix& A)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::usb(A, res);
   return res;
}

IntervalMatrix operator*(double a, const IntervalMatrix& B)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::mulScalar(a, B, res);
   return res;
}

IntervalMatrix operator*(const IntervalMatrix& B, double a)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::mulScalar(a, B, res);
   return res;   
}

IntervalMatrix operator/(const IntervalMatrix& B, double a)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::divScalar(B, a, res);
   return res;
}

IntervalMatrix operator*(const IntervalMatrix& A, const IntervalMatrix& B)
{
   IntervalMatrix res(A.nrows(), B.ncols());
   IntervalMatrix::BaseType::mul(A, B, res);
   return res;   
}

IntervalMatrix operator*(const RealMatrix& A, const IntervalMatrix& B)
{
   IntervalMatrix tmp(A);
   return tmp*B; 
}

} // namespace
