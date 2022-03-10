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

#include "realpaver/RealPoint.hpp"
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
   virtual Scope rfunScope() const = 0;

   /// @return the number of variables in this
   virtual size_t rfunArity() const = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   virtual double rfunEval(const RealPoint& pt) = 0;

   /// Differentiates this, i.e. evaluates its gradient
   /// @param pt values of variables
   /// @param g gradient of this at pt
   virtual void rfunDiff(const RealPoint& pt, RealVector& g) = 0;

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param g gradient of this at pt
   /// @param e value of this at pt
   virtual void rfunEvalDiff(const RealPoint& pt, RealVector& g,
                             double& e) = 0;
};

} // namespace

#endif
