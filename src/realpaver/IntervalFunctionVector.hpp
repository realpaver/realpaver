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

   /// @return the vector of values after an evaluation of this
   const IntervalVector& getValues() const;

   /// Evaluates this
   /// @param reg domains of variables
   ///
   /// getValues() returns the vector of values
   void eval(const IntervalRegion& reg);

   /// Evaluates this
   /// @param pt values of variables
   ///
   /// getValues() returns the vector of values
   void pointEval(const RealPoint& pt);

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param reg domains of variables
   /// @param J Jacobian matrix of this at reg such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J mush have nbFuns() rows and nbVars() columns.
   ///
   /// This is first evaluated, hence f you need both values and derivatives,
   /// calling first eval() is useless and getValues() returns the vector of
   /// values if needed.
   void diff(const IntervalRegion& reg, IntervalMatrix& J);

   /// Evaluates this and calculates the violation of the constraints
   /// @param reg domains of variables
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at reg
   ///
   /// getValues() returns the vector of values if needed
   void violation(const IntervalRegion& reg, IntervalVector& viol);

private:
   SharedDag dag_;
   IntervalVector val_;
};

} // namespace

#endif
