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

#include "realpaver/interval.hpp"
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
   /// Creates an empty vector
   IntervalVector();

   /// Creates a vector
   /// @param n number of elements
   /// @param x initialization value
   IntervalVector(size_t n, const Interval& x = Interval::universe());
   
   /// Creates a vector from a list
   /// @param l list of elements inserted in this
   IntervalVector(const std::initializer_list<Interval>& l);

   /// Default copy constructor
   IntervalVector(const IntervalVector&) = default;

   /// No assignment
   IntervalVector& operator=(const IntervalVector&) = delete;

   /// Default destructor
   ~IntervalVector() = default;

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

   /// Set containment test
   /// @param V a vector having the same size than this
   /// @return true if this contains V
   bool contains(const IntervalVector& V) const;

   /// Set containment test
   /// @param V a vector having the same size than this
   /// @return true if this strictly contains V
   bool strictlyContains(const IntervalVector& V) const;

   /// Set containment test
   /// @return true if this contains 0.0
   bool containsZero() const;

   /// Set containment test
   /// @return true if this strictly contains 0.0
   bool strictlyContainsZero() const;

   /// Test of disjoint intervals
   /// @param V a vector having the same size than this
   /// @return true if this and V are disjoint
   bool isDisjoint(const IntervalVector& V) const;

   /// Test of overlapping intervals
   /// @param V a vector having the same size than this
   /// @return true if this and V overlap
   bool overlaps(const IntervalVector& V) const;

   /// @return the one-norm of this
   double oneNorm() const;

   /// @return the infinite-norm of this
   double infNorm() const;

   /// Intersection with assignment
   /// @param V an interval vector
   /// @return a reference to this
   ///
   /// this is assigned to the intersection of this and V
   IntervalVector& operator&=(const IntervalVector& V);

   /// Intersects two interval vectors
   /// @param V an interval vector
   /// @param W an interval vector
   /// @return a new interval vector equal to the intersection of 'V' and 'W'
   friend IntervalVector operator&(const IntervalVector& V,
                                   const IntervalVector& W);

   /// Hull with assignment
   /// @param V an interval vector
   /// @return a reference to this
   ///
   /// this is assigned to the hull of this and V
   IntervalVector& operator|=(const IntervalVector& V);

   /// Calculates the hull of two interval vectors
   /// @param V an interval vector
   /// @param W an interval vector
   /// @return a new interval vector equal to the hull of 'V' and 'W'
   friend IntervalVector operator|(const IntervalVector& V,
                                   const IntervalVector& W);
};

} // namespace

#endif
