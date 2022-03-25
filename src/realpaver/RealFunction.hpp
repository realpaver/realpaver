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

#include "realpaver/Function.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for real-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class RealFunction : public virtual Function {
public:
   /// Virtual destructor
   virtual ~RealFunction();

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   virtual double realEval(const RealPoint& pt) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for differentiable real-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class DiffRealFunction : public RealFunction {
public:
   /// Virtual destructor
   virtual ~DiffRealFunction();

   /// Differentiates this, i.e. evaluates its gradient
   /// @param pt values of variables
   /// @param g gradient of this at pt
   virtual void realDiff(const RealPoint& pt, RealVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param g gradient of this at pt
   /// @return result of evaluation of this at pt
   virtual double realEvalDiff(const RealPoint& pt, RealVector& g) = 0;
};

} // namespace

#endif
