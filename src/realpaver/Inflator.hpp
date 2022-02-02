///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INFLATOR_HPP
#define REALPAVER_INFLATOR_HPP

#include "realpaver/IntervalVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This inflates intervals.
///
/// Given an interval x and two real numbers delta > 1.0 and chi > 0.0,
/// an inflator calculates the interval m(x) + delta*(x - m(x)) + chi*[-1,1]
/// where m(x) is the midpoint of x.
///////////////////////////////////////////////////////////////////////////////
class Inflator {
public:
   /// Default constructor with typical values of delta and chi
   Inflator();

   /// Creates an inflator
   /// @param delta delta
   /// @param chi chi
   Inflator(double delta, double chi);

   /// Default copy constructor
   Inflator(const Inflator&) = default;

   /// Default asignment operator
   Inflator& operator=(const Inflator&) = default;

   /// Default destructor
   ~Inflator() = default;

   /// @return the value of delta
   double getDelta() const;

   /// Sets the value of delta
   /// @param delta new value of delta
   void setDelta(double delta);

   /// @return the value of chi
   double getChi() const;

   /// Sets the value of chi
   /// @param chi new value of chi
   void setChi(double chi);

   /// Inflates an interval
   /// @param x an interval
   /// @return interval resulting from the inflation of 'x'
   Interval inflate(const Interval& x) const;

   /// Inflates each component of an interval vector
   /// @param X interval vector that is inflated
   void inflate(IntervalVector& X) const;

private:
   double delta_;
   double chi_;
};

} // namespace

#endif
