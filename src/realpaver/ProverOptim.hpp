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
 * @file   ProverOptim.hpp
 * @brief  Prover for optimization problems
 * @author Laurent Granvilliers
 * @date   30 Sep 2025
 */

#ifndef REALPAVER_PROVER_OPTIM_HPP
#define REALPAVER_PROVER_OPTIM_HPP

#include "realpaver/Problem.hpp"
#include <vector>

namespace realpaver {

/**
 * @brief Prover for feasible points.
 *
 * A tolerance tol is given in order to consider any equation f(x) = 0 as |f(x)| <= tol.
 */
class ProverOptim {
public:
   /// Constructor
   ProverOptim(const Problem &p, const double &tol = 0.0);

   /// Default destructor
   ~ProverOptim() = default;

   /// No copy
   ProverOptim(const ProverOptim &) = delete;

   /// No asignment
   ProverOptim &operator=(const ProverOptim &) = delete;

   /// Certification method
   bool certify(IntervalBox &B);

   /// Certification method
   bool certify(IntervalBox &B, const Bitset &active);

   /// Returns the tolerance used to relax the equations
   double getTol() const;

   /// Assigns the tolerance used to relax the equations
   void setTol(const double &tol);

private:
   std::vector<Constraint> ctrs_; // initial constraints of a problem
   double tol_;                   // tolerance
};

} // namespace realpaver

#endif
