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
 * @file   EnvSolver.hpp
 * @brief  Environment for solvers
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_ENV_SOLVER_HPP
#define REALPAVER_ENV_SOLVER_HPP

#include "realpaver/Env.hpp"

namespace realpaver {

/// Environment for solvers
class EnvSolver : public Env {
public:
   /// Constructor
   EnvSolver();

   /// Default destructor
   ~EnvSolver() = default;

   /// Default copy constructor
   EnvSolver(const EnvSolver &) = default;

   /// No assignment
   EnvSolver &operator=(const EnvSolver &) = delete;

   /// Determines if the solving process is aborted due a solution limit
   void setSolutionLimit(bool b = true);

   /// Returns true if a solving process is aborted due a solution limit
   bool usedSolutionLimit() const;

   /// Determines if the a branch of the search tree is cut due a depth limit
   void setDepthLimit(bool b = true);

   /// Returns true if the a branch of the search tree is cut due a depth limit
   bool usedDepthLimit() const;

   /// Returns true if no limit has been activated during the solving
   bool usedNoLimit() const;

private:
   bool slim_; // solution limit reached
   bool dlim_; // depth limit reached in a search tree
};

} // namespace realpaver

#endif
