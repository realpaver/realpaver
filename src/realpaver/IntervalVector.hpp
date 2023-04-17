///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_VECTOR_HPP
#define REALPAVER_INTERVAL_VECTOR_HPP

#include "realpaver/Bitset.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/NumericIntervalTraits.hpp"
#include "realpaver/NumericVector.hpp"
#include "realpaver/RealMatrix.hpp"
#include "realpaver/RealVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a vector of intervals.
///
/// The elements of a vector of size n are indexed from 0 to n-1.
///////////////////////////////////////////////////////////////////////////////
class IntervalVector : public NumericVector<Interval> {
public:
   /// Base class
   typedef NumericVector<Interval> BaseType;

   /// Creates a vector
   /// @param n number of elements
   /// @param x initialization value
   IntervalVector(size_t n = 0, const Interval& x = Interval::universe());

   /// Creates a vector from a list
   /// @param l list of elements inserted in this
   IntervalVector(const std::initializer_list<Interval>& l);

   /// Creates a vector
   /// @param pt real vector assigned to this
   IntervalVector(const RealVector& pt);

   /// Default copy constructor
   IntervalVector(const IntervalVector&) = default;

   /// Default assignment operator
   IntervalVector& operator=(const IntervalVector&) = default;

   /// Virtual destructor
   virtual ~IntervalVector();

   /// Equality test
   /// @param X a vector
   /// @return true if this == X and both intervals are non empty,
   ///         false otherwise
   bool equals(const IntervalVector& X) const;

   /// @param i an index between 0 and size()-1
   /// @return the i-th element of this
   Interval get(size_t i) const;

   /// Sets an element of this
   /// @param i an index between 0 and size()-1
   /// @param x an interval
   void set(size_t i, const Interval& x);

   /// Assignds this to the empty vector (assigns the first component to empty)
   void setEmpty();

   /// @return true if one element of this is empty
   bool isEmpty() const;

   /// @return true every component of this is finite and non empty
   bool isFinite() const;

   /// @return the width of this
   double width() const;

   /// @return the midpoint of this
   RealVector midpoint() const;

   /// @return the corner of this made from all the left bounds
   RealVector lCorner() const;

   /// @return the corner of this made from all the right bounds
   RealVector rCorner() const;

   /// Gets a corner of this
   /// @param bs a bitset indexed from 0 to size()-1
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the left bound of
   /// the i-th component of this if bs[i] is true, the right bound otherwise.
   RealVector corner(const Bitset& bs) const;

   /// Gets a corner of this
   /// @param bs a bitset indexed from 0 to size()-1
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the right bound of
   /// the i-th component of this if bs[i] is true, the left bound otherwise.
   RealVector oppositeCorner(const Bitset& bs) const;

   /// Set containment test
   /// @param X a vector having the same size than this
   /// @return true if each X[i] is included in this[i]
   bool contains(const IntervalVector& X) const;

   /// Set containment test
   /// @param X a vector having the same size than this
   /// @return true if each X[i] is strictly included in this[i]
   bool strictlyContains(const IntervalVector& X) const;

   /// Set containment test
   /// @param X a vector having the same size than this
   /// @return true if each X[i] belongs to this[i]
   bool contains(const RealVector& X) const;

   /// Set containment test
   /// @param X a vector having the same size than this
   /// @return true if each X[i] strictly belongs to this[i]
   bool strictlyContains(const RealVector& X) const;

   /// Set containment test
   /// @return true if this contains 0.0
   bool containsZero() const;

   /// Set containment test
   /// @return true if this strictly contains 0.0
   bool strictlyContainsZero() const;

   /// Tests if two intervals overlap
   /// @param X a vector having the same size than this
   /// @return true if this and X overlap
   bool overlaps(const IntervalVector& X) const;

   /// @return the one-norm of this
   double l1Norm() const;

   /// @return the infinite-norm of this
   double linfNorm() const;

   /// @return a clone of this
   virtual IntervalVector* clone() const;

   /// Hausdorff distance between vectors
   /// @param X an interval vector
   /// @return the maximum distance componentwise between this and X
   double distance(const IntervalVector& X) const;

   /// Gap between vectors
   /// @param X an interval vector
   /// @return the maximum gap componentwise between this and X
   double gap(const IntervalVector& X) const;

   /// Inflation method
   /// @param delta a real > 1.0
   /// @param chi a real > 0.0
   ///
   /// Let x be an interval of this and let m(x) be its midpoint.
   /// Then x is replaced by m(x) + delta*(x - m(x)) + chi*[-1,1].
   void inflate(double delta, double chi);
};

/// Multiplication
/// @param A a real matrix
/// @param X an interval vector
/// @return A*X
IntervalVector operator*(const RealMatrix& A, const IntervalVector& X);

/// Unary subtraction
/// @param X an interval vector
/// @return -X
IntervalVector operator-(const IntervalVector& X);

/// output on a stream
std::ostream& operator<<(std::ostream& os, const IntervalVector& X);

} // namespace

#endif
