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

#include <memory>
#include "realpaver/Bitset.hpp"
#include "realpaver/interval.hpp"
#include "realpaver/NumericVector.hpp"
#include "realpaver/RealVector.hpp"
#include "realpaver/scope.hpp"

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

   /// No assignment
   IntervalVector& operator=(const IntervalVector&) = delete;

   /// Default destructor
   ~IntervalVector() = default;

   /// @param i an index between 0 and size()-1
   /// @return the i-th element of this
   Interval operator[](size_t i) const;

   /// @param v a variable
   /// @return the i-th element of this where i is the identifier of v
   Interval operator[](Variable v) const;

   /// Sets an element of this
   /// @param i an index between 0 and size()-1
   /// @param x an interval
   void set(size_t i, const Interval& x);

   /// Sets an element of this
   /// @param v a variable
   /// @param x an interval
   void set(Variable v, const Interval& x);

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
   /// @return true if this contains X
   bool contains(const IntervalVector& X) const;

   /// Set containment test
   /// @param X a vector having the same size than this
   /// @return true if this strictly contains X
   bool strictlyContains(const IntervalVector& X) const;

   /// Set containment test
   /// @return true if this contains 0.0
   bool containsZero() const;

   /// Set containment test
   /// @return true if this strictly contains 0.0
   bool strictlyContainsZero() const;

   /// Tests if two intervals are disjoint
   /// @param X a vector having the same size than this
   /// @return true if this and X are disjoint
   bool isDisjoint(const IntervalVector& X) const;

   /// Tests if two intervals overlap
   /// @param X a vector having the same size than this
   /// @return true if this and X overlap
   bool overlaps(const IntervalVector& X) const;

   /// @return the one-norm of this
   double oneNorm() const;

   /// @return the infinite-norm of this
   double infNorm() const;

   /// Intersection with assignment
   /// @param X an interval vector
   ///
   /// this is assigned to the intersection of this and X
   void interAssign(const IntervalVector& X);

   /// Hull with assignment
   /// @param X an interval vector
   ///
   /// this is assigned to the hull of this and X
   void hullAssign(const IntervalVector& X);

   /// Hull with assignment on a scope
   /// @param X an interval vector
   /// @param s a scope
   ///
   /// this[s] is assigned to the hull of this[s] and X[s]
   void hullAssignOnScope(const IntervalVector& X, const Scope& s);

   /// Assignment on a scope
   /// @param X an interval vector
   /// @param s a scope
   ///
   /// this[s] is assigned to X[s]
   void setOnScope(const IntervalVector& X, const Scope& s);

   /// @return a clone of this
   IntervalVector* clone() const;
};

/// This is a shared interval vector.
typedef std::shared_ptr<IntervalVector> SharedIntervalVector;

} // namespace

#endif
