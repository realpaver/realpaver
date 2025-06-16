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
 * @file   IntervalUnion.cpp
 * @brief  Ordered set of disjoint intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/IntervalUnion.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

IntervalUnion::IntervalUnion()
    : v_()
{
}

IntervalUnion::IntervalUnion(const Interval &x)
    : v_()
{
   insert(x);
}

IntervalUnion::IntervalUnion(const std::initializer_list<Interval> &l)
    : v_()
{
   for (Interval x : l)
      insert(x);
}

size_t IntervalUnion::size() const
{
   return v_.size();
}

const Interval &IntervalUnion::operator[](size_t i) const
{
   ASSERT(i >= 0 && i < v_.size(), "Bad access in an interval union @ " << i);

   return v_[i];
}

bool IntervalUnion::isEmpty() const
{
   return v_.empty();
}

void IntervalUnion::setEmpty()
{
   v_.clear();
}

IntervalUnion::iterator IntervalUnion::begin()
{
   return v_.begin();
}

IntervalUnion::iterator IntervalUnion::end()
{
   return v_.end();
}

IntervalUnion IntervalUnion::subUnion(size_t i, size_t j) const
{
   ASSERT(i >= 0 && i < v_.size(), "Bad access in an interval union @ " << i);
   ASSERT(j >= 0 && j < v_.size(), "Bad access in an interval union @ " << j);
   ASSERT(i <= j, "Bad indexes used to create a sub interval union");

   IntervalUnion u;
   for (size_t k = i; k <= j; ++k)
      u.v_.push_back(v_[k]);
   return u;
}

IntervalUnion &IntervalUnion::insert(const Interval &x)
{
   if (x.isEmpty())
      return *this;

   if (isEmpty())
   {
      v_.push_back(x);
      return *this;
   }

   // insertion at the beginning?            v_[0]:         |------|
   //                                            x: |---|
   if (x.right() < v_[0].left())
   {
      v_.insert(v_.begin(), x);
      return *this;
   }

   // insertion at the end?             v_[size-1]: |------|
   //                                            x:            |---|
   if (x.left() > v_[size() - 1].right())
   {
      v_.push_back(x);
      return *this;
   }

   // dichotomic search of the range of intervals intersecting x
   int first, last;
   bool found = findInter(x, first, last);

   if (found)
   {
      // only one interval having an intersection
      if (first == last)
      {
         v_[first] |= x;
      }
      else
      {
         Interval y(v_[first] | v_[last] | x);

         // removes the range first..last from the vector
         auto itfirst = v_.begin();
         std::advance(itfirst, first);

         auto itlast = v_.begin();
         std::advance(itlast, last + 1);

         v_.erase(itfirst, itlast);

         // inserts the hull at the right place
         auto it = v_.begin();
         std::advance(it, first);
         v_.insert(it, y);
      }
   }
   else
   {
      // no interval intersecting x
      // inserts x before first (we have first == last+1)
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

   else if (size() == 1)
      return v_[0];

   else
      return Interval(v_[0].left(), v_[size() - 1].right());
}

void IntervalUnion::contractInterval(Interval &x) const
{
   if (!x.isEmpty())
   {
      if (isEmpty())
         x.setEmpty();

      else
      {
         int first, last;
         bool found = findInter(x, first, last);
         if (found)
            x &= (v_[first] | v_[last]);

         else
            x.setEmpty();
      }
   }
}

void IntervalUnion::contract(const Interval &x)
{
   if (x.isEmpty())
   {
      clear();
      return;
   }

   int first, last;
   bool b = findInter(x, first, last);

   if (!b)
   {
      clear();
      return;
   }

   // intersects the outermost intervals (and not the other ones)
   v_[first] &= x;
   v_[last] &= x;

   // removes the intervals after last
   if (last < (int)(v_.size() - 1))
   {
      auto it = v_.begin();
      std::advance(it, last + 1);
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

void IntervalUnion::print(std::ostream &os) const
{
   if (isEmpty())
      os << "{empty}";

   else
   {
      os << "{";
      for (auto it = v_.begin(); it != v_.end(); ++it)
      {
         if (it != v_.begin())
            os << ", ";
         os << (*it);
      }
      os << "}";
   }
}

bool IntervalUnion::findInter(const Interval &x, int &first, int &last) const
{
   first = 0;
   last = size() - 1;
   int current;
   bool found = false;

   // dichotomic search of an interval intersecting x
   while ((!found) && last >= first)
   {
      // checks the midpoint interval between first and last
      current = (first + last) / 2;

      // first case:           v_[current]: |------|
      //                                 x:              |---|
      if (v_[current].right() < x.left())
         first = current + 1;

      // second case:          v_[current]:            |------|
      //                                 x:   |---|
      else if (v_[current].left() > x.right())
         last = current - 1;

      // last case:            v_[current]:    |------|
      //                               x:    |---|
      else
         found = true;
   }

   if (found)
   {
      // finds the leftmost interval intersecting x
      first = current - 1;
      while (first >= 0 && (v_[first].overlaps(x)))
         --first;
      ++first;

      // finds the rightmost interval intersecting x
      last = current + 1;
      while ((last < (int)size()) && (v_[last].overlaps(x)))
         ++last;
      --last;

      return true;
   }
   else
      return false;
}

void IntervalUnion::clear()
{
   v_.clear();
}

double IntervalUnion::width() const
{
   double s = 0.0;
   for (size_t i = 0; i < size(); ++i)
      s += v_[i].width();
   return s;
}

bool IntervalUnion::equals(const IntervalUnion &other) const
{
   if (isEmpty() || other.isEmpty())
      return false;
   if (size() != other.size())
      return false;

   for (size_t i = 0; i < size(); ++i)
      if (v_[i].isSetNeq(other.v_[i]))
         return false;

   return true;
}

std::ostream &operator<<(std::ostream &os, const IntervalUnion &u)
{
   u.print(os);
   return os;
}

IntervalUnion addPX(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
      U.insert(addPX(x, y, Z.v_[i]));
   return U;
}

IntervalUnion addPY(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
      U.insert(addPY(x, y, Z.v_[i]));
   return U;
}

IntervalUnion subPX(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
      U.insert(subPX(x, y, Z.v_[i]));
   return U;
}

IntervalUnion subPY(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
      U.insert(subPY(x, y, Z.v_[i]));
   return U;
}

IntervalUnion mulPX(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
   {
      Interval z = Z.v_[i];
      if (y.strictlyContainsZero())
      {
         U.insert(mulPX(x, Interval(y.left(), 0.0), z));
         U.insert(mulPX(x, Interval(0.0, y.right()), z));
      }
      else
      {
         U.insert(mulPX(x, y, z));
      }
   }
   return U;
}

IntervalUnion mulPY(const Interval &x, const Interval &y, const IntervalUnion &Z)
{
   IntervalUnion U;
   for (size_t i = 0; i < Z.size(); ++i)
   {
      Interval z = Z.v_[i];
      if (x.strictlyContainsZero())
      {
         U.insert(mulPY(Interval(x.left(), 0.0), y, z));
         U.insert(mulPY(Interval(0.0, x.right()), y, z));
      }
      else
      {
         U.insert(mulPY(x, y, z));
      }
   }
   return U;
}

IntervalUnion sqrPX(const Interval &x, const IntervalUnion &Y)
{
   IntervalUnion U;
   for (size_t i = 0; i < Y.size(); ++i)
   {
      Interval y = Y.v_[i];
      if (y.left() > 0.0)
      {
         Interval z = sqrt(y);
         U.insert(x & z);
         U.insert(x & (-z));
      }
      else if (y.right() >= 0.0)
      {
         Interval z = sqrt(Interval(y.right()));
         U.insert(x & Interval(-z.right(), z.right()));
      }
   }
   return U;
}

} // namespace realpaver
