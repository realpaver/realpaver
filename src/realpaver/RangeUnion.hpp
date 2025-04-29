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
 * @file   RangeUnion.hpp
 * @brief  Union of ranges
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_RANGE_UNION_HPP
#define REALPAVER_RANGE_UNION_HPP

#include "realpaver/Range.hpp"
#include <iostream>
#include <vector>

namespace realpaver {

class Interval;

/// Ordered set of disjoint ranges
class RangeUnion {
public:
   /// Creates an empty range union
   RangeUnion();

   /// Creates a range union reduced to one range
   RangeUnion(const Range &r);

   /// Creates a range union from a list
   RangeUnion(const std::initializer_list<Range> &l);

   /// Creates a range union from a list of int
   RangeUnion(const std::initializer_list<int> &l);

   /// Default copy constructor
   RangeUnion(const RangeUnion &) = default;

   /// Default assignment operator
   RangeUnion &operator=(const RangeUnion &) = default;

   /// Default destructor
   ~RangeUnion() = default;

   /// Returns the number of disjoint ranges in this
   size_t size() const;

   /// Gets the i-th range of this
   Range operator[](size_t i) const;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Assigns this to the empty set
   void setEmpty();

   /// Gets a part of this between the indexes i and j
   RangeUnion subUnion(size_t i, size_t j) const;

   /// Inserts r in this and returns a reference to this
   RangeUnion &insert(const Range &r);

   /// Returns the hull of this
   Range hull() const;

   /// Contracts x as hull(x inter this)
   void contractInterval(Interval &x) const;

   /// Contracts this as (this inter x)
   void contract(const Interval &x);

   /// Output on a stream
   void print(std::ostream &os) const;

   /// Clears this, which becomes empty
   void clear();

   /// Returns the number of elements in this
   unsigned long nbElems() const;

   /// Equality test
   bool equals(const RangeUnion &other) const;

private:
   typedef std::vector<Range> VectorType;
   VectorType v_;

   // first := index of the leftmost range that overlaps with r
   // right := index of the rightmost range that ioverlaps with r
   // returns true if this and r overlap
   bool findInter(const Range &r, int &first, int &last) const;

   // first := index of the leftmost range that is joinable with r
   // right := index of the rightmost range that is joinable with r
   // returns true if this and r overlap
   bool findJoin(const Range &r, int &first, int &last) const;

public:
   /// Type of iterators in an interval union
   using iterator = VectorType::iterator;

   /// Returns an iterator to the beginning of this
   iterator begin();

   /// Returns an iterator to the end of this
   iterator end();
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const RangeUnion &u);

} // namespace realpaver

#endif
