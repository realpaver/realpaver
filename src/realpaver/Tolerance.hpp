///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TOLERANCE_HPP
#define REALPAVER_TOLERANCE_HPP

#include "realpaver/Interval.hpp"
#include "realpaver/IntervalVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a couple of a relative tolerance and an absolute tolerance.
///////////////////////////////////////////////////////////////////////////////
class Tolerance {
public:
   /// Creates a tolerance
   /// @param rtol relative tolerance
   /// @param atol absolute tolerance
   Tolerance(double rtol = 0.0, double atol = 0.0);

   /// Default copy constructor
   Tolerance(const Tolerance&) = default;

   /// Default assignment operator
   Tolerance& operator=(const Tolerance&) = default;

   /// Default destructor
   ~Tolerance() = default;

   /// @return the relative tolerance of this
   double getRelTol() const;

   /// @return the absolute tolerance of this
   double getAbsTol() const;

   /// Sets the relative tolerance of this
   /// @param val a positive real number
   void setRelTol(double val);

   /// Sets the absolute tolerance of this
   /// @param val a positive real number
   void setAbsTol(double val);

   /// Tests if an interval is tight enough with respect to this
   /// @param x an interval
   /// @return true if x is tight enough, false otherwise
   bool isTight(const Interval& x) const;

   /// Tests if every component of an interval vector is tight enough with
   /// respect to this
   /// @param X an interval vector
   /// @return true if X is tight enough, false otherwise
   bool isTight(const IntervalVector& X) const;

   /// Tests if the distance between two real numbers is tight enough with
   /// respect to this
   /// @param x a number
   /// @param y a number
   /// @return true if the distance between 'x' and 'y' is tight enough,
   ///         false otherwise
   bool isTight(double x, double y) const;

   /// Tests if the distance between two intervals is less than this
   /// @param x an interval
   /// @param y an interval
   /// @return true if the distance between 'x' and 'y' is less than this
   bool areClose(const Interval& x, const Interval& y) const;

   /// Gets the largest interval having this tolerance given its upper bound
   /// @param ub upper of the resulting interval
   /// @return the largest interval [lb, ub] having this tolerance
   //Interval maxIntervalDn(double ub) const;

   /// Gets the largest interval having this tolerance given its lower bound
   /// @param lb lower of the resulting interval
   /// @return the largest interval [lb, ub] having this tolerance
   //Interval maxIntervalUp(double lb) const;

   /// Calculates the size of a partition of an interval with respect to the
   /// absolute tolerance of this
   /// @param x an interval
   /// @return 0 if x is empty, 1 if x is canonical, the width of x divided
   ///         by the absolute tolerance of this rounded upward otherwise
   double discreteSize(const Interval& x) const;

private:
   double rtol_;   // relative tolerance
   double atol_;   // absolute tolerance
};

/// Output on a stream
/// @param os an output stream
/// @param tol a tolerance written os 'os'
/// @return os
std::ostream& operator<<(std::ostream& os, const Tolerance& tol);

} // namespace

#endif
