// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_BOX_HPP
#define REALPAVER_BOX_HPP

#include <memory>
#include <vector>
#include "realpaver_interval.hpp"
#include "realpaver_point.hpp"
#include "realpaver_scope.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of interval boxes (vectors).
 *****************************************************************************/
class Box {
public:
   // constructors
   Box();
   Box(size_t n, const Interval& x = Interval::universe());
   Box(const std::initializer_list<Interval>& l);

   // copy
   Box(const Box&) = default;
   Box& operator=(const Box&) = delete;

   // size of this vector
   size_t size() const;

   // constant access to the i-th interval
   const Interval& operator[](size_t i) const;
   const Interval& operator[](const Variable& v) const;

   // constant and safe access to the i-th interval
   const Interval& at(size_t i) const;
   const Interval& at(const Variable& v) const;

   // assignment of the i-th interval to x
   void set(size_t i, const Interval& x);
   void set(const Variable& v, const Interval& x);

   // assignment of all the intervals to x
   void setAll(const Interval& x);

   // for each variable v in s: assigns this(v) to other(v)
   void set(const Box& other, const Scope& s);

   // for each variable v in the scope of this: assigns this(v) to other(v)
   void set(const Box& other);

   // for each variable v in s assigns this(v) to hull(this(v), other(v))
   void setHull(const Box& other, const Scope& s);

   // inserts an interval at the end
   void push(const Interval& x);

   // returns a clone
   Box* clone() const;

   // hash code
   size_t hashCode() const;

   // test functions
   bool isEmpty() const;
   bool isFinite() const;
   bool isInf() const;

   // midpoint
   Point midpoint() const;

   // corner of this box made from all the left bounds
   Point lCorner() const;

   // corner of this box made from all the right bounds
   Point rCorner() const;

   // set operations
   bool contains(const Box& other) const;
   bool strictlyContains(const Box& other) const;
   bool containsZero() const;
   bool strictlyContainsZero() const;

   bool isDisjoint(const Box& other) const;
   bool overlaps(const Box& other) const;

   // ||V||_1 = sum_i |V_i| where |.| stands for the magnitude
   double oneNorm() const;

   // ||V||_inf = max_i |V_i| where |.| stands for the magnitude
   double infNorm() const;

   // maximum width componentwise
   double width() const;

   // intersection
   Box& operator&=(const Box& other);
   friend Box operator&(const Box& x, const Box& y);

   // interval hull
   Box& operator|=(const Box& other);
   friend Box operator|(const Box& x, const Box& y);

private:
   std::vector<Interval> v_;
};

typedef std::shared_ptr<Box> SharedBox;

// output on a stream
std::ostream& operator<<(std::ostream& os, const Box& x);

inline size_t Box::size() const
{
   return v_.size();
}

inline void Box::push(const Interval& x)
{
   v_.push_back(x);
}

inline const Interval& Box::operator[](size_t i) const
{
   return v_[i];
}

inline const Interval& Box::operator[](const Variable& v) const
{
   return v_[v.id()];
}

} // namespace

#endif
