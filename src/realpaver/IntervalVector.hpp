///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_VECTOR_HPP
#define REALPAVER_INTERVAL_VECTOR_HPP

#include "realpaver/Bitset.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/LinumIntervalTraits.hpp"
#include "realpaver/NumericVector.hpp"
#include "realpaver/RealVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a vector of intervals.
///
/// The elements of a vector of size n are indexed from 0 to n-1.
///////////////////////////////////////////////////////////////////////////////
class IntervalVector : public NumericVector<Interval> {
public:
   /// Creates a vector
   /// @param n number of elements
   /// @param x initialization value
   IntervalVector(size_t n = 0, const Interval& x = Interval::universe());

   /// Creates a vector from a list
   /// @param l list of elements inserted in this
   IntervalVector(const std::initializer_list<Interval>& l);

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

   /// @return true if one element of this is empty
   bool isEmpty() const;

   /// @return the width of this
   double width() const;

   /// @return the midpoint of this
   RealVector midpoint() const;

   /// @return the corner of this made from all the left bounds
   RealVector lCorner() const;

   /// @return the corner of this made from all the right bounds
   RealVector rCorner() const;

   /// Gets a corner of this
   /// @param bs a bitset having the same size than this
   /// @return a corner of this defined by 'bs'
   ///
   /// Let res be the result. For each i, res[i] is equal to the left bound of
   /// the i-th component of this if bs[i] is true, the right bound otherwise.
   RealVector corner(const Bitset& bs) const;

   /// Gets a corner of this
   /// @param bs a bitset having the same size than this
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
};

} // namespace

#endif
