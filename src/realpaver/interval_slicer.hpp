// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_INTERVAL_SLICER_HPP
#define REALPAVER_INTERVAL_SLICER_HPP

#include <list>
#include "realpaver/interval.hpp"

namespace realpaver {

/*****************************************************************************
 * Base class of interval slicers.
 * 
 * A slicer generates a partition of an interval. The slices can be obtained
 * through iterators.
 *****************************************************************************/
class IntervalSlicer {
public:
   // destructor
   virtual ~IntervalSlicer();

   // slicing method that returns the number of slices
   size_t apply(const Interval& x);

   // number of slices
   size_t size() const;

protected:
   // implementation of the slicing method
   virtual void applyImpl(const Interval& x) = 0;
   void push(const Interval& x);
   void clear();

private:
   typedef std::list<Interval> ContainerType;
   ContainerType cont_;

public:
   // iterators
   typedef typename ContainerType::iterator iterator;
   iterator begin();
   iterator end();
};

inline void IntervalSlicer::push(const Interval& x)
{
   if (!x.isEmpty())
      cont_.push_back(x);
}

inline void IntervalSlicer::clear()
{
   cont_.clear();
}

inline size_t IntervalSlicer::size() const
{
   return cont_.size();
}

inline IntervalSlicer::iterator IntervalSlicer::begin()
{
   return cont_.begin();
}

inline IntervalSlicer::iterator IntervalSlicer::end()
{
   return cont_.end();
}

/*****************************************************************************
 * Class of interval slicers used to bisect an interval, i.e. to split it
 * in two equal parts.
 *****************************************************************************/
class IntervalBisecter : public IntervalSlicer {
public:
   // Override
   void applyImpl(const Interval& x);
};

/*****************************************************************************
 * Class of interval slicers used to split an interval in three parts.
 * 
 * Given [a,b] it generates [a,a+w], [a+w,b-w], [b-w,b] where w is a factor
 * of the width of [a,b]
 *****************************************************************************/
class IntervalPeeler : public IntervalSlicer {
public:
   // constructor given a factor f in ]0,100[ considered as a percentage
   // of the width of an input interval
   IntervalPeeler(const double& f);

   // getter / setter of the interval width factor
   double factor() const;
   void setFactor(const double& f);

   // peeling methods
   Interval peelLeft(const Interval& x) const;
   Interval peelRight(const Interval& x) const;

   // Override
   void applyImpl(const Interval& x);

private:
   double f_;     // factor
   Interval p_;   // f_ / 100
};

inline double IntervalPeeler::factor() const
{
   return f_;
}

/*****************************************************************************
 * Class of interval slicers used to split an interval in several parts.
 * 
 * Given [a,b] and a number of slices n, it generates the sequence
 * [a,a+h], [a+h,a+2h], ..., [b-h,b] where h is obtained as the width
 * of [a,b] divided by n
 *****************************************************************************/
class IntervalPartitioner : public IntervalSlicer {
public:
   // constructor given the size of the partition
   IntervalPartitioner(size_t n = 2);

   // getter / setter of the size of the partition
   size_t arity() const;
   void setArity(size_t n);

   // Override
   void applyImpl(const Interval& x);

private:
   size_t n_;  // partition size (number of slices)
};

inline size_t IntervalPartitioner::arity() const
{
   return n_;
}

} // namespace

#endif
