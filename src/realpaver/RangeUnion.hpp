///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_RANGE_UNION_HPP
#define REALPAVER_RANGE_UNION_HPP

#include <iostream>
#include <vector>
#include "realpaver/Range.hpp"

namespace realpaver {

class Interval;

///////////////////////////////////////////////////////////////////////////////
/// This is an ordered set of disjoint ranges.
///////////////////////////////////////////////////////////////////////////////
class RangeUnion {
public:
   /// Creates an empty range union
   RangeUnion();

   /// Creates a range union reduced to one range
   RangeUnion(const Range& r);

   /// Creates a range union
   /// @para l list of ranges inserted in this
   RangeUnion(const std::initializer_list<Range>& l);

   /// Creates a range union
   /// @para l list of integers inserted in this
   RangeUnion(const std::initializer_list<int>& l);

   /// Default copy constructor
   RangeUnion(const RangeUnion&) = default;

   /// Default assignment operator
   RangeUnion& operator=(const RangeUnion&) = default;

   /// Default destructor
   ~RangeUnion() = default;

   /// @return the number of disjoint ranges in this
   size_t size() const;

   /// Gets a range of this
   /// @param i index of a range between 0 and size()-1
   /// @return the i-th range of this
   Range operator[](size_t i) const;

   /// @return true if this is empty
   bool isEmpty() const;

   /// Assigns this to the empty set
   void setEmpty();

   /// Gets a part of this
   /// @param i index between 0 and size()-1
   /// @param j index between 0 and size()-1 such that j >= i
   RangeUnion subUnion(size_t i, size_t j) const;

   /// Inserts an element in this
   /// @param r range inserted in this
   /// @return a reference to this
   RangeUnion& insert(const Range& r);

   /// @return the hull of this
   Range hull() const;

   /// Contracts an interval
   /// @param x an interval
   ///
   /// x is assigned tohull(x inter this)
   void contractInterval(Interval& x) const;

   /// Contracts this
   /// @param x an interval
   ///
   /// this is assigned to (this inter x)
   void contract(const Interval& x);

   /// Output on a stream
   /// @param os output stream
   void print(std::ostream& os) const;

   /// Clears this, which becomes empty
   void clear();

private:
   typedef std::vector<Range> VectorType;
   VectorType v_;

   // first := index of the leftmost range that overlaps with r
   // right := index of the rightmost range that ioverlaps with r
   // returns true if this and r overlap
   bool findInter(const Range& r, int& first, int& last) const;

   // first := index of the leftmost range that is joinable with r
   // right := index of the rightmost range that is joinable with r
   // returns true if this and r overlap
   bool findJoin(const Range& r, int& first, int& last) const;

public:
   /// Type of iterators in an interval union
   typedef VectorType::iterator iterator;

   /// @return an iterator to the beginning of this
   iterator begin();

   /// @return an iterator to the end of this
   iterator end();
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const RangeUnion& u);

} // namespace toutatis

#endif
