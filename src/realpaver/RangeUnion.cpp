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
 * @file   RangeUnion.cpp
 * @brief  Union of ranges
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/RangeUnion.hpp"

namespace realpaver {

RangeUnion::RangeUnion() : v_()
{}

RangeUnion::RangeUnion(const Range& r) : v_()
{
   insert(r);
}

RangeUnion::RangeUnion(const std::initializer_list<Range>& l) : v_()
{
   for (Range x : l)
      insert(x);
}

RangeUnion::RangeUnion(const std::initializer_list<int>& l) : v_()
{
   for (int k : l)
      insert(Range(k));
}

size_t RangeUnion::size() const
{
   return v_.size();
}

Range RangeUnion::operator[](size_t i) const
{
   ASSERT(i>=0 && i<v_.size(), "Bad access in a range union @ " << i);

   return v_[i];
}

bool RangeUnion::isEmpty() const
{
   return v_.empty();
}

void RangeUnion::setEmpty()
{
   v_.clear();
}

RangeUnion::iterator RangeUnion::begin()
{
   return v_.begin();
}

RangeUnion::iterator RangeUnion::end()
{
   return v_.end();
}

RangeUnion RangeUnion::subUnion(size_t i, size_t j) const
{
   ASSERT(i>=0 && i<v_.size(), "Bad access in a range union @ " << i);
   ASSERT(j>=0 && j<v_.size(), "Bad access in a range union @ " << j);
   ASSERT(i<=j, "Bad indexes used to create a sub range union");

   RangeUnion u;
   for (size_t k=i; k<=j; ++k)
      u.v_.push_back(v_[k]);
   return u;
}

RangeUnion& RangeUnion::insert(const Range& r)
{
   if (r.isEmpty()) return *this;

   if (isEmpty())
   {
      v_.push_back(r);
      return *this;
   }

   // insertion at the beginning?            v_[0]:         |------|
   //                                            r: |---|
   if (r.isCertainlyLt(v_[0]) && (!r.isJoinable(v_[0])))
   {
      v_.insert(v_.begin(), r);
      return *this;
   }

   // insertion at the end?             v_[size-1]: |------|
   //                                            r:            |---|
   if (r.isCertainlyGt(v_[size()-1]) && (!r.isJoinable(v_[size()-1])))
   {
      v_.push_back(r);
      return *this;
   }

   // dichotomic search of the range of intervals joinable with r
   int first, last;
   bool found = findJoin(r, first, last);

   if (found)
   {
      // only one range having a join
      if (first==last)
      {
         v_[first] |= r;
      }
      else
      {
         Range y(v_[first] | v_[last] | r);

         // removes the range first..last from the vector
         auto itfirst = v_.begin();
         std::advance(itfirst, first);
            
         auto itlast = v_.begin();
         std::advance(itlast, last + 1);

         v_.erase(itfirst,itlast);
            
         // inserts the hull at the right place
         auto it = v_.begin();
         std::advance(it, first);
         v_.insert(it, y);
      }
   }
   else
   {
      // no range joinable with r
      // inserts r before first (we have first == last+1)
      auto it = v_.begin();
      std::advance(it, first);
      v_.insert(it, r);
   }
   return *this;
}

Range RangeUnion::hull() const
{
   if (isEmpty())
      return Range::emptyset();

   else if (size()==1)
      return v_[0];

   else
      return Range(v_[0].left(), v_[size()-1].right());
}

void RangeUnion::contractInterval(Interval& x) const
{
   Range r = Range::roundInward(x);

   if (isEmpty() || r.isEmpty())
      x.setEmpty();

   else
   {
      int first, last;
      bool found = findInter(r, first, last);

      if (found)
      {
         Range y = r & (v_[first] | v_[last]);
         x = y.toInterval();
      }

      else
         x.setEmpty();
   }
}


void RangeUnion::contract(const Interval& x)
{
   if (x.isEmpty())
   {
      clear();
      return;
   }

   int first, last;
   Range r = Range::roundInward(x);
   bool b = findInter(r, first, last);

   if (!b)
   {
      clear();
      return;
   }

   // intersects the outermost intervals (and not the other ones)
   v_[first] &= r;
   v_[last] &= r;

   // removes the intervals after last
   if (last < v_.size()-1)
   {
      auto it = v_.begin();
      std::advance(it, last+1);
      v_.erase(it, v_.end());
   }

   // (and then) removes the intervals before first
   if (first > 0)
   {
      auto it = v_.begin();
      std::advance(it, first);
      v_.erase(v_.begin(), it);
   }
}

void RangeUnion::print(std::ostream& os) const
{
   if (isEmpty())
      os << "{empty}";

   else if (size()==1)
      os << v_[0];


   else
   {
      os << "{";
      for (auto it = v_.begin(); it != v_.end(); ++it)
      {
         if (it != v_.begin()) os << ", ";
         os << (*it);
      }
      os << "}";
   }
}

bool RangeUnion::findInter(const Range& r, int& first, int& last) const
{
   first = 0;
   last = size()-1;
   int current;
   bool found = false;

   // dichotomic search of an interval intersecting r
   while ( (!found) && last>=first )
   {
      // checks the midpoint interval between first and last
      current = (first+last)/ 2;

      // first case:           v_[current]: |------|
      //                                 r:              |---|   
      if (v_[current].right()<r.left())
         first = current + 1;

      // second case:          v_[current]:            |------|
      //                                 r:   |---|   
      else if (v_[current].left()>r.right())
         last = current - 1;

      // last case:            v_[current]:    |------|
      //                               r:    |---|  
      else
         found = true;
   }

   if (found)
   {
      // finds the leftmost interval intersecting r
      first = current - 1;
      while (first>=0 && (v_[first].overlaps(r)))
         --first;
      ++first;

      // finds the rightmost interval intersecting r
      last = current + 1;
      while ( (last<(int)size()) && (v_[last].overlaps(r)))
         ++last;
      --last;

      return true;
   }
   else
      return false;
}

bool RangeUnion::findJoin(const Range& r, int& first, int& last) const
{
   first = 0;
   last = size()-1;
   int current;
   bool found = false;

   // dichotomic search of a range that is joinable with r
   while ( (!found) && last>=first )
   {
      // checks the midpoint interval between first and last
      current = (first+last)/ 2;

      // first case:            v_[current]:    |------|
      //                               r:    |---|  
      if (v_[current].isJoinable(r))
         found = true;

      // second case:           v_[current]: |------|
      //                                 r:              |---|   
      else if (v_[current].isCertainlyLt(r))
         first = current + 1;

      // third case:          v_[current]:            |------|
      //                                 r:   |---|   
      else
         last = current - 1;
   }

   if (found)
   {
      // finds the leftmost range that is joinable with r
      first = current - 1;
      while (first>=0 && (v_[first].isJoinable(r)))
         --first;
      ++first;

      // finds the rightmost range intersecting r
      last = current + 1;
      while ( (last<(int)size()) && (v_[last].isJoinable(r)))
         ++last;
      --last;

      return true;
   }
   else
      return false;
}

void RangeUnion::clear()
{
   v_.clear();
}

unsigned long RangeUnion::nbElems() const
{
   unsigned long n = 0;
   for (size_t i=0; i<size(); ++i)
      n += v_[i].nbElems();
   return n;
}

std::ostream& operator<<(std::ostream& os, const RangeUnion& u)
{
   u.print(os);
   return os;
}

} // namespace
