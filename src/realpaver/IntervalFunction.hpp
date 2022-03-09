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

#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealVector.hpp"
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

   /// @return the scopeof this, i.e. the set of variables
   virtual Scope ifunScope() const = 0;

   /// @return the number of variables in this
   virtual size_t ifunArity() const = 0;

   /// Evaluates this
   /// @param reg domains of variables
   /// @return interval value of this at r
   virtual Interval ifunEval(const IntervalRegion& reg) = 0;

   /// Evaluates this
   /// @param x values of variables
   /// @return value of this at x
   virtual Interval ifunEvalPoint(const RealVector& x) = 0;

   /// Differentiates this, i.e. evaluates its gradient
   /// @param reg domains of variables
   /// @param g interval gradient of this at r (output)
   virtual void ifunDiff(const IntervalRegion& reg, IntervalVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param g interval gradient of this at r (output)
   /// @param e interval value of this at r (output)
   virtual void ifunEvalDiff(const IntervalRegion& reg, IntervalVector& g,
                             Interval& e) = 0;
};

} // namespace

#endif
