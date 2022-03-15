///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_THICK_INTERVAL_FUNCTION_HPP
#define REALPAVER_THICK_INTERVAL_FUNCTION_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/UniIntervalFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a thick interval function.
///
/// Given a function f(x1, ..., xn) in a DAG, a variable xi and a region R,
/// the thick function is obtained from f by replacing each variable xj but xi
/// by its domain in R.
///
/// The update method must be called first. Then the evaluation and
/// differentiation methods can be called.
///////////////////////////////////////////////////////////////////////////////
class ThickIntervalFunction : public UniIntervalFunction {
public:
   /// Creates a thick function
   /// @param dag a DAG
   /// @param i index of a function in the DAG
   /// @param v a variable occurring in the function
   ThickIntervalFunction(Dag* dag, size_t i, Variable v);

   /// Default copy constructor
   ThickIntervalFunction(const ThickIntervalFunction&) = default;

   /// No assignment
   ThickIntervalFunction& operator=(const ThickIntervalFunction&) = delete;

   /// Default destructor
   ~ThickIntervalFunction() = default;

   /// @return the function object in the DAG
   DagFun* getFun() const;

   /// Dependency test
   /// @param bs a bitset
   /// @return true if this depends on bs
   bool dependsOn(const Bitset& bs) const;

   ///@{
   /// Overrides (UniIntervalFunction)
   Interval eval(const Interval& x);
   Interval diff(const Interval& x);
   std::pair<Interval, Interval> evalDiff(const Interval& x);
   ///@}

   /// Updates the associated function in the DAG
   /// @param reg domains of variables
   /// @return interval evaluation of this at reg
   Interval update(const IntervalRegion& reg);

   /// @return the index of the variable of this
   Variable getVar() const;

private:
   DagFun* f_;    // function in the dag
   size_t if_;    // function index in the dag
   Variable v_;   // variable index
};

} // namespace

#endif
