///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_SLICER_HPP
#define REALPAVER_INTERVAL_SLICER_HPP

#include <list>
#include "realpaver/interval.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class of interval slicers.
///
/// A slicer generates a partition of an interval. The slices can be obtained
/// through iterators.
///////////////////////////////////////////////////////////////////////////////
class IntervalSlicer {
public:
   /// Default constructor
   IntervalSlicer() = default;

   /// Destructor
   virtual ~IntervalSlicer();

   /// Default copy constructor
   IntervalSlicer(const IntervalSlicer&) = default;

   /// Default assignment operator
   IntervalSlicer& operator=(const IntervalSlicer&) = default;

   /// Slices an interval
   /// @param x an interval
   /// @return the number of slices
   ///
   /// This method calls applyImpl(x).
   size_t apply(const Interval& x);

   /// @return the number of slices generated by apply(x)
   size_t nbSlices() const;

protected:
   /// Implements the slicing method
   virtual void applyImpl(const Interval& x) = 0;

   /// Inserts an interval in the container
   void push(const Interval& x);

   /// Clears the container
   void clear();

private:
   typedef std::list<Interval> ContainerType;
   ContainerType cont_;

public:
   /// Iterators on the set of slices
   typedef typename ContainerType::iterator iterator;

   /// @returns an iterator on the beginning of the container of slices
   iterator begin();

   /// @returns an iterator on the end of the container of slices
   iterator end();
};

///////////////////////////////////////////////////////////////////////////////
/// This bisects an interval.
///////////////////////////////////////////////////////////////////////////////
class IntervalBisecter : public IntervalSlicer {
public:
   /// Overrides
   void applyImpl(const Interval& x);
};

///////////////////////////////////////////////////////////////////////////////
/// This divides an interval in three parts.
///
/// Given an interval [a,b] and a factor 0 < f < 100, it calculates the
/// w = (f / 100) * (b-a), which is a proportion of the width of [a,b].
/// Then it generates three intervals [a,a+w], [a+w,b-w], [b-w,b]. The
/// slice [a,a+w] is the left peel and [b-w,b] is the right peel.
///////////////////////////////////////////////////////////////////////////////
class IntervalPeeler : public IntervalSlicer {
public:
   /// Creates a slicer
   /// @param f width factor
   IntervalPeeler(const double& f);

   /// @return the width factor
   double getFactor() const;

   /// Sets the width factor
   /// @param f width factor
   void setFactor(const double& f);

   /// Peeling method
   /// @param interval x
   /// @return the left peel of x
   Interval peelLeft(const Interval& x) const;

   /// Peeling method
   /// @param interval x
   /// @return the right peel of x
   Interval peelRight(const Interval& x) const;

   /// Overrides
   void applyImpl(const Interval& x);

private:
   double f_;     // factor
   Interval p_;   // f_ / 100
};

///////////////////////////////////////////////////////////////////////////////
/// This divides an interval in several parts of equal size.
///
/// Given an interval [a,b] and a number of slices n, it generates the sequence
/// [a,a+h], [a+h,a+2h], ..., [b-h,b] where h is obtained as the width of [a,b]
/// divided by n. The number n is called arity.
///////////////////////////////////////////////////////////////////////////////
class IntervalPartitioner : public IntervalSlicer {
public:
   /// Creates a slicer
   /// @param n number of slices
   IntervalPartitioner(size_t n = 2);

   /// @return the arity of this
   size_t getArity() const;

   /// Sets the arity of this
   /// @param n arity of this with n >= 2
   void setArity(size_t n);

   /// Overrides
   void applyImpl(const Interval& x);

private:
   size_t n_;
};

} // namespace

#endif
