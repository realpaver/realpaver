/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   NLPModel.hpp
 * @brief  Interface for local optimization solvers
 * @author Raphaël Chenouard
 * @date   2024-4-11
 */

#ifndef REALPAVER_NLP_MODEL_HPP
#define REALPAVER_NLP_MODEL_HPP

#include "realpaver/Common.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/RealFunction.hpp"
#include "realpaver/RealFunctionVector.hpp"

namespace realpaver {

/**
 * @brief Interface for local optimization solvers.
 *
 * A NLP model is a nonlinear optimization problem such that an objective
 * function subject to a set of constraints is minimized.
 *
 * The abstract optimization method has to be overriden in sub-classes
 * interfacing concrete solvers.
 */
class NLPModel {
public:
   /// @name Constructors
   ///@{

   /// Constructor from a problem
   NLPModel(const Problem &pb);

   /// Constructor given an objective function to be minimized
   NLPModel(const RealFunction &obj);

   /// Constructor given an objective function and a set of constraints
   NLPModel(const RealFunction &obj, const RealFunctionVector &ctrs);

   /// Default copy constructor
   NLPModel(const NLPModel &) = default;

   ///@}

   /// No assignment
   NLPModel &operator=(const NLPModel &) = delete;

   /// Virtual destructor
   virtual ~NLPModel();

   /**
    * @brief Minimization method.
    *
    * Solves the optimization problem given a box and a starting point src
    * that belongs to this box and returns an optimization status.
    *
    * Both scopes of the box and src must contain the scope of this.
    * They do not necessarily correspond.
    */
   virtual OptimizationStatus minimize(const IntervalBox &box, const RealPoint &src) = 0;

   /// Returns the time limit in seconds for a run of minimize
   double timeLimit() const;

   /// Assigns the time limit in seconds for a run of minimize
   void setTimeLimit(double val);

   /// Returns the iteration limit for a run of minimize
   size_t iterLimit() const;

   /// Assigns the iteration limit for a run of minimize
   void setIterLimit(size_t iter);

   /// Returns the name of the solving algorithm to use
   std::string algorithm() const;

   /// Assigns the name of the solving algorithm to use
   void setAlgorithm(const std::string &name);

   /// Returns the name of the optimization algorithm
   std::string getAlgorithm() const;

   /// Returns the number of variables in the optimization problem
   size_t nbVars() const;

   /// Returns the number of constraints in the optimization problem
   size_t nbCtrs() const;

   /// Returns the a pointer to the objective function
   RealFunction *obj() const;

   /// Returns the pointer to the vector of constraints functions
   RealFunctionVector *ctrs() const;

   /// Returns the best value for the objective function
   double bestVal() const;

   /// Returns the optimal point
   RealPoint bestPoint() const;

   /// Assigns the optimal point
   void setBestPoint(const RealPoint &best);

   /// Returns the optimization status
   OptimizationStatus status() const;

   /// Returns the tolerance on the objective function
   Tolerance tol() const;

   /// Assigns the tolerance on the objective function
   void setTol(Tolerance tol);

   /// Returns the scope of this
   Scope scope() const;

protected:
   RealFunction *obj_;        // Objective function on real numbers
   RealFunctionVector *ctrs_; // Vector of functions for the constraints

   Scope scop_; // union of scopes of the objective function and the constraints

   size_t n_; // number of variables
   size_t m_; // number of constraints

   RealPoint *best_; // Optimal point
   double best_val_; // objective function value for optimal point
   OptimizationStatus status_;

   double time_limit_; // Stop criterion based on time spend to optimize
   size_t iter_limit_; // Stop criterion based on the number of iterations
   Tolerance tol_;     // Stop criterion based on tolerance on the objective

   std::string alg_; // Local optimization algorithm
};

} // namespace realpaver

#endif
