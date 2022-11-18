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

#include "realpaver/Dag.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a real-valued function.
///
/// This class is an adapter of the DagFun class.
///////////////////////////////////////////////////////////////////////////////
class RealFunction {
public:
   /// Constructor
   /// @param dag expression graph
   /// @param i index of function in the DAG
   RealFunction(SharedDag dag, size_t i);

   /// Virtual destructor
   virtual ~RealFunction();

   /// Copy constructor
   RealFunction(const RealFunction&) = default;

   /// No asssignment
   RealFunction& operator=(const RealFunction&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of arguments of this
   size_t nbVars() const;

   /// @return the image of this
   Interval getImage() const;

   /// @return the value obtained from the last evaluation
   double getValue() const;

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   double eval(const RealPoint& pt);

   /// Evaluates and differentiates this
   /// @param pt values of variables
   /// @param grad output vector such that grad[i] if the derivative of this
   /// at pt with respect to the i-th variable of its scope
   ///
   /// getValue() returns the value of this if needed
   void diff(const RealPoint& pt, RealVector& grad);

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt the values of the variables
   /// @return 0.0 if the constraint is satisfied, a positive real number
   ///         otherwise equal to the width of the gap between the image of the
   ///         function and the result of its evaluation at pt
   ///
   /// Given [lo, up] the image of this in the DagFun object, the underlying
   /// constraint is defined by lo <= f(x) <= up.
   ///
   /// getValue() returns the value of this if needed
   double violation(const RealPoint& pt);

   /// Evaluates this and calculates the violation of the underlying constraint
   /// @param pt the values of the variables
   /// @param lo left bound for this
   /// @param up right bound for this
   /// @return 0.0 if the constraint is satisfied, a positive real number
   ///         otherwise equal to the width of the gap between the image of the
   ///         function and the result of its evaluation at pt
   ///
   /// The underlying constraint is defined by lo <= f(x) <= up.
   ///
   /// getValue() returns the value of this if needed
   double violation(const RealPoint& pt, double lo, double up);

private:
   SharedDag dag_;      // DAG
   size_t index_;       // index of function
   double val_;         // value
};

} // namespace

#endif
