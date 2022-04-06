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

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunction.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/RealFunction.hpp"

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
class BOModel : public DiffIntervalFunction, public DiffRealFunction {
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
   IntervalRegion getInitRegion() const;

   /// Sets a variable as a boundary variable
   /// @param v a variable of this
   void setBoundaryVar(Variable v);

   /// Sets a variable as an interior variable
   /// @param v a variable of this
   void setInteriorVar(Variable v);

   /// Tests if a variable is a boundary variable when the model is created
   /// @param v a variable of this
   /// @return true if v is a boundary variable
   bool isBoundaryVar(Variable v) const;

   /// Tests if a variable is a boundary variable in a given region
   /// @param v a variable of this
   /// @param reg domains of variables
   /// @return true if v is a boundary variable in reg
   bool isBoundaryVar(Variable v, const IntervalRegion& reg) const;

   /// Tests if a variable is an interior variable when the model is created
   /// @param v a variable of this
   /// @return true if v is an interior variable
   bool isInteriorVar(Variable v) const;

   /// Tests if a variable is an interior variable in a given region
   /// @param v a variable of this
   /// @param reg domains of variables
   /// @return true if v is an interior variable in reg
   bool isInteriorVar(Variable v, const IntervalRegion& reg) const;

   /// Makes a bitset stating which functions to relax
   /// @param reg domains of variables
   /// @return a bitset such that the i-th bit is equal to 1 if the i-th
   ///         function in the DAG has to be relaxed
   Bitset makeRelaxationBitset(const IntervalRegion& reg);

   /// Linearizes this model in a region
   /// @param reg the given region
   /// @param lm output LP model
   void linearize(const IntervalRegion& reg, LPModel& lm);

   ///@{
   /// Overrides
   Scope  funScope() const;
   size_t funArity() const;

   Interval intervalEval     (const IntervalRegion& reg);
   Interval intervalPointEval(const RealPoint& pt);
   void     intervalDiff     (const IntervalRegion& reg, IntervalVector& g);
   Interval intervalEvalDiff (const IntervalRegion& reg, IntervalVector& g);

   double realEval    (const RealPoint& pt);
   void   realDiff    (const RealPoint& pt, RealVector& g);
   double realEvalDiff(const RealPoint& pt, RealVector& g);
   ///@}

   Dag* getDag();

private:
   Dag* dag_;
   IntervalRegion* init_;
   Variable z_;
   Scope objscope_;     // scope of the objective function
   Scope fullscope_;    // objscope_ union {z_}
   Scope boundary_;     // subset of objscope_

   size_t ic_;          // index of the objective constraint in the DAG

   Dag* odag_;          // DAG used to evaluate the objective function

   // size of the scope of the objective function
   size_t dim() const;

   DagFun* getDerivative(size_t i) const;
   DagFun* getObjConstraint() const;

   friend class BOPresolver;
   friend class BOSolver;
};

} // namespace

#endif
