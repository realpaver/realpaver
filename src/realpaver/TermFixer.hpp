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
 * @file   TermFixer.hpp
 * @brief  Rewriting of terms
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_TERM_FIXER_HPP
#define REALPAVER_TERM_FIXER_HPP

#include "realpaver/Term.hpp"

namespace realpaver {

/**
 * @brief Visitor that rewrites terms.
 * 
 * Let t be a term, let vvm be a map Variable -> Variable, and let vim
 * be a map Variable -> Interval.
 * - for every entry (v, x) in vvm, every occurrence of v in t is replaced
 *   by x.
 * - for every entry (v, d) in vim, every occurrence of v in t is replaced
 *   by d.
 */
class TermFixer : public TermVisitor {
public:
   /// Map type Variable -> Variable
   using VarVarMapType = std::unordered_map<Variable, Variable,
                                            VariableHasher>;

   /// Map type Variable -> Interval
   using VarIntervalMapType = std::unordered_map<Variable, Interval,
                                                 VariableHasher>;

   /// Constructor
   TermFixer(VarVarMapType* vvm, VarIntervalMapType* vim);

   /// No copy
   TermFixer(const TermFixer&) = delete;

   /// No assignment
   TermFixer& operator=(const TermFixer&) = delete;

   /// Default destructor
   ~TermFixer() = default;

   /// Return the new term after a visit
   Term getTerm() const;

   void apply(const TermCst* t) override;
   void apply(const TermVar* t) override;
   void apply(const TermAdd* t) override;
   void apply(const TermSub* t) override;
   void apply(const TermMul* t) override;
   void apply(const TermDiv* t) override;
   void apply(const TermMin* t) override;
   void apply(const TermMax* t) override;
   void apply(const TermUsb* t) override;
   void apply(const TermAbs* t) override;
   void apply(const TermSgn* t) override;
   void apply(const TermSqr* t) override;
   void apply(const TermSqrt* t) override;
   void apply(const TermPow* t) override;
   void apply(const TermExp* t) override;
   void apply(const TermLog* t) override;
   void apply(const TermCos* t) override;
   void apply(const TermSin* t) override;
   void apply(const TermTan* t) override;
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

private:
   VarVarMapType* vvm_;       // map Variable -> Variable
   VarIntervalMapType* vim_;  // map Variable -> Interval
   Term t_;                   // new term after a visit
};

} // namespace

#endif
