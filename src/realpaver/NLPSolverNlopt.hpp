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
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_NLP_SOLVER_NLOPT_HPP
#define REALPAVER_NLP_SOLVER_NLOPT_HPP

#include "realpaver/Bitset.hpp"
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

   /// Creates a minimization NLPModel from a DAG and extended DAG
   NLPSolver(SharedDag obj_dag, int obj_idx, SharedDagExtended edag);

   ///@}
   /// Destructor
   ~NLPSolver();

   /// No copy
   NLPSolver(const NLPSolver &) = delete;

   /// No assignment
   NLPSolver &operator=(const NLPSolver &) = delete;

   OptimizationStatus minimizeImpl(const IntervalBox &box, const RealPoint &src,
                                   const Bitset &active) override;

   /// Structure used to process a constraint
   struct Ctr {
      NLPSolver *ls;  // the solver vector
      SharedDagFun f; // Function of the constraint
      std::shared_ptr<ConstraintRep>
          ctr;     // Constraint pointer for those not stored in a dag
      bool isleft; // true if the left bound is considered
   };

private:
   nlopt::opt *optimizer_;      // nlopt optimizer
   nlopt::algorithm nlopt_alg_; // optimization technique
   std::vector<Ctr> nl_ctrs_;   // management of constraints

   void makeAlgorithm();
   void makeCtrs(const IntervalBox &box, const Bitset &active);
};

} // namespace realpaver

#endif
