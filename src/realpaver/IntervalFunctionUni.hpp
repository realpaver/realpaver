///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_FUNCTION_UNI_HPP
#define REALPAVER_INTERVAL_FUNCTION_UNI_HPP

#include "realpaver/Interval.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of univariate interval functions.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionUni {
public:
   /// Default constructor
   IntervalFunctionUni() = default;

   /// Default copy constructor
   IntervalFunctionUni(const IntervalFunctionUni&) = default;

   /// No assignment
   IntervalFunctionUni& operator=(const IntervalFunctionUni&) = delete;

   /// Virtual destructor
   virtual ~IntervalFunctionUni();

   /// Evaluates this
   /// @param x
   /// @return interval evaluation of this at x
   virtual Interval eval(const Interval& x) = 0;

   /// Differentiates this
   /// @param x
   /// @return interval derivative of this at x
   virtual Interval diff(const Interval& x) = 0;
};

} // namespace

#endif
