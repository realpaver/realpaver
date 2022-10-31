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

   /// @return the i-th function of thos
   RealFunction fun(size_t i) const;

   /// @return the vector of values after an evaluation or a differentiation
   ///         of this
   const RealVector& values() const;

   /// @return the interval jacobian matrix after a differentiation of this
   const RealMatrix& jacobian() const;

   /// Evaluates this
   /// @param pt values of variables
   ///
   /// values() returns the vector of values
   void eval(const RealPoint& pt);

   /// Differentiates this
   /// @param pt values of variables
   ///
   /// values() returns the vector of values
   /// jacobian() return the jacobian matrix
   void diff(const RealPoint& pt);

private:
   SharedDag dag_;
   RealVector val_;
   RealMatrix jac_;
};

} // namespace

#endif
