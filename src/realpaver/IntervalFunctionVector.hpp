///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP
#define REALPAVER_INTERVAL_FUNCTION_VECTOR_HPP

#include "realpaver/Function.hpp"
#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for vectors of interval-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVector : public virtual FunctionVector {
public:
   /// Virtual destructor
   virtual ~IntervalFunctionVector();

   /// Evaluates this
   /// @param reg domains of variables
   /// @param val interval value of this at reg
   virtual void intervalEval(const IntervalRegion& reg, IntervalVector& val) = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @param val value of this at pt
   virtual void intervalPointEval(const RealPoint& pt, IntervalVector& val) = 0;
};

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for vectors of differentiable interval-valued functions.
///
/// This class is a pure abstract class.
///////////////////////////////////////////////////////////////////////////////
class DiffIntervalFunctionVector : public IntervalFunctionVector {
public:
   /// Virtual destructor
   virtual ~DiffIntervalFunctionVector();

   /// Differentiates this, i.e. evaluates its jacobian matrix
   /// @param reg domains of variables
   /// @param jac interval jacobian matrix of this at reg (output)
   virtual void intervalDiff(const IntervalRegion& reg, IntervalMatrix& jac) = 0;

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param val value of this at reg
   /// @param jac interval jacobian matrix of this at reg (output)
   virtual void intervalEvalDiff(const IntervalRegion& reg,
                                 IntervalVector& val, IntervalMatrix& jac) = 0;
};

} // namespace

#endif
