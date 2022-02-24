///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_UNION_HPP
#define REALPAVER_INTERVAL_UNION_HPP

#include <iostream>
#include <vector>
#include "realpaver/Interval.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an ordered set of disjoint intervals.
///////////////////////////////////////////////////////////////////////////////
class IntervalUnion {
public:
   /// Creates an empty interval union
   IntervalUnion();

   /// Creates an interval union reduced to one interval
   IntervalUnion(const Interval& x);

   /// Creates an interval union
   /// @para l list of intervals inserted in this
   IntervalUnion(const std::initializer_list<Interval>& l);

   /// Default copy constructor
   IntervalUnion(const IntervalUnion&) = default;

   /// Default assignment operator
   IntervalUnion& operator=(const IntervalUnion&) = default;

   /// Default destructor
   ~IntervalUnion() = default;

   /// @return the number of disjoint intervals in this
   size_t size() const;

   /// Gets an interval of this
   /// @param i index of an interval between 0 and size()-1
   /// @return the i-th interval of this
   Interval operator[](size_t i) const;

   /// @return true if this is empty
   bool isEmpty() const;

   /// Assigns this to the empty set
   void setEmpty();

   /// Inserts an element in this
   /// @param x interval inserted in this
   /// @return a reference to this
   IntervalUnion& insert(const Interval& x);

   /// @return the interval hull of this
   Interval hull() const;

   /// Contracts an interval
   /// @param x an interval
   ///
   /// x is assigned tohull(x inter this)
   void contract(Interval& x) const;

   /// Output on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

private:
   typedef std::vector<Interval> VectorType;
   VectorType v_;

   // first := index of the leftmost interval intersecting x
   // right := index of the rightmost interval intersecting x
   // returns true if this and x overlap
   bool findInter(const Interval& x, int& first, int& last) const;

public:
   /// Type of iterators in an interval union
   typedef VectorType::iterator iterator;

   /// @return an iterator to the beginning of this
   iterator begin();

   /// @return an iterator to the end of this
   iterator end();
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const IntervalUnion& u);

} // namespace toutatis

#endif
