///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_FUNCTION_VECTOR_HPP
#define REALPAVER_REAL_FUNCTION_VECTOR_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of real-valued functions based on a DAG.
///////////////////////////////////////////////////////////////////////////////
class RealFunctionVector {
public:
   /// Constructor
   /// @param dag expression DAG representing a vector of functions
   RealFunctionVector(SharedDag dag);

   /// Virtual destructor
   virtual ~RealFunctionVector();

   /// Default copy
   RealFunctionVector(RealFunctionVector&) = default;

   /// No assignment
   RealFunctionVector& operator=(RealFunctionVector&) = delete;

   /// @return the dag
   SharedDag dag() const;

   /// @return the scope of this, i.e. the set of variables
   Scope scope() const;

   /// @return the number of variables in this
   size_t nbVars() const;

   /// @return the number of functions in this
   size_t nbFuns() const;

   /// @return the i-th function of this
   RealFunction fun(size_t i) const;

   /// @return the vector of values after an evaluation of this
   const RealVector& getValues() const;

   /// Evaluates this
   /// @param pt values of variables
   /// @param v output vector 
   ///
   /// getValues() returns the vector of values
   void eval(const RealPoint& pt);

   /// Evaluates and differentiates this (calculates a Jacobian matrix)
   /// @param pt values of variables
   /// @param J Jacobian matrix of this at pt such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J mush have nbFuns() rows and nbVars() columns.
   ///
   /// This is first evaluated, hence f you need both values and derivatives,
   /// calling first eval() is useless and getValues() returns the vector of
   /// values if needed.
   void diff(const RealPoint& pt, RealMatrix& J);

   /// Evaluates this and calculates the violation of the constraints
   /// @param reg domains of variables
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at pt
   ///
   /// getValues() returns the vector of values if needed
   void violation(const RealPoint& pt, RealVector& viol);

private:
   SharedDag dag_;
   RealVector val_;
};

} // namespace

#endif
