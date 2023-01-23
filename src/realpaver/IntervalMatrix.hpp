///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_MATRIX_HPP
#define REALPAVER_INTERVAL_MATRIX_HPP

#include "realpaver/LinumIntervalTraits.hpp"
#include "realpaver/RealMatrix.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a matrix of intervals.
///
/// The elements of a matrix of size (n, m) are indexed from 0 to n-1 and
/// 0 to m-1.
///////////////////////////////////////////////////////////////////////////////
class IntervalMatrix : public NumericMatrix<Interval> {
public:
   /// Base class
   typedef NumericMatrix<Interval> BaseType;

   /// Creates a matrix
   /// @param nrows number of rows
   /// @param ncols number of columns
   /// @param x initialization value
   IntervalMatrix(size_t nrows, size_t ncols,
                  const Interval& x = Interval::zero());
   
   /// Creates a matrix from a list
   /// @param l list of elements inserted in this
   IntervalMatrix(const std::initializer_list<
                           std::initializer_list<Interval>>& l);

   /// Creates a matrix
   /// @param A matrix copied in this
   IntervalMatrix(const RealMatrix& A);

   /// Default copy constructor
   IntervalMatrix(const IntervalMatrix&) = default;

   /// Default assignment operator
   IntervalMatrix& operator=(const IntervalMatrix& V) = default;

   /// Default destructor
   ~IntervalMatrix() = default;

   /// @return true if this is empty
   bool isEmpty() const;

   /// Constant access
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @return the element at position (i, j) of this
   Interval get(size_t i, size_t j) const;

   /// Sets an element of this
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @param x an interval
   void set(size_t i, size_t j, const Interval& x);

   /// Maximum absolute column sum norm
   /// @return the L1-norm of this
   double l1Norm() const;

   /// Maximum absolute row sum norm
   /// @return the infinite-norm of this
   double linfNorm() const;

   /// @return the midpoint of this
   RealMatrix midpoint() const;

   /// Addition with assignment
   /// @param A a matrix
   /// @return a reference to this
   ///
   /// this is assigned to this + A
   IntervalMatrix& operator+=(const IntervalMatrix& A);

   /// Subtraction with assignment
   /// @param A a matrix
   /// @return a reference to this
   ///
   /// this is assigned to this - A
   IntervalMatrix& operator-=(const IntervalMatrix& A);

   /// Multiplication with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this * a
   IntervalMatrix& operator*=(double a);

   /// Division with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this / a
   IntervalMatrix& operator/=(double a);
};

/// Output on s stream
std::ostream& operator<<(std::ostream& os, const IntervalMatrix& A);

///@{
/// Operators on real matrices
IntervalMatrix operator+(const IntervalMatrix& A, const IntervalMatrix& B);
IntervalMatrix operator-(const IntervalMatrix& A, const IntervalMatrix& B);
IntervalMatrix operator-(const IntervalMatrix& A);

IntervalMatrix operator*(double a, const IntervalMatrix& B);
IntervalMatrix operator*(const IntervalMatrix& B, double a);
IntervalMatrix operator*(const IntervalMatrix& A, const IntervalMatrix& B);

IntervalMatrix operator*(const RealMatrix& A, const IntervalMatrix& B);

IntervalMatrix operator/(const IntervalMatrix& B, double a);
///@}

} // namespace

#endif
