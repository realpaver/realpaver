// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_INTERVAL_UNION_HPP
#define REALPAVER_INTERVAL_UNION_HPP

#include <iostream>
#include <vector>
#include "realpaver_interval.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of interval unions, i.e. ordered sets of disjoint intervals.
 *****************************************************************************/
class IntervalUnion {
public:
   // constructors
   IntervalUnion();
   IntervalUnion(const Interval& x);
   IntervalUnion(const std::initializer_list<Interval>& l);
   IntervalUnion(const IntervalUnion&) = default;

   // returns the number of disjoint intervals
   size_t size() const;

   // access to the i-th interval
   const Interval& operator[](size_t i) const;

   // returns true if this union is empty
   bool isEmpty() const;

   // assigns this to empty
   void setEmpty();

   // inserts x in this
   IntervalUnion& insert(const Interval& x);

   // returns the interval hull of this
   Interval hull() const;

   // contracts x as hull(x inter this)
   void contract(Interval& x) const;

   // output on a stream
   void print(std::ostream& os) const;

private:
   typedef std::vector<Interval> VectorType;
   VectorType v_;

   // first := index of the leftmost interval intersecting x
   // right := index of the rightmost interval intersecting x
   // returns true if this and x overlap
   bool findInter(const Interval& x, int& first, int& last) const;

public:
   // iterators
   typedef VectorType::iterator iterator;
   iterator begin();
   iterator end();
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const IntervalUnion& u);

inline size_t IntervalUnion::size() const
{
   return v_.size();
}

inline const Interval& IntervalUnion::operator[](size_t i) const
{
   return v_[i];
}

inline bool IntervalUnion::isEmpty() const
{
   return v_.empty();
}

inline void IntervalUnion::setEmpty()
{
   v_.clear();
}

inline IntervalUnion::iterator IntervalUnion::begin()
{
   return v_.begin();
}

inline IntervalUnion::iterator IntervalUnion::end()
{
   return v_.end();
}

} // namespace toutatis

#endif
