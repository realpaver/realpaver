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
 * @file   IntervalVector.hpp
 * @brief  Vector of intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_VECTOR_HPP
#define REALPAVER_INTERVAL_VECTOR_HPP

#include "realpaver/Bitset.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/NumericIntervalTraits.hpp"
#include "realpaver/NumericVector.hpp"
#include "realpaver/RealMatrix.hpp"
#include "realpaver/RealVector.hpp"

namespace realpaver {

/**
 * @brief Vector of intervals.
 *
 * The elements of a vector of size n are indexed from 0 to n-1.
 */
class IntervalVector : public NumericVector<Interval> {
public:
   /// Base class
   using BaseType = NumericVector<Interval>;

   /// Creates a vector with n elements assigned to x
   IntervalVector(size_t n = 0, const Interval &x = Interval::universe());

   /// Creates a vector from a list
   IntervalVector(const std::initializer_list<Interval> &l);

   /// Copy constructor
   IntervalVector(const RealVector &pt);

   /// Default copy constructor
   IntervalVector(const IntervalVector &) = default;

   /// Default assignment operator
   IntervalVector &operator=(const IntervalVector &) = default;

   /// Virtual destructor
   virtual ~IntervalVector();

   /// Returns true if this == X
   bool equals(const IntervalVector &X) const;

   /// Returns the i-th element of this
   Interval get(size_t i) const;

   /// Sets the i-th element of this to x
   void set(size_t i, const Interval &x);

   /// Assigns this to the empty vector (assigns the first component to empty)
   void setEmpty();

   /// Returns true if one element of this is empty
   bool isEmpty() const;

   /// Returns true every component of this is finite and non empty
   bool isFinite() const;

   /// Returns the width of this
   double width() const;

   /// Returns the midpoint of this
   RealVector midpoint() const;

   /// Returns the corner of this made from all the left bounds
   RealVector lCorner() const;

   /// Returns the corner of this made from all the right bounds
   RealVector rCorner() const;

   /**
    * @brief Gets a corner of this.
    *
    * Let res be the result. For each i, res[i] is equal to the left bound of
    * the i-th component of this if bs[i] is true, the right bound otherwise.
    */
   RealVector corner(const Bitset &bs) const;

   /**
    * @brief Gets a corner of this.
    *
    * Let res be the result. For each i, res[i] is equal to the right bound of
    * the i-th component of this if bs[i] is true, the left bound otherwise.
    */
   RealVector oppositeCorner(const Bitset &bs) const;

   /// Returns true if each X[i] is included in this[i]
   bool contains(const IntervalVector &X) const;

   /// Returns true if each X[i] is strictly included in this[i]
   bool strictlyContains(const IntervalVector &X) const;

   /// Return true if each X[i] belongs to this[i]
   bool contains(const RealVector &X) const;

   /// Returns true if each X[i] strictly belongs to this[i]
   bool strictlyContains(const RealVector &X) const;

   /// Returns true if this contains 0.0
   bool containsZero() const;

   /// Returns true if this strictly contains 0.0
   bool strictlyContainsZero() const;

   /// Returns true if this and X overlap
   bool overlaps(const IntervalVector &X) const;

   /// Returns the one-norm of this
   double l1Norm() const;

   /// Returns the infinite-norm of this
   double linfNorm() const;

   /// Returns a clone of this
   virtual IntervalVector *clone() const;

   /// Returns the Hausdorff distance between this and X
   double distance(const IntervalVector &X) const;

   /// Returns the maximum gap componentwise between this and X
   double gap(const IntervalVector &X) const;

   /**
    * @brief Inflation method.
    *
    * Let delta > 1.0 and let chi > 0.0. Let x be an element of this and let
    * m(x) be its midpoint. Then x is replaced by
    * m(x) + delta*(x - m(x)) + chi*[-1,1].
    */
   void inflate(double delta, double chi);
};

// Returns X*Y
Interval operator*(const IntervalVector &X, const IntervalVector &Y);

// Returns X*Y
Interval operator*(const RealVector &X, const IntervalVector &Y);

// Returns X*Y
Interval operator*(const IntervalVector &X, const RealVector &Y);

/// Returns A*X
IntervalVector operator*(const RealMatrix &A, const IntervalVector &X);

/// Returns -X
IntervalVector operator-(const IntervalVector &X);

/// Returns X-Y
IntervalVector operator-(const IntervalVector &X, const IntervalVector &Y);

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const IntervalVector &X);

} // namespace realpaver

#endif
