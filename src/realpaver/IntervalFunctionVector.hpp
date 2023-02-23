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

#include <vector>
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalFunction.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of the hirarchy of representations of interval
/// function vectors.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVectorRep {
public:
   /// Default constructor
   IntervalFunctionVectorRep() = default;

   /// Virtual destructor
   virtual ~IntervalFunctionVectorRep();

   /// Default copy constructor
   IntervalFunctionVectorRep(const IntervalFunctionVectorRep&) = default;

   /// No assignment
   IntervalFunctionVectorRep&
   operator=(const IntervalFunctionVectorRep&) = delete;

   /// @return the scope of this, i.e. the set of variables
   virtual Scope scope() const = 0;

   /// @return the number of variables in this
   virtual size_t nbVars() const = 0;

   /// @return the number of functions in this
   virtual size_t nbFuns() const = 0;

   /// @return the i-th function of this
   virtual IntervalFunction fun(size_t i) const = 0;

   /// Evaluates this
   /// @param reg domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at reg
   ///
   /// val must have nbFuns() components.
   virtual void eval(const IntervalRegion& reg, IntervalVector& val) = 0;

   /// Evaluates this
   /// @param pt values of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   ///
   /// val must have nbFuns() components.
   virtual void pointEval(const RealPoint& pt, IntervalVector& val) = 0;

   /// Differentiates this (calculates an interval Jacobian matrix)
   /// @param reg domains of variables
   /// @param J Jacobian matrix of this at reg such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns.
   virtual void diff(const IntervalRegion& reg, IntervalMatrix& J) = 0;

   /// Evaluates and differentiates this
   /// @param reg domains of variables
   /// @param val output vector such that val[i] is the result of the evaluation
   ///        of the i-th function of this at pt
   /// @param J Jacobian matrix of this at reg such that we have the partial
   ///        derivative dfi / dxj in the i-th row and j-th column of J
   ///
   /// J must have nbFuns() rows and nbVars() columns; val must have
   /// nbFuns() components.
   virtual void evalDiff(const IntervalRegion& reg, IntervalVector& val,
                         IntervalMatrix& J) = 0;

   /// Evaluates this and calculates the violation of the constraints
   /// @param reg domains of variables
   /// @param viol output vector such that viol[i] is the violation of the
   ///        i-th function / constraint at reg
   virtual void violation(const IntervalRegion& reg, IntervalVector& viol) = 0;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVector {
public:

private:
   
};

///////////////////////////////////////////////////////////////////////////////
/// This is a vector of interval-valued functions based on a DAG such that
/// all the functions of the DAG are considered.
///////////////////////////////////////////////////////////////////////////////
class IntervalFunctionVectorDag : public IntervalFunctionVectorRep {
public:

private:
   SharedDag dag_;
};

} // namespace

#endif
