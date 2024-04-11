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
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONSTRAINT_FIXER_HPP
#define REALPAVER_CONSTRAINT_FIXER_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

/**
 * @brief Visitor that rewrites constraints.
 * 
 * Let c be a constraint, let vvm be a map Variable -> Variable, and let vim
 * be a map Variable -> Interval.
 * - for every entry (v, x) in vvm, every occurrence of v in c is replaced
 *   by x.
 * - for every entry (v, d) in vim, every occurrence of v in c is replaced
 *   by d.
 */
class ConstraintFixer : public ConstraintVisitor {
public:
   /// Map type Variable -> Variable
   using VarVarMapType = TermFixer::VarVarMapType;

   /// Map type Variable -> Interval
   using VarIntervalMapType = TermFixer::VarIntervalMapType;

   /**
    * @brief Constructor.
    * @param vvm map Variable -> Variable
    * @param vim map Variable -> Interval
    * @param B domains of the variables in vvm
    */
   ConstraintFixer(VarVarMapType* vvm, VarIntervalMapType* vim,
                   const IntervalBox& B);

   /// No copy
   ConstraintFixer(const ConstraintFixer&) = delete;

   /// No assignment
   ConstraintFixer& operator=(const ConstraintFixer&) = delete;

   /// Default destructor
   ~ConstraintFixer() = default;

   /// Returns the new constraint after a visit
   Constraint getConstraint() const;

   void apply(const ArithCtrEq* c) override;
   void apply(const ArithCtrLe* c) override;
   void apply(const ArithCtrLt* c) override;
   void apply(const ArithCtrGe* c) override;
   void apply(const ArithCtrGt* c) override;
   void apply(const ArithCtrIn* c) override;
   void apply(const TableCtr* c) override;
   void apply(const CondCtr* c) override;

private:
   VarVarMapType* vvm_;       // map Variable -> Variable
   VarIntervalMapType* vim_;  // map Variable -> Interval
   IntervalBox B_;            // domainsd of variables
   Constraint c_;             // new constraint
};

} // namespace

#endif
