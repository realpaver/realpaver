///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_UNI_INTERVAL_FUNCTION_HPP
#define REALPAVER_UNI_INTERVAL_FUNCTION_HPP

#include "realpaver/Variable.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for univariate interval functions.
///////////////////////////////////////////////////////////////////////////////
class UniIntervalFunction {
public:
   /// Virtual destructor
   virtual ~UniIntervalFunction();

   /// Evaluates this
   /// @param x
   /// @return interval evaluation of this at x
   virtual Interval eval(const Interval& x) = 0;

   /// Differentiates this
   /// @param x
   /// @return interval derivative of this at x
   virtual Interval diff(const Interval& x) = 0;

   /// Evaluates and differentiates this
   /// @param x
   /// @return interval evaluation and derivative of this at x; the first
   ///         component is the value, the second component is the derivative
   virtual std::pair<Interval, Interval> evalDiff(const Interval& x) = 0;
};

} // namespace

#endif
