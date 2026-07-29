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
 * @file   IntervalMatrix.hpp
 * @brief  Dense interval matrix
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_INTERVAL_MATRIX_HPP
#define REALPAVER_INTERVAL_MATRIX_HPP

#include "realpaver/IntervalVector.hpp"
#include "realpaver/NumericIntervalTraits.hpp"
#include "realpaver/RealMatrix.hpp"
#include <vector>

namespace realpaver {

/**
 *  @brief Dense interval matrix.
 *
 *  The elements of a matrix of size (n, m) are indexed from 0 to n-1 and
 *  0 to m-1.
 */
class IntervalMatrix : public NumericMatrix<Interval> {
public:
   /// Base class
   using BaseType = NumericMatrix<Interval>;

   /// Type of lists of indexes
   using IndexList = std::vector<size_t>;

   /// Creates a matrix with nrows rows and ncols columns
   IntervalMatrix(size_t nrows, size_t ncols, const Interval &x = Interval::zero());

   /// Creates a matrix from a list of rows
   IntervalMatrix(const std::initializer_list<std::initializer_list<Interval>> &l);

   /// Copy constructor from a real matrix
   IntervalMatrix(const RealMatrix &A);

   /// Default copy constructor
   IntervalMatrix(const IntervalMatrix &) = default;

   /// Default assignment operator
   IntervalMatrix &operator=(const IntervalMatrix &V) = default;

   /// Default destructor
   ~IntervalMatrix() = default;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Gets the coefficient (i, j)
   Interval get(size_t i, size_t j) const;

   /// Sets the element (i, j) to x
   void set(size_t i, size_t j, const Interval &x);

   /// Returns the L1-norm of this
   double l1Norm() const;

   /// Returns the infinite-norm of this
   double linfNorm() const;

   /// Returns the midpoint of this
   RealMatrix midpoint() const;

   /// Returns the transpose of this
   IntervalMatrix transpose() const;

   /// Addition with assignment
   IntervalMatrix &operator+=(const IntervalMatrix &A);

   /// Subtraction with assignment
   IntervalMatrix &operator-=(const IntervalMatrix &A);

   /// Multiplication with assignment
   IntervalMatrix &operator*=(double a);

   /// Division with assignment
   IntervalMatrix &operator/=(double a);

   /**
    * @brief Selects the most sensitive variables.
    *
    * The number of rows of this must be less than the number of columns.
    *
    * Returns false if this is not invertible. Otherwise, columns is assigned to a list of
    * column indexes from the most sensitive variables to the less sensitives ones. For
    * example, if this is a rectangular matrix with n rows and m columns with n>m, columns
    * is a permutation of the set {0, 1, ..., m-1} such that the n first indexes
    * correspond to the most sensitive variables.
    */
   bool selectMostSensitive(IndexList &columns);

private:
   // finds the maximum pivit in the submatrix from coordinates (i, i)
   bool findMaxPivot(size_t i, size_t &row, size_t &col);
};

/// Output on s stream
std::ostream &operator<<(std::ostream &os, const IntervalMatrix &A);

/// Returns A+B
IntervalMatrix operator+(const IntervalMatrix &A, const IntervalMatrix &B);

/// Returns A-B
IntervalMatrix operator-(const IntervalMatrix &A, const IntervalMatrix &B);

/// Returns -A
IntervalMatrix operator-(const IntervalMatrix &A);

/// Returns a*B
IntervalMatrix operator*(double a, const IntervalMatrix &B);

/// Returns B*a
IntervalMatrix operator*(const IntervalMatrix &B, double a);

/// Returns A*B
IntervalMatrix operator*(const IntervalMatrix &A, const IntervalMatrix &B);

/// Returns A*X
IntervalVector operator*(const IntervalMatrix &A, const IntervalVector &X);

/// Returns A*X
IntervalVector operator*(const IntervalMatrix &A, const RealVector &X);

/// Returns A*B
IntervalMatrix operator*(const RealMatrix &A, const IntervalMatrix &B);

/// Returns B/a
IntervalMatrix operator/(const IntervalMatrix &B, double a);

} // namespace realpaver

#endif
