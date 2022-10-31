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

   /// @return the value obtained from the last evaluation
   double value() const;

   /// @return the gradient after a differentiation
   const RealVector& gradient() const;

   /// Evaluates this
   /// @param pt values of variables
   /// @return value of this at pt
   double eval(const RealPoint& pt);

   /// Evaluates and differentiates this
   /// @param pt values of variables
   ///
   /// value() returns the value of this
   /// gradient() returns the gradient of this
   void diff(const RealPoint& pt);

private:
   SharedDag dag_;      // DAG
   size_t index_;       // index of function
   double val_;         // value
   RealVector grad_;    // gradient
};

} // namespace

#endif
