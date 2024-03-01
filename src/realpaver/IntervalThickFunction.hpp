///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_THICK_FUNCTION_HPP
#define REALPAVER_INTERVAL_THICK_FUNCTION_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunctionUni.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a thick interval function.
///
/// Given a function f(x1, ..., xn) in a DAG, a variable xi and a box B,
/// the thick function is obtained from f by replacing each variable xj but xi
/// by its domain in B.
///
/// The update method must be called first. Then the evaluation and
/// differentiation methods can be called.
///////////////////////////////////////////////////////////////////////////////
class IntervalThickFunction : public IntervalFunctionUni {
public:
   /// Creates a thick function
   /// @param dag a DAG
   /// @param i index of a function in the DAG
   /// @param v a variable occurring in the function
   IntervalThickFunction(SharedDag dag, size_t i, Variable v);

   /// Default copy constructor
   IntervalThickFunction(const IntervalThickFunction&) = default;

   /// No assignment
   IntervalThickFunction& operator=(const IntervalThickFunction&) = delete;

   /// Default destructor
   ~IntervalThickFunction() = default;

   /// Evaluates this
   /// @param x
   /// @return interval evaluation of this at x
   Interval eval(const Interval& x) override;

   /// Differentiates this
   /// @param x
   /// @return interval derivative of this at x
   Interval diff(const Interval& x) override;

   /// Updates the associated function in the DAG
   /// @param B domains of variables
   /// @return interval evaluation of this in B
   Interval update(const IntervalBox& B);

   /// @return the index of the variable of this
   Variable getVar() const;

   /// @return the function in the dag
   DagFun* getFun() const;

private:
   SharedDag dag_;   // dag
   DagFun* f_;       // function in the dag
   Variable v_;      // variable index
};

} // namespace

#endif
