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

#include "realpaver/Dag.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interval-valued function.
///
/// This class is an adapter of the DagFun class.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunction {
public:
   /// Constructor
   /// @param dag expression graph
   /// @param i index of function in the DAG
   IntervalFunction(SharedDag dag, size_t i);

   /// Virtual destructor
   virtual ~IntervalFunction();

   /// Copy constructor
   IntervalFunction(const IntervalFunction&) = default;

   /// No asssignment
   IntervalFunction& operator=(const IntervalFunction&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of arguments of this
   size_t nbVars() const;

   /// @return the image of this
   Interval getImage() const;

   /// @return the value obtained from the last evaluation
   Interval value() const;

   /// @return the gradient after a differentiation
   const IntervalVector& gradient() const;

   /// Evaluates this
   /// @param reg domains of variables
   /// @return value of this at reg
   Interval eval(const IntervalRegion& reg);

   /// Evaluates this
   /// @param  pt values of variables
   /// @return value of this at pt
   Interval pointEval(const RealPoint& pt);

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   ///
   /// value() returns the value of this
   /// gradient() returns the gradient of this
   void diff(const IntervalRegion& reg);

   /// Calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @return 0.0 if the constraint is satisfied, a positive real number
   ///         otherwise equal to the width of the gap between the image of the
   ///         function and the result of its evaluation at reg
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   double violation(const IntervalRegion& reg);

   /// Calculates the violation of the underlying constraint
   /// @param reg domains of variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @return 0.0 if the constraint is satisfied, a positive real number
   ///         otherwise equal to the width of the gap between the image of the
   ///         function and the result of its evaluation at reg
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   double violation(const IntervalRegion& reg, double lo, double up);

private:
   SharedDag dag_;         // DAG
   size_t index_;          // index of function
   Interval val_;          // value
   IntervalVector grad_;   // gradient
};

} // namespace

#endif
