///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/RealMatrix.hpp"

namespace realpaver {

RealMatrix::RealMatrix(size_t nrows, size_t ncols, double x)
      : NumericMatrix<double>(nrows, ncols, x),
        minpiv_(Param::GetDblParam("GAUSSIAN_MIN_PIVOT"))
{}

RealMatrix::RealMatrix(
   const std::initializer_list<std::initializer_list<double>>& l)
      : NumericMatrix<double>(0, 0),
        minpiv_(Param::GetDblParam("GAUSSIAN_MIN_PIVOT"))
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

void RealMatrix::setIdentity()
{
   ASSERT(isSquare(),
          "Bad assignment of the identoity matrix to a non-square matrix");

   setAll(0.0);
   for (size_t i=0; i<nrows(); ++i)
      set(i, i, 1.0);
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

bool RealMatrix::inverse(RealMatrix& P)
{
   ASSERT(isSquare(), "Inversion of a non-square matrix");
   ASSERT(nrows() == P.nrows(), "Bad dimension opf an inverse matrix");
   ASSERT(ncols() == P.ncols(), "Bad dimension opf an inverse matrix");

   P.setIdentity();

   bool b = elimination(P);
   if (b) substitution(P);
   return b;
}

bool RealMatrix::elimination(RealMatrix& P)
{
   size_t row, col;

   for (size_t i=0; i<nrows(); ++i)
   {
      bool b = findPivot(i, row, col);
      if (!b) return false;

      // swaps rows and columns in order to get the pivot at (i, i)
      if (i != row)
      {
         swapRows(i, row);
         P.swapRows(i, row);
      }
      if (i != col)
      {
         swapCols(i, col);
         P.swapCols(i, col);
      }

      double pivot = get(i, i);

      // puts zeros in the column under the pivot
      for (size_t j=i+1; j<nrows(); ++j)
      {
         double c = get(j, i) / pivot;

         for (size_t k=0; k<ncols(); ++k)
         {
            double val = P.get(j, k) - c*P.get(i, k);
            P.set(j, k, val);
         }

         for (size_t k=i+1; k<ncols(); ++k)
         {
            double val = get(j, k) - c*get(i, k);
            set(j, k, val);
         }

         set(j, i, 0.0);
      }

      // divides the i-th rows by the pivot
      dividePivot(i, P);
   }

   return true;
}

void RealMatrix::dividePivot(size_t i, RealMatrix& P)
{
   double pivot = get(i, i);
   if (pivot == 1.0) return;

   set(i, i, 1.0);

   for (size_t k=i+1; k<ncols(); ++k)
      set(i, k, get(i, k) / pivot);

   for (size_t k=0; k<ncols(); ++k)
      P.set(i, k, P.get(i, k) / pivot);
}

void RealMatrix::substitution(RealMatrix& P)
{
   for (int k=(int)ncols()-1; k>0; --k)
   {
      for (int i=k-1; i>=0; --i)
      {
         double c = get(i, k);

         for (int j=i+1; j<(int)ncols(); ++j)
         {
            double val = get(i, j) - c*get(k, j);
            set(i, j, val);
         }

         set(i, k, 0.0);

         for (int j=0; j<(int)ncols(); ++j)
         {
            double val = P.get(i, j) - c*P.get(k, j);
            P.set(i, j, val);
         }
      }
   }
}

bool RealMatrix::findPivot(size_t i, size_t& row, size_t& col)
{
   bool found = false;
   col = i;

   while (!found && col < ncols())
   {
      double pivot = std::abs(get(i, col));
      row = i;

      // finds the maximal pivot in the column
      for (size_t k = i+1; k < nrows(); ++k)
      {
         double val = std::abs(get(k, col));
         if (val > pivot)
         {
            row = k;
            pivot = val;
         }
      }

      if (pivot > minpiv_)
         found = true;

      else
         ++col;
   }

   return found;
}

bool RealMatrix::getMinPivot() const
{
   return minpiv_;
}

void RealMatrix::setMinPivot(double val)
{
   ASSERT(val >= 0.0, "Bad threshold on the values of pivots");
   minpiv_ = val;
}

bool RealMatrix::operator==(const RealMatrix& A) const
{
   if (nrows() != A.nrows()) return false;
   if (ncols() != A.ncols()) return false;

   for (size_t i=0; i<nrows(); ++i)
      for (size_t j=0; j<ncols(); ++j)
         if (get(i, j) != A.get(i, j))
            return false;

   return true;
}

bool RealMatrix::operator!=(const RealMatrix& A) const
{
   return !operator==(A);
}

} // namespace
