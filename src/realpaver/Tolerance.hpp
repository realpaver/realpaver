///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_TOLERANCE_HPP
#define REALPAVER_TOLERANCE_HPP

#include "realpaver/Interval.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a tolerance.
///
/// The tolerance of an interval x is equal to
/// # -1 if x is empty
/// # 0 if x is canonical
/// # the width of x if the precision is ABSOLUTE
/// # otherwise
///     ## the width of x if x is included in [-1,+1]
///     ## the relative width of x otherwise
///////////////////////////////////////////////////////////////////////////////
class Tolerance {
public:
   /// Creates a tolerance
   /// @param val      magnitude given as a positive real number
   /// @param absolute true for an absolute tolerance,
   ///                 false for a relative tolerance
   Tolerance(double val = 0.0, bool absolute = true);

   /// Creates a tolerance
   /// @param str string representation of the tolerance, e.g.
   ///        1.0e-6A for an absolute tolerance, 1.0-6R for a relative tolerance
   ///
   /// Throws an exception if the format is not respected.
   Tolerance(const std::string& str);

   /// Default copy constructor
   Tolerance(const Tolerance&) = default;

   /// Default assignment operator
   Tolerance& operator=(const Tolerance&) = default;

   /// Default destructor
   ~Tolerance() = default;

   /// Creates an absolute precision
   /// @param val magnitude given as a positive real number
   /// @return an absolute tolerance having the given magnitude
   static Tolerance makeAbs(double val);

   /// Creates a relative precision
   /// @param val magnitude given as a positive real number
   /// @return an relative tolerance having the given magnitude
   static Tolerance makeRel(double val);

   /// @return the magnitude of this
   double getVal() const;

   /// Sets the magnitude of this
   /// @param val magnitude given as a positive real number
   void setVal(double val);

   /// @return true if this is absolute, false otherwise
   bool isAbsolute() const;

   /// @return true if this is relative, false otherwise
   bool isRelative() const;

   /// Calculates the tolerance of an interval
   /// @param x an interval
   /// @return -1 if 'x' is empty,
   ///         0 if 'x' is canonical,
   ///         the width of 'x' if this is absolute,
   ///         the width of 'x' if 'x' is included in [-1, 1],
   ///         the relative width of 'x' otherwise
   double toleranceOf(const Interval& x);

   /// Tests if the tolerance of an interval is less than this
   /// @param x an interval
   /// @return true if the tolerance of 'x' is less than this
   bool hasTolerance(const Interval& x) const;

   /// Tests if the distance between two real numbers is less than this
   /// @param x a number
   /// @param y a number
   /// @return true if the distance between 'x' and 'y' is less than this
   bool hasTolerance(double x, double y) const;

   /// Tests if the distance between two intervals is less than this
   /// @param x an interval
   /// @param y an interval
   /// @return true if the distance between 'x' and 'y' is less than this
   bool haveDistTolerance(const Interval& x, const Interval& y) const;

   /// Gets the largest interval having this tolerance given its upper bound
   /// @param ub upper of the resulting interval
   /// @return the largest interval [lb, ub] having this tolerance
   Interval maxIntervalDn(double ub) const;

   /// Gets the largest interval having this tolerance given its lower bound
   /// @param lb lower of the resulting interval
   /// @return the largest interval [lb, ub] having this tolerance
   Interval maxIntervalUp(double lb) const;

private:
   double val_;   // magnitude
   bool abs_;     // true for an absolute precision
};

/// Output on a stream
/// @param os an output stream
/// @param tol a tolerance written os 'os'
/// @return os
std::ostream& operator<<(std::ostream& os, const Tolerance& tol);

} // namespace

#endif
