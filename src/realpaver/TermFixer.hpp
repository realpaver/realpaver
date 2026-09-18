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
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_TERM_FIXER_HPP
#define REALPAVER_TERM_FIXER_HPP

#include "realpaver/ProblemBridge.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

/**
 * @brief Visitor that rewrites terms.
 *
 * Every variable of a source problem is replaced either by an interval domain
 * if it is fixed, or the corresponding variable in the destination
 * problem. Those problems are given in a bridge.
 */
class TermFixer : public TermVisitor {
public:
   /// Creates a fixer given a problem bridge
   TermFixer(SharedProblemBridge bridge);

   /// No copy
   TermFixer(const TermFixer &) = delete;

   /// No assignment
   TermFixer &operator=(const TermFixer &) = delete;

   /// Default destructor
   ~TermFixer() = default;

   /// Return the new term after a visit
   Term getTerm() const;

   void apply(const TermCst *t) override;
   void apply(const TermVar *t) override;
   void apply(const TermAdd *t) override;
   void apply(const TermSub *t) override;
   void apply(const TermMul *t) override;
   void apply(const TermDiv *t) override;
   void apply(const TermMin *t) override;
   void apply(const TermMax *t) override;
   void apply(const TermUsb *t) override;
   void apply(const TermAbs *t) override;
   void apply(const TermSgn *t) override;
   void apply(const TermSqr *t) override;
   void apply(const TermSqrt *t) override;
   void apply(const TermPow *t) override;
   void apply(const TermPowr *t) override;
   void apply(const TermExp *t) override;
   void apply(const TermLog *t) override;
   void apply(const TermCos *t) override;
   void apply(const TermSin *t) override;
   void apply(const TermTan *t) override;
   void apply(const TermCosh *t) override;
   void apply(const TermSinh *t) override;
   void apply(const TermTanh *t) override;
   void apply(const TermAcos *t) override;
   void apply(const TermAsin *t) override;
   void apply(const TermAtan *t) override;
   void apply(const TermAcosh *t) override;
   void apply(const TermAsinh *t) override;
   void apply(const TermAtanh *t) override;

private:
   SharedProblemBridge bridge_; // bridge between problems
   Term rt_;                    // new term after a visit
};

} // namespace realpaver

#endif
