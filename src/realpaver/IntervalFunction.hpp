///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_FUNCTION_HPP
#define REALPAVER_INTERVAL_FUNCTION_HPP

#include "realpaver/IntervalVector.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for differentiable interval-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunction {
public:
   /// Virtual destructor
   virtual ~IntervalFunction();

   /// Evaluates this
   /// @param x argument of this
   /// @return value of this at x
   virtual Interval intervalEval(const IntervalVector& x) = 0;

   /// Differentiates this, i.e. evaluates its gradient
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   virtual void intervalDiff(const IntervalVector& x, IntervalVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   /// @return value of this at x
   virtual Interval intervalEvalDiff(const IntervalVector& x,
                                     IntervalVector& g) = 0;
};

} // namespace

#endif