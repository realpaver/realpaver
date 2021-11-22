// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_interval_union.hpp"

namespace realpaver {

IntervalUnion::IntervalUnion() : v_()
{}

IntervalUnion::IntervalUnion(const Interval& x) : v_()
{
   insert(x);
}

IntervalUnion::IntervalUnion(const std::initializer_list<Interval>& l) : v_()
{
   for (Interval x : l)
      insert(x);
}

IntervalUnion& IntervalUnion::insert(const Interval& x)
{
   if (x.isEmpty()) return *this;

   if (isEmpty())
   {
      v_.push_back(x);
      return *this;
   }

   // insertion at the beginning?            v_[0]:         |------|
   //                                            x: |---|
   if (x.right()<v_[0].left())
   {
      v_.insert(v_.begin(), x);
      return *this;
   }

   // insertion at the end?             v_[size-1]: |------|
   //                                            x:            |---|
   if (x.left()>v_[size()-1].right())
   {
      v_.push_back(x);
      return *this;
   }

   // dichotomic search of the range of intervals intersecting x
   int first, last;
   bool found = findInter(x,first,last);

   if (found)
   {
      // only one interval having an intersection
      if (first==last)
      {
         v_[first] |= x;
      }
      else
      {
         Interval y(v_[first] | v_[last] | x);
            
         // remove the range first..last from the vector
         auto itfirst = v_.begin();
         std::advance(itfirst,first);
            
         auto itlast = v_.begin();
         std::advance(itlast,last + 1);

         v_.erase(itfirst,itlast);
            
         // insert the hull at the right place
         auto it = v_.begin();
         std::advance(it,first);
         v_.insert(it,y);
      }
   }
   else
   {
      // no interval intersecting x
      // insert x before first (we have first == last+1)
      auto it = v_.begin();
      std::advance(it, first);
      v_.insert(it, x);
   }
   return *this;
}

Interval IntervalUnion::hull() const
{
   if (isEmpty())
      return Interval::emptyset();

   else if (size()==1)
      return v_[0];

   else
      return Interval(v_[0].left(),v_[size()-1].right());
}

void IntervalUnion::contract(Interval& x) const
{
   if (!x.isEmpty())
   {
      if (isEmpty())
         x.setEmpty();

      else
      {
         int first, last;
         bool found = findInter(x,first,last);
         if (found)
            x &= (v_[first] | v_[last]);

         else
            x.setEmpty();
      }
   }
}

void IntervalUnion::print(std::ostream& os) const
{
   if (isEmpty())
      os << "{empty}";

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

bool IntervalUnion::findInter(const Interval& x, int& first, int& last) const
{
   first = 0;
   last = size()-1;
   int current;
   bool found = false;

   // dichotomic search of an interval intersecting x
   while ( (!found) && last>=first )
   {
      // checks the midpoint interval between first and last
      current = (first+last)/ 2;

      // first case:           v_[current]: |------|
      //                                 x:              |---|   
      if (v_[current].right()<x.left())
         first = current + 1;

      // second case:          v_[current]:            |------|
      //                                 x:   |---|   
      else if (v_[current].left()>x.right())
         last = current - 1;

      // last case:            v_[current]:    |------|
      //                               x:    |---|  
      else
         found = true;
   }

   if (found)
   {
      // find the leftmost interval intersecting x
      first = current - 1;
      while (first>=0 && (v_[first].overlaps(x)))
         --first;
      ++first;

      // find the rightmost interval intersecting x
      last = current + 1;
      while ( (last<(int)size()) && (v_[last].overlaps(x)))
         ++last;
      --last;

      return true;
   }
   else
      return false;
}

std::ostream& operator<<(std::ostream& os, const IntervalUnion& u)
{
   u.print(os);
   return os;
}

} // namespace
