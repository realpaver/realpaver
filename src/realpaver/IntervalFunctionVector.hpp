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

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of interval-valued functions based on a DAG.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVector {
public:
   /// Constructor
   /// @param dag expression DAG representing a vector of functions
   IntervalFunctionVector(SharedDag dag);

   /// Virtual destructor
   virtual ~IntervalFunctionVector();

   /// Default copy
   IntervalFunctionVector(IntervalFunctionVector&) = default;

   /// No assignment
   IntervalFunctionVector& operator=(IntervalFunctionVector&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of variables in this
   size_t nbVars() const;

   /// @return the number of functions in this
   size_t nbFuns() const;

   /// @return the i-th function of thos
   IntervalFunction fun(size_t i) const;

   /// @return the vector of values after an evaluation or a differentiation
   ///         of this
   const IntervalVector& values() const;

   /// @return the interval jacobian matrix after a differentiation of this
   const IntervalMatrix& jacobian() const;

   /// Evaluates this
   /// @param reg domains of variables
   ///
   /// values() returns the vector of values
   void eval(const IntervalRegion& reg);

   /// Evaluates this
   /// @param pt values of variables
   ///
   /// values() returns the vector of values
   void pointEval(const RealPoint& pt);

   /// Differentiates this
   /// @param reg domains of variables
   ///
   /// values() returns the vector of values
   /// jacobian() returns the jacobian matrix
   void diff(const IntervalRegion& reg);

private:
   SharedDag dag_;
   IntervalVector val_;
   IntervalMatrix jac_;
};

} // namespace

#endif
