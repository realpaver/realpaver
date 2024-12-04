/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   IntervalFunctionUni.hpp
 * @brief  Base class of univariate interval functions
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_INTERVAL_FUNCTION_UNI_HPP
#define REALPAVER_INTERVAL_FUNCTION_UNI_HPP

#include "realpaver/Interval.hpp"

namespace realpaver {

/// Base class of univariate interval functions
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

   /// Returns the evaluation of this on x
   virtual Interval eval(const Interval& x) = 0;

   /// Returns the interval derivative of this on x
   virtual Interval diff(const Interval& x) = 0;
};

} // namespace

#endif
