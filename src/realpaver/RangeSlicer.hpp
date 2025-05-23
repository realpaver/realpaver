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
 * @file   RangeSlicer.hpp
 * @brief  Classes of range slicers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_RANGE_SLICER_HPP
#define REALPAVER_RANGE_SLICER_HPP

#include "realpaver/Range.hpp"
#include <list>

namespace realpaver {

/**
 * @brief Base class of range slicers.
 *
 * A slicer generates a partition of a range. The slices stored in a container
 * can be obtained through iterators.
 */
class RangeSlicer {
public:
   /// Default constructor
   RangeSlicer() = default;

   /// Default copy constructor
   RangeSlicer(const RangeSlicer &) = default;

   /// Default assignment operator
   RangeSlicer &operator=(const RangeSlicer &) = default;

   /// Destructor
   virtual ~RangeSlicer();

   /**
    * @brief Slices a range x and returns the number of slices.
    *
    * This method calls applyImpl(x).
    */
   size_t apply(const Range &x);

   /// Returns the number of slices generated by apply(x)
   size_t nbSlices() const;

protected:
   /// Implements the slicing method
   virtual void applyImpl(const Range &x) = 0;

   /// Inserts an interval in the container
   void push(const Range &x);

   /// Clears the container
   void clear();

private:
   using ContainerType = std::list<Range>;
   ContainerType cont_;

public:
   /// Iterators on the set of slices
   typedef typename ContainerType::iterator iterator;

   /// Returns an iterator on the beginning of the container of slices
   iterator begin();

   /// Returns an iterator on the end of the container of slices
   iterator end();
};

/*----------------------------------------------------------------------------*/

/// Bisection: [a,b] -> [a,m], [m+1,b] (midpoint m)
class RangeBisecter : public RangeSlicer {
public:
   void applyImpl(const Range &x) override;
};

/*----------------------------------------------------------------------------*/

/// Trisection: [a,b] -> [a,a], [a+1,b-1], [b,b]
class RangePeeler : public RangeSlicer {
public:
   void applyImpl(const Range &x) override;
};

/*----------------------------------------------------------------------------*/

/// Peeling: [a,b] -> [a,a], [a+1,b]
class RangeLeftFixer : public RangeSlicer {
public:
   void applyImpl(const Range &x) override;
};

/*----------------------------------------------------------------------------*/

/// Peeling: [a,b] -> [a,b-1], [b,b]
class RangeRightFixer : public RangeSlicer {
public:
   void applyImpl(const Range &x) override;
};

/*----------------------------------------------------------------------------*/

/// Partition: [a,b] -> [a,a], [a+1,a+1], ..., [b,b]
class RangeSprayer : public RangeSlicer {
public:
   void applyImpl(const Range &x) override;
};

} // namespace realpaver

#endif
