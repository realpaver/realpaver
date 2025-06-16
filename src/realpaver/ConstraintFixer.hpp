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
 * @file   ConstraintFixer.hpp
 * @brief  Rewriting of constraints
 * @author Laurent Granvilliers
 * @date   28 may 2025
 */

#ifndef REALPAVER_CONSTRAINT_FIXER_HPP
#define REALPAVER_CONSTRAINT_FIXER_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/ProblemBridge.hpp"

namespace realpaver {

/**
 * @brief Visitor that rewrites constraints.
 */
class ConstraintFixer : public ConstraintVisitor {
public:
   /// Creates a fixer using a bridge
   ConstraintFixer(SharedProblemBridge bridge);

   /// No copy
   ConstraintFixer(const ConstraintFixer &) = delete;

   /// No assignment
   ConstraintFixer &operator=(const ConstraintFixer &) = delete;

   /// Default destructor
   ~ConstraintFixer() = default;

   /// Returns the new constraint after a visit
   Constraint getConstraint() const;

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
   SharedProblemBridge bridge_; // bridge between problems
   Constraint rc_;              // new constraint
};

} // namespace realpaver

#endif
