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
 * @file   IntervalMatrix.cpp
 * @brief  Dense interval matrix
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

IntervalMatrix::IntervalMatrix(size_t nrows, size_t ncols, const Interval &x)
    : NumericMatrix<Interval>(nrows, ncols, x)
{
}

IntervalMatrix::IntervalMatrix(
    const std::initializer_list<std::initializer_list<Interval>> &l)
    : NumericMatrix<Interval>(0, 0)
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

      for (Interval x : r)
         push(x);
   }
}

IntervalMatrix::IntervalMatrix(const RealMatrix &A)
    : NumericMatrix<Interval>(A.nrows(), A.ncols())
{
   for (size_t i = 0; i < nrows(); ++i)
      for (size_t j = 0; j < ncols(); ++j)
         set(i, j, A.get(i, j));
}

IntervalMatrix IntervalMatrix::transpose() const
{
   IntervalMatrix A(ncols(), nrows());

   for (size_t i = 0; i < nrows(); ++i)
      for (size_t j = 0; j < ncols(); ++j)
         A.set(j, i, get(i, j));

   return A;
}

bool IntervalMatrix::isEmpty() const
{
   for (size_t i = 0; i < nrows(); ++i)
      for (size_t j = 0; j < ncols(); ++j)
         if (get(i, j).isEmpty())
            return true;

   return false;
}

Interval IntervalMatrix::get(size_t i, size_t j) const
{
   return operator()(i, j);
}

void IntervalMatrix::set(size_t i, size_t j, const Interval &x)
{
   operator()(i, j) = x;
}

double IntervalMatrix::l1Norm() const
{
   double norm = 0.0;
   for (size_t j = 0; j < ncols(); ++j)
   {
      double s = 0.0;

      for (size_t i = 0; i < nrows(); ++i)
         s += get(i, j).mag();

      if (s > norm)
         norm = s;
   }
   return norm;
}

double IntervalMatrix::linfNorm() const
{
   double norm = 0.0;
   for (size_t i = 0; i < nrows(); ++i)
   {
      double s = 0.0;
      for (size_t j = 0; j < ncols(); ++j)
         s += get(i, j).mag();

      if (s > norm)
         norm = s;
   }
   return norm;
}

RealMatrix IntervalMatrix::midpoint() const
{
   size_t n = nrows(), m = ncols();
   RealMatrix res(n, m);

   for (size_t i = 0; i < n; ++i)
      for (size_t j = 0; j < m; ++j)
         res.set(i, j, get(i, j).midpoint());

   return res;
}

std::ostream &operator<<(std::ostream &os, const IntervalMatrix &A)
{
   A.print(os);
   return os;
}

IntervalMatrix &IntervalMatrix::operator+=(const IntervalMatrix &A)
{
   IntervalMatrix::BaseType::add(*this, A, *this);
   return *this;
}

IntervalMatrix &IntervalMatrix::operator-=(const IntervalMatrix &A)
{
   IntervalMatrix::BaseType::sub(*this, A, *this);
   return *this;
}

IntervalMatrix &IntervalMatrix::operator*=(double a)
{
   IntervalMatrix::BaseType::mulScalar(a, *this, *this);
   return *this;
}

IntervalMatrix &IntervalMatrix::operator/=(double a)
{
   IntervalMatrix::BaseType::divScalar(*this, a, *this);
   return *this;
}

IntervalMatrix operator+(const IntervalMatrix &A, const IntervalMatrix &B)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::add(A, B, res);
   return res;
}

IntervalMatrix operator-(const IntervalMatrix &A, const IntervalMatrix &B)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::sub(A, B, res);
   return res;
}

IntervalMatrix operator-(const IntervalMatrix &A)
{
   IntervalMatrix res(A.nrows(), A.ncols());
   IntervalMatrix::BaseType::usb(A, res);
   return res;
}

IntervalMatrix operator*(double a, const IntervalMatrix &B)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::mulScalar(a, B, res);
   return res;
}

IntervalMatrix operator*(const IntervalMatrix &B, double a)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::mulScalar(a, B, res);
   return res;
}

IntervalMatrix operator/(const IntervalMatrix &B, double a)
{
   IntervalMatrix res(B.nrows(), B.ncols());
   IntervalMatrix::BaseType::divScalar(B, a, res);
   return res;
}

IntervalMatrix operator*(const IntervalMatrix &A, const IntervalMatrix &B)
{
   IntervalMatrix res(A.nrows(), B.ncols());
   IntervalMatrix::BaseType::mul(A, B, res);
   return res;
}

IntervalMatrix operator*(const RealMatrix &A, const IntervalMatrix &B)
{
   IntervalMatrix tmp(A);
   return tmp * B;
}

IntervalVector operator*(const IntervalMatrix &A, const IntervalVector &X)
{
   ASSERT(A.ncols() == X.size(), "Bad dimensions in a product of a matric and a vector");

   IntervalVector Y(A.nrows());
   for (size_t i = 0; i < A.nrows(); ++i)
   {
      Interval z(0.0);
      for (size_t j = 0; j < A.ncols(); ++j)
         z += A.get(i, j) * X[j];

      Y.set(i, z);
   }
   return Y;
}

IntervalVector operator*(const IntervalMatrix &A, const RealVector &X)
{
   IntervalVector tmp(X);
   return A * tmp;
}

} // namespace realpaver
