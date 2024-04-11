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
 * @file   RealVector.hpp
 * @brief  Real vector
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_REAL_VECTOR_HPP
#define REALPAVER_REAL_VECTOR_HPP

#include "realpaver/Double.hpp"
#include "realpaver/NumericDoubleTraits.hpp"
#include "realpaver/NumericVector.hpp"
#include "realpaver/RealMatrix.hpp"

namespace realpaver {

/**
 * @brief Vector of real numbers.
 *
 * The elements of a vector of size n are indexed from 0 to n-1.
 */
class RealVector : public NumericVector<double> {
public:
   /// Base class
   using BaseType = NumericVector<double>;

   /// Creates a vector with n elements assigned to x
   RealVector(size_t n = 0, double x = 0.0);
   
   /// Creates a vector from a list
   RealVector(const std::initializer_list<double>& l);

   /// Default copy constructor
   RealVector(const RealVector&) = default;

   /// Default assignment operator
   RealVector& operator=(const RealVector& V) = default;

   /// Default destructor
   ~RealVector() = default;

   /// Returns the i-th element of this
   double get(size_t i) const;

   /// Sets the i-th element to x
   void set(size_t i, double x);

   /// Returns true if at least one component is a NaN
   bool isNan() const;

   /// Returns true if every component is not a NaN and is finite
   bool isFinite() const;

   /// Returns the scalar product of this and V
   double scalarProduct(const RealVector& V) const;

   /// Returns the L1-norm of this
   double l1Norm() const;

   /// Returns the L2-norm (Euclidean-norm) of this
   double l2Norm() const;

   /// Returns the infinite-norm of this
   double linfNorm() const;

   /// Addition with assignment
   RealVector& operator+=(const RealVector& V);

   /// Subtraction with assignment
   RealVector& operator-=(const RealVector& V);

   /// Multiplication with assignment
   RealVector& operator*=(double a);

   /// Division with assignment
   RealVector& operator/=(double a);

   /// Returns a clone of this
   virtual RealVector* clone() const;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const RealVector& V);

/// Returns V+W
RealVector operator+(const RealVector& V, const RealVector& W);

/// Returns V+w
RealVector operator+(const RealVector& V, double w);

/// Returns V-W
RealVector operator-(const RealVector& V, const RealVector& W);

/// Returns -V
RealVector operator-(const RealVector& V);

/// Returns a*V
RealVector operator*(double a, const RealVector& V);

/// Returns V*a
RealVector operator*(const RealVector& V, double a);

/// Returns A*X
RealVector operator*(const RealMatrix& A, const RealVector& X);

/// Returns V/a
RealVector operator/(const RealVector& V, double a);

} // namespace

#endif
