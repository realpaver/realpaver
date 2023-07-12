///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NLP_MODEL_HPP
#define REALPAVER_NLP_MODEL_HPP

#include "realpaver/Common.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/IntervalRegion.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealFunctionVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for local optimization solvers.
///////////////////////////////////////////////////////////////////////////////
class NLPModel {
public:
   /// Constructor
   /// @param problem a numerical problem
   NLPModel(const Problem& pb);

   /// Constructor
   /// @param obj a function over reals to be minimized
   NLPModel(const RealFunction& obj);

   /// Constructor
   /// @param obj a function over reals to be minimized
   /// @param ctrs a vector of functions over reals for the constraints
   NLPModel(const RealFunction& obj, const RealFunctionVector& ctrs);

   /// Default copy constructor
   NLPModel(const NLPModel&) = default;

   /// No assignment
   NLPModel& operator=(const NLPModel&) = delete;

   /// Virtual destructor
   virtual ~NLPModel();

   /// Minimization of a function
   /// @param reg interval region in the search space
   /// @param src starting point that belongs to the region
   /// @return an optimization status
   virtual OptimizationStatus minimize(const IntervalRegion& reg,
                                       const RealPoint& src) = 0;

   /// @return the time limit for a run of minimize
   double timeLimit() const;

   /// Assigns the time limit for a run of minimize
   /// @param val time limit in seconds
   void setTimeLimit(double val);

   /// @return the iteration limit for a run of minimize
   size_t iterLimit() const;

   /// Assigns the iteration limit for a run of minimize
   /// @param iter iteration limit
   void setIterLimit(size_t iter);

   /// @return the name of the solving algorithm to use
   std::string algorithm() const;

   /// Assigns the name of the solving algorithm to use
   void setAlgorithm(const std::string& name);

   /// @return the name of the optimization algorithm
   std::string getAlgorithm() const;

   /// @return the number of variables in the optimization problem
   size_t nbVars() const;

   /// @return the number of constraints in the optimization problem
   size_t nbCtrs() const;

   /// @return the a pointer to the objective function
   RealFunction* obj() const;

   /// @return the pointer to the vector of constraints functions
   RealFunctionVector* ctrs() const;

   /// @return the best value for the objective function
   double bestVal() const;

   /// @return the optimal point
   RealPoint bestPoint() const;

   /// Assigns the optimal point
   void setBestPoint(const RealPoint& best);

   /// @return the optimization status
   OptimizationStatus status() const;

   /// @return the absolute tolerance on the objective function
   Tolerance atol() const;

   /// Assigns the absolute tolerance on the objective function
   /// @param tol new value
   void setAtol(Tolerance tol);

   /// @return the relative tolerance on the objective function
   Tolerance rtol() const;

   /// Assigns the relative tolerance on the objective function
   /// @param tol new value
   void setRtol(Tolerance tol);

   /// @return the scope of this
   Scope scope() const;

protected:
   RealFunction* obj_;            // Objective function on real numbers
   RealFunctionVector* ctrs_;     // Vector of functions for the constraints

   Scope scope_;     // union of scopes of the objective function and the constraints

   size_t n_;     // number of variables
   size_t m_;     // number of constraints

   RealPoint* best_;                    // Optimal point
   double best_val_;                    // objective function value for optimal point
   OptimizationStatus status_;

   double time_limit_;                             // Stop criterion based on time spend to optimize
   size_t iter_limit_;                             // Stop criterion based on the number of iterations
   Tolerance atol_;                                // Stop criterion based on absolute tolerance on the objective
   Tolerance rtol_;                                // Stop criterion based on relative tolerance on the objective

   std::string alg_;                               // Local optimization algorithm
};

} // namespace

#endif
