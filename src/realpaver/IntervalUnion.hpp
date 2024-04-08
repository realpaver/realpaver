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
 * @file   IntervalUnion.hpp
 * @brief  Ordered set of disjoint intervals
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_INTERVAL_UNION_HPP
#define REALPAVER_INTERVAL_UNION_HPP

#include <iostream>
#include <vector>
#include "realpaver/Interval.hpp"

namespace realpaver {

/// Ordered set of disjoint intervals
class IntervalUnion {
public:
   /// Creates an empty interval union
   IntervalUnion();

   /// Creates an interval union reduced to one interval
   IntervalUnion(const Interval& x);

   /// Creates an interval union from a list
   IntervalUnion(const std::initializer_list<Interval>& l);

   /// Default copy constructor
   IntervalUnion(const IntervalUnion&) = default;

   /// Default assignment operator
   IntervalUnion& operator=(const IntervalUnion&) = default;

   /// Default destructor
   ~IntervalUnion() = default;

   /// Returns the number of disjoint intervals in this
   size_t size() const;

   /// Gets the i-th interval of this
   const Interval& operator[](size_t i) const;

   /// Gets a part of this
   IntervalUnion subUnion(size_t i, size_t j) const;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Assigns this to the empty set
   void setEmpty();

   /// Inserts x in this
   IntervalUnion& insert(const Interval& x);

   /// Returns the interval hull of this
   Interval hull() const;

   /// Contracts x as hull(x inter this)
   void contractInterval(Interval& x) const;

   /// Contracts this as (this inter x)
   void contract(const Interval& x);

   /// Output on a stream
   void print(std::ostream& os) const;

   /// Clears this, which becomes empty
   void clear();

   /// Returns the sum of the widths of the components of this
   double width() const;

private:
   typedef std::vector<Interval> VectorType;
   VectorType v_;

   // first := index of the leftmost interval intersecting x
   // right := index of the rightmost interval intersecting x
   // returns true if this and x overlap
   bool findInter(const Interval& x, int& first, int& last) const;

public:
   /// Type of iterators in an interval union
   using iterator = VectorType::iterator;

   /// Returns an iterator to the beginning of this
   iterator begin();

   /// Returns an iterator to the end of this
   iterator end();
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const IntervalUnion& u);

} // namespace toutatis

#endif
