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
 * @file   ConstraintProver.hpp
 * @brief  Prover for feasible points
 * @author Laurent Granvilliers
 * @date   29 Sep 2025
 */

#ifndef REALPAVER_CONSTRAINT_PROVER_HPP
#define REALPAVER_CONSTRAINT_PROVER_HPP

#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/IntervalBox.hpp"

namespace realpaver {

/**
 * @brief Visitor that proves feasible points.
 *
 * A tolerance tol is given in order to consider any equation f(x) = 0 as |f(x)| <= tol.
 */
class ConstraintProver : public ConstraintVisitor {
public:
   /// Proves that B is feasible given a tolerance tol
   ConstraintProver(IntervalBox &B, double tol = 0.0);

   /// Returns the tolerance
   double getTolerance() const;

   /// After a visit, returns true if the box is feasible, false otherwise
   bool isFeasible() const;

   void apply(const ConstraintCond *c) override;
   void apply(const ConstraintTable *c) override;
   void apply(const ConstraintPiecewise *c) override;
   void apply(const ConstraintEq *c) override;
   void apply(const ConstraintLe *c) override;
   void apply(const ConstraintLt *c) override;
   void apply(const ConstraintGe *c) override;
   void apply(const ConstraintGt *c) override;
   void apply(const ConstraintIn *c) override;

private:
   IntervalBox *B_; // box
   double tol_;     // tolerance
   bool feasible_;  // result of a visit
};

} // namespace realpaver

#endif
