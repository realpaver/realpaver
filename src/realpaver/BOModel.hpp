///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_MODEL_HPP
#define REALPAVER_BO_MODEL_HPP

#include "realpaver/RealFunction.hpp"
#include "realpaver/dag.hpp"
#include "realpaver/Problem.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This represents a bound constrained optimization model.
///
/// Given an objective function f(x0, ..., xk) a dag is created and the i-th
/// function in the DAG (i in 0..k) corresponds to df / dxi.
///
/// A (k+1)-th function can be created in the DAG in order to represent the
/// equation z - f = 0 if f has to be minimized or z + f = 0 otherwise.
/// To this end, a new variable z is created in the input problem.
///
/// A model can be created from an original problem or a simplified problem.
/// It is then important to distinguish boundary variables and interior
/// variables in order to find optimal points. The domain of a boundary
/// variable shares at least one bound with the corresponding domain in the
/// original problem. The domain of an interior variable in strictly included
/// in the corresponding domain in the original problem. The default status of
/// a variable is Boundary.
///
/// This class implements the RealFunction interface since its objective
/// function acts as a real function.
///////////////////////////////////////////////////////////////////////////////
class BOModel : public RealFunction {
public:
   /// Creates a model
   /// @param problem problem to be solved
   /// @param withobj true if the objective function is added in this
   ///
   /// The equations df / dxi = 0 are inserted in this.
   ///
   /// If withobj is true, a new variable z is created in the problem and
   /// an equation representing the objective function is inserted in this.
   BOModel(Problem& problem, bool withobj = true);

   /// Destructor
   ~BOModel();

   /// No copy
   BOModel(const BOModel&) = delete;

   /// No assignment
   BOModel& operator=(const BOModel&) = delete;

   /// @return the variable representing the objective function
   Variable getObjVar() const;

   /// @return the scope of the objective function
   Scope getObjScope() const;

   /// @return the scope of the objective function plus the objective variable
   Scope getFullScope() const;

   /// @return returns the initial region
   IntervalVector getInitRegion() const;

   /// 
   void setBoundaryVar(const Variable& v);

   /// 
   void setInteriorVar(const Variable& v);

   ///
   bool isBoundaryVar(const Variable& v) const;

   ///
   bool isInteriorVar(const Variable& v) const;

   ///@{
   /// Overrides the methods of RealFunction
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
   Scope fullscope_;    // objscope_ union {z_}
   Scope boundary_;     // subset of objscope_

   // size of the scope of the objective function
   size_t dim() const;

   Dag* getDag();
   friend class BOPresolver;
};

} // namespace

#endif
