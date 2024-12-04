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
 * @file   Tolerance.hpp
 * @brief  Tolerances for numerical computations
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_TOLERANCE_HPP
#define REALPAVER_TOLERANCE_HPP

#include "realpaver/Interval.hpp"
#include "realpaver/IntervalVector.hpp"

namespace realpaver {

/// Couple of a relative tolerance and an absolute tolerance
class Tolerance {
public:
   /**
    * @brief Constructor.
    * 
    * @param rtol relative tolerance
    * @param atol absolute tolerance
    */
   Tolerance(double rtol = 0.0, double atol = 0.0);

   /// Default copy constructor
   Tolerance(const Tolerance&) = default;

   /// Default assignment operator
   Tolerance& operator=(const Tolerance&) = default;

   /// Default destructor
   ~Tolerance() = default;

   /// Returns the relative tolerance of this
   double getRelTol() const;

   /// Returns the absolute tolerance of this
   double getAbsTol() const;

   /// Sets the relative tolerance of this
   void setRelTol(double val);

   /// Sets the absolute tolerance of this
   void setAbsTol(double val);

   /**
    * @brief Tests if an interval is tight enough with respect to this.
    * 
    * Returns false if x is empty; true if x is canonical ; otherwise it tests
    * if abs(b-a) <= max(rtol*max(abs(b), abs(a)), atol) where x=[a,b], atol
    * and rtol are respectively absolute and relative tolerances
    */
   bool isTight(const Interval& x) const;

   /// Return true if every component of X is tight enough, false otherwise
   bool isTight(const IntervalVector& X) const;

   /**
    * @brief Test of improvement.
    * 
    * Tests if two consecutive intervals of a nested sequence are such that
    * the width of the second one has been reduced enough with respect to the
    * width of the first one according to the relative tolerance
    * 
    * Returns true if (1 - wid(x)/wid(old)) > rtol where rtol is the
    * relative tolerance, false otherwise
    */
   bool isImproved(const Interval& old, const Interval& x) const;

   /**
    * @brief Gets the largest interval having this tolerance.
    * 
    * Returns the largest interval [lb, ub] having the absolute or the
    * relative tolerance
    */
   Interval maxIntervalDn(double ub) const;

   /**
    *  Gets the largest interval having this tolerance
    * 
    * Returns the largest interval [lb, ub] having the absolute or the
    * relative tolerance
    */
   Interval maxIntervalUp(double lb) const;

   /**
    * @brief Calculates the size of a partition of an interval.
    * 
    * Returns 0 if x is empty, 1 if x is canonical, the width of x divided
    * by the absolute tolerance of this rounded upward otherwise
    */
   double discreteSize(const Interval& x) const;

private:
   double rtol_;   // relative tolerance
   double atol_;   // absolute tolerance
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Tolerance& tol);

} // namespace

#endif
