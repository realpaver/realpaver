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
 * @file   Inlator.hpp
 * @brief  Inflator of intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INFLATOR_HPP
#define REALPAVER_INFLATOR_HPP

#include "realpaver/IntervalVector.hpp"

namespace realpaver {

/**
 * @brief Inflator of intervals.
 *
 *
 * Given an interval x and two real numbers delta > 1.0 and chi > 0.0,
 * an inflator calculates the interval m(x) + delta*(x - m(x)) + chi*[-1,1]
 * where m(x) is the midpoint of x.
 */
class Inflator {
public:
   /// Default constructor with typical values of delta and chi
   Inflator();

   /// Constructor
   Inflator(double delta, double chi);

   /// Default copy constructor
   Inflator(const Inflator &) = default;

   /// Default asignment operator
   Inflator &operator=(const Inflator &) = default;

   /// Default destructor
   ~Inflator() = default;

   /// Returns the value of delta
   double getDelta() const;

   /// Sets the value of delta
   void setDelta(double delta);

   /// Returns the value of chi
   double getChi() const;

   /// Sets the value of chi
   void setChi(double chi);

   /// Inflates an interval
   Interval inflate(const Interval &x) const;

   /// Inflates each component of an interval vector
   void inflate(IntervalVector &X) const;

private:
   double delta_;
   double chi_;
};

} // namespace realpaver

#endif
