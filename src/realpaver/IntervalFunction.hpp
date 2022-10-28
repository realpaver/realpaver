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

#include "realpaver/Function.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for interval-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunction : public virtual Function {
public:
   /// Virtual destructor
   virtual ~IntervalFunction();

   /// Evaluates this
   /// @param reg domains of variables
   /// @return interval value of this at r
   virtual Interval intervalEval(const IntervalRegion& reg) = 0;

   /// Evaluates this
   /// @param  ptvalues of variables
   /// @return value of this at x
   virtual Interval intervalPointEval(const RealPoint& pt) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for differentiable interval-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class DiffIntervalFunction : public IntervalFunction {
public:
   /// Virtual destructor
   virtual ~DiffIntervalFunction();

   /// Differentiates this, i.e. evaluates its gradient
   /// @param reg domains of variables
   /// @param grad interval gradient of this at reg (output)
   virtual void intervalDiff(const IntervalRegion& reg,
                             IntervalVector& grad) = 0;

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param value of this at reg
   /// @param grad interval gradient of this at reg
   virtual void intervalEvalDiff(const IntervalRegion& reg,
                                 Interval& val,
                                 IntervalVector& grad) = 0;
};

} // namespace

#endif
