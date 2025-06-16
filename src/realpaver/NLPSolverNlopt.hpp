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
 * @file   NLPSolverNlopt.hpp
 * @brief  Implementation of NLP solver for Nlopt
 * @author Raphaël Chenouard
 * @date   2024-4-11
 */

#ifndef REALPAVER_NLP_SOLVER_NLOPT_HPP
#define REALPAVER_NLP_SOLVER_NLOPT_HPP

#include "realpaver/NLPModel.hpp"
#include <nlopt.hpp>

namespace realpaver {

/// Implementation of NLP solver for Nlopt
class NLPSolver : public NLPModel {
public:
   /// @name Constructors
   ///@{

   /// Constructor from a problem
   NLPSolver(const Problem &pb);

   /// Constructor given an objective function to be minimized
   NLPSolver(const RealFunction &obj);

   /// Constructor given an objective function and a set of constraints
   NLPSolver(const RealFunction &obj, const RealFunctionVector &ctrs);
   ///@}
   /// Destructor
   ~NLPSolver();

   /// No copy
   NLPSolver(const NLPSolver &) = delete;

   /// No assignment
   NLPSolver &operator=(const NLPSolver &) = delete;

   OptimizationStatus minimize(const IntervalBox &box, const RealPoint &src) override;

   /// Structure used to process a constraint
   struct Ctr {
      NLPSolver *ls; // the solver vector
      size_t idx;    // constraint index
      bool isleft;   // true if the left bound is considered
   };

private:
   nlopt::opt *optimizer_;      // nlopt optimizer
   nlopt::algorithm nlopt_alg_; // optimization technique
   std::vector<Ctr> nl_ctrs_;   // management of constraints

   void makeAlgorithm();
   void makeCtrs();
};

} // namespace realpaver

#endif
