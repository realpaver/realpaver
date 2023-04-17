///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_MATRIX_HPP
#define REALPAVER_REAL_MATRIX_HPP

#include "realpaver/Double.hpp"
#include "realpaver/NumericDoubleTraits.hpp"
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

   /// Assigns this to the identity matrix
   void setIdentity();

   /// Equality test
   /// @param A a matrix
   /// return true if this is equal to A
   bool operator==(const RealMatrix& A) const;

   /// Disequality test
   /// @param A a matrix
   /// return true if this is not equal to A
   bool operator!=(const RealMatrix& A) const;

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

   /// Calculates the inverse of a square matrix
   /// @param P the result in case of success
   /// @return true if this is invertible
   ///
   /// This is modified.
   bool inverse(RealMatrix& P);

   /// @return the threshold on the values of pivots
   bool getMinPivot() const;

   /// Assigns the threshold on the values of pivots
   /// @param val any pivot must be strictly greater than this value
   void setMinPivot(double val);

private:
   double minpiv_;   // threshold on the values of pivots

   // first phase of inversion: calculates an upper triangular matrix
   // using Gauss-Jordan elimination
   bool elimination(RealMatrix& P);

   // second phase of inversion: back substitution
   void substitution(RealMatrix& P);

   // finds a pivot in the submatrix of this such that the coefficient
   // (i, i) is the upper left corner
   bool findPivot(size_t i, size_t& row, size_t& col);

   // divides the i-th rows in this and P by this(i, i)
   void dividePivot(size_t i, RealMatrix& P);
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
