///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/RealMatrix.hpp"

namespace realpaver {

RealMatrix::RealMatrix(size_t nrows, size_t ncols, double x)
      : NumericMatrix<double>(nrows, ncols, x)
{}

RealMatrix::RealMatrix(
   const std::initializer_list<std::initializer_list<double>>& l)
      :  NumericMatrix<double>(0, 0)
{
   size_t nrows = l.size();

   ASSERT(nrows > 0, "Bad initialization of real matrix");

   auto it = l.begin();
   size_t ncols = it->size();

   ASSERT(ncols > 0, "Bad initialization of real matrix");

   setNrows(nrows);
   setNcols(ncols);

   for (auto r : l)
   {
      THROW_IF(r.size() != ncols, "Bad initialization of real matrix");

      for (double x : r) push(x);
   }
}

double RealMatrix::get(size_t i, size_t j) const
{
   return operator()(i, j);
}

void RealMatrix::set(size_t i, size_t j, double x)
{
   operator()(i, j) = x;
}

bool RealMatrix::isNan() const
{
   for (size_t i=0; i<nrows(); ++i)
      for (size_t j=0; j<ncols(); ++j)
         if (Double::isNan(get(i, j))) return true;

   return false;
}

double RealMatrix::l1Norm() const
{
   double norm = 0.0;
   for (size_t j=0; j<ncols(); ++j)
   {
      double s = 0.0;

      for (size_t i=0; i<nrows(); ++i)
         Double::addAssign(s, Double::abs(get(i, j)));

      if (s > norm) norm = s;
   }
   return norm;   
}

double RealMatrix::linfNorm() const
{
   double norm = 0.0;
   for (size_t i=0; i<nrows(); ++i)
   {
      double s = 0.0;
      for (size_t j=0; j<ncols(); ++j)
         Double::addAssign(s, Double::abs(get(i, j)));

      if (s > norm) norm = s;
   }
   return norm;
}

std::ostream& operator<<(std::ostream& os, const RealMatrix& A)
{
   A.print(os);
   return os;
}

RealMatrix& RealMatrix::operator+=(const RealMatrix& A)
{
   RealMatrix::BaseType::add(*this, A, *this);
   return *this;
}

RealMatrix& RealMatrix::operator-=(const RealMatrix& A)
{
   RealMatrix::BaseType::sub(*this, A, *this);
   return *this;   
}

RealMatrix& RealMatrix::operator*=(double a)
{
   RealMatrix::BaseType::mulScalar(a, *this, *this);
   return *this;
}

RealMatrix& RealMatrix::operator/=(double a)
{
   RealMatrix::BaseType::divScalar(*this, a, *this);
   return *this;
}

RealMatrix operator+(const RealMatrix& A, const RealMatrix& B)
{
   RealMatrix res(A.nrows(), A.ncols());
   RealMatrix::BaseType::add(A, B, res);
   return res;
}

RealMatrix operator-(const RealMatrix& A, const RealMatrix& B)
{
   RealMatrix res(A.nrows(), A.ncols());
   RealMatrix::BaseType::sub(A, B, res);
   return res;
}

RealMatrix operator-(const RealMatrix& A)
{
   RealMatrix res(A.nrows(), A.ncols());
   RealMatrix::BaseType::usb(A, res);
   return res;
}

RealMatrix operator*(double a, const RealMatrix& B)
{
   RealMatrix res(B.nrows(), B.ncols());
   RealMatrix::BaseType::mulScalar(a, B, res);
   return res;
}

RealMatrix operator*(const RealMatrix& B, double a)
{
   RealMatrix res(B.nrows(), B.ncols());
   RealMatrix::BaseType::mulScalar(a, B, res);
   return res;   
}

RealMatrix operator/(const RealMatrix& B, double a)
{
   RealMatrix res(B.nrows(), B.ncols());
   RealMatrix::BaseType::divScalar(B, a, res);
   return res;
}

RealMatrix operator*(const RealMatrix& A, const RealMatrix& B)
{
   RealMatrix res(A.nrows(), B.ncols());
   RealMatrix::BaseType::mul(A, B, res);
   return res;   
}

} // namespace
