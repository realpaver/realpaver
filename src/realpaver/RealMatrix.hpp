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
 * @file   RealMatrix.hpp
 * @brief  Dense real matrix
 * @author Laurent Granvilliers
 * @author Raphaël Chenouard (LU decomposition)
 * @date   2024-4-11
 */

#ifndef REALPAVER_REAL_MATRIX_HPP
#define REALPAVER_REAL_MATRIX_HPP

#include "realpaver/Double.hpp"
#include "realpaver/NumericDoubleTraits.hpp"
#include "realpaver/NumericMatrix.hpp"

namespace realpaver {

/**
 *  @brief Dense real matrix.
 * 
 *  The elements of a matrix of size (n, m) are indexed from 0 to n-1 and
 *  0 to m-1.
 */
class RealMatrix : public NumericMatrix<double> {
public:
   /// Base class
   using BaseType = NumericMatrix<double>;

   /// Creates a matrix with nrows rows and ncols columns
   RealMatrix(size_t nrows, size_t ncols, double x = 0.0);
   
   /// Creates a matrix from a list of rows
   RealMatrix(const std::initializer_list<std::initializer_list<double>>& l);

   /// Default copy constructor
   RealMatrix(const RealMatrix&) = default;

   /// Default assignment operator
   RealMatrix& operator=(const RealMatrix& V) = default;

   /// Default destructor
   ~RealMatrix() = default;

   /// Gets the coefficient (i, j)
   double get(size_t i, size_t j) const;

   /// Sets the coefficient (i, j) to x
   void set(size_t i, size_t j, double x);

   /// Returns true if at least one component is a NaN
   bool isNan() const;

   /// Return the L1-norm of this
   double l1Norm() const;

   /// Return the infinite-norm of this
   double linfNorm() const;

   /// Assigns this to the identity matrix
   void setIdentity();

   /// Returns true if this is equal to A
   bool operator==(const RealMatrix& A) const;

   /// Returns true if this is not equal to A
   bool operator!=(const RealMatrix& A) const;

   /// Addition with assignment
   RealMatrix& operator+=(const RealMatrix& A);

   /// Subtraction with assignment
   RealMatrix& operator-=(const RealMatrix& A);

   /// Multiplication with assignment
   RealMatrix& operator*=(double a);

   /// Division with assignment
   RealMatrix& operator/=(double a);

   /// Returns the transpose of this
   RealMatrix transpose() const;

   /**
    * @brief Calculates the inverse of a square matrix.
    * 
    * Returns true if this is invertible, in this case P is the inverse; returns
    * false otherwise.
    * 
    * this is modified.
    */
   bool inverse(RealMatrix& P);

   /// Returns the threshold on the values of pivots
   bool getMinPivot() const;

   /// Assigns the threshold on the values of pivots
   void setMinPivot(double val);

   /**
    * @brief Computes the LU decomposition of this.
    * 
    * this, L, and U must have the same dimension.
    */
   void LU(RealMatrix& L, RealMatrix& U) const;

   /// Checks if this is positive definite
   bool isPositiveDefinite() const;

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

/// Returns A+B
RealMatrix operator+(const RealMatrix& A, const RealMatrix& B);

/// Returns A-B
RealMatrix operator-(const RealMatrix& A, const RealMatrix& B);

/// Returns -A
RealMatrix operator-(const RealMatrix& A);

/// Returns a*B
RealMatrix operator*(double a, const RealMatrix& B);

/// Returns B*a
RealMatrix operator*(const RealMatrix& B, double a);

/// Returns A*B
RealMatrix operator*(const RealMatrix& A, const RealMatrix& B);

/// Returns B/a
RealMatrix operator/(const RealMatrix& B, double a);

} // namespace

#endif
