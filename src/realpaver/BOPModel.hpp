///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BOPMODEL_HPP
#define REALPAVER_BOPMODEL_HPP

#include "realpaver/AbstractRealFunction.hpp"
#include "realpaver/dag.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This represents a bound constrained optimization model.
///
/// Given an objective function f(x0, ..., xk) a dag is created and we have:
/// - the i-th function in the DAG (i in 0..k) corresponds to df / dxi;
/// - the (k+1)-th function in the DAG represents the equation z - f = 0 if
///    f has to be minimized or z + f = 0 otherwise.
///
/// A new variable z is created in the input problem.
///
/// A BOPModel implements the AbstractRealFunction interface, i.e. its
/// objective acts as a real function.
///////////////////////////////////////////////////////////////////////////////
class BOPModel : public AbstractRealFunction {
public:
   /// Creates a model
   /// @param problem problem to be solved
   BOPModel(Problem& problem);

   /// Destructor
   ~BOPModel();

   /// No copy
   BOPModel(const BOPModel&) = delete;

   /// No assignment
   BOPModel& operator=(const BOPModel&) = delete;

   /// @return the variable representing the objective function
   Variable getObjVar() const;

   /// @return the scope of the objective function
   Scope getObjScope() const;

   /// @return the scope of the objective function plus the objective variable
   Scope getFullScope() const;

   /// @return returns the initial region
   IntervalVector getInitRegion() const;

   ///@{
   /// Overrides the methods of AbstractRealFunction
   double realEval(const RealVector& x);
   bool isDifferentiable() const;
   void realDiff(const RealVector& x, RealVector& g);
   double realEvalDiff(const RealVector& x, RealVector& g);
   ///@}

private:
   Dag* dag_;
   IntervalVector initreg_;
   Variable z_;
   Scope objscope_;     // scope of the objective function
   Scope fullscope_;    // scope of the dag (fscope_ + z_)

   // size of the scope of the objective function
   size_t dim() const;
};

} // namespace

#endif
