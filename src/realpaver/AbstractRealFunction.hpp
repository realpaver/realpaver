///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_ABSTRACT_REAL_FUNCTION_HPP
#define REALPAVER_ABSTRACT_REAL_FUNCTION_HPP

#include "realpaver/RealVector.hpp"
#include "realpaver/scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This an abstraction of a real-valued function.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class AbstractRealFunction {
public:
   /// Virtual destructor
   virtual ~AbstractRealFunction();

   /// Evaluates this
   /// @param x argument of this
   /// @return value of this at x
   virtual double realEval(const RealVector& x) = 0;

   /// @return true if this is differentiable
   virtual bool isDifferentiable() const = 0;

   /// Differentiates this, i.e. evaluates its gradient
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   virtual void realDiff(const RealVector& x, RealVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   /// @return value of this at x
   virtual double realEvalDiff(const RealVector& x, RealVector& g) = 0;
};

} // namespace

#endif
