///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_FUNCTION_HPP
#define REALPAVER_REAL_FUNCTION_HPP

#include "realpaver/RealVector.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for differentiable real-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class RealFunction {
public:
   /// Virtual destructor
   virtual ~RealFunction();

   /// @return the scopeof this, i.e. the set of variables
   virtual Scope realFunScope() const = 0;

   /// @return the number of variables in this
   virtual size_t realFunArity() const = 0;

   /// Evaluates this
   /// @param x argument of this
   /// @return value of this at x
   virtual double realFunEval(const RealVector& x) = 0;

   /// Differentiates this, i.e. evaluates its gradient
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   virtual void realFunDiff(const RealVector& x, RealVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param x argument of this
   /// @param g gradient of this at x (output)
   /// @param val value of this at x (output)
   virtual void realFunEvalDiff(const RealVector& x, RealVector& g,
                                double& val) = 0;
};

} // namespace

#endif
