///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_MATRIX_HPP
#define REALPAVER_REAL_MATRIX_HPP

#include "realpaver/Double.hpp"
#include "realpaver/LinumDoubleTraits.hpp"
#include "realpaver/NumericMatrix.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a matrix of real numbers.
///
/// The elements of a matrix of size (n, m) are indexed from 0 to n-1 and
/// 0 to m-1.
///////////////////////////////////////////////////////////////////////////////
class RealMatrix : public NumericMatrix<double> {
public:
   /// Base class
   typedef NumericMatrix<double> BaseType;

   /// Creates a matrix
   /// @param nrows number of rows
   /// @param ncols number of columns
   /// @param x initialization value
   RealMatrix(size_t nrows, size_t ncols, double x = 0.0);
   
   /// Creates a matrix from a list
   /// @param l list of elements inserted in this
   RealMatrix(const std::initializer_list<std::initializer_list<double>>& l);

   /// Default copy constructor
   RealMatrix(const RealMatrix&) = default;

   /// Default assignment operator
   RealMatrix& operator=(const RealMatrix& V) = default;

   /// Default destructor
   ~RealMatrix() = default;

   /// Constant access
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @return the element at position (i, j) of this
   double get(size_t i, size_t j) const;

   /// Sets an element of this
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @param x a real number
   void set(size_t i, size_t j, double x);

   /// @return true if at least one component is a NaN
   bool isNan() const;

   /// Maximum absolute column sum norm
   /// @return the L1-norm of this
   double l1Norm() const;

   /// Maximum absolute row sum norm
   /// @return the infinite-norm of this
   double linfNorm() const;

   /// Addition with assignment
   /// @param A a matrix
   /// @return a reference to this
   ///
   /// this is assigned to this + A
   RealMatrix& operator+=(const RealMatrix& A);

   /// Subtraction with assignment
   /// @param A a matrix
   /// @return a reference to this
   ///
   /// this is assigned to this - A
   RealMatrix& operator-=(const RealMatrix& A);

   /// Multiplication with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this * a
   RealMatrix& operator*=(double a);

   /// Division with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this / a
   RealMatrix& operator/=(double a);
};

/// Output on s stream
std::ostream& operator<<(std::ostream& os, const RealMatrix& A);

///@{
/// Operators on real matrices
RealMatrix operator+(const RealMatrix& A, const RealMatrix& B);
RealMatrix operator-(const RealMatrix& A, const RealMatrix& B);
RealMatrix operator-(const RealMatrix& A);

RealMatrix operator*(double a, const RealMatrix& B);
RealMatrix operator*(const RealMatrix& B, double a);
RealMatrix operator*(const RealMatrix& A, const RealMatrix& B);

RealMatrix operator/(const RealMatrix& B, double a);
///@}

} // namespace

#endif
