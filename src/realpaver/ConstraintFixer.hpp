///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONSTRAINT_FIXER_HPP
#define REALPAVER_CONSTRAINT_FIXER_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor of terms that replaces variables.
///
/// Let c be a constraint, let vvm be a map Variable -> Variable, and let vim
/// be a map Variable -> Interval.
/// - for every entry (v, x) in vvm, every occurrence of v in c is replaced
///   by x;
/// - for every entry (v, d) in vim, every occurrence of v in c is replaced
/// - by d.
///////////////////////////////////////////////////////////////////////////////
class ConstraintFixer : public ConstraintVisitor {
public:
   /// map type Variable -> Variable
   typedef TermFixer::VarVarMapType VarVarMapType;

   /// map type Variable -> Interval
   typedef TermFixer::VarIntervalMapType VarIntervalMapType;

   /// Creates a fixer given two maps
   /// @param vvm map Variable -> Variable
   /// @param vim map Variable -> Interval
   /// @param reg domains of the variables in vvm
   ConstraintFixer(VarVarMapType* vvm, VarIntervalMapType* vim,
                   const IntervalRegion& reg);

   /// No copy
   ConstraintFixer(const ConstraintFixer&) = delete;

   /// No assignment
   ConstraintFixer& operator=(const ConstraintFixer&) = delete;

   /// Default destructor
   ~ConstraintFixer() = default;

   /// @return the new constraint after a visit
   Constraint getConstraint() const;

   ///@{
   void apply(const ArithCtrEq* c) override;
   void apply(const ArithCtrLe* c) override;
   void apply(const ArithCtrLt* c) override;
   void apply(const ArithCtrGe* c) override;
   void apply(const ArithCtrGt* c) override;
   void apply(const ArithCtrIn* c) override;
   void apply(const TableCtr* c) override;
   ///@}

private:
   VarVarMapType* vvm_;
   VarIntervalMapType* vim_;
   IntervalRegion reg_;
   Constraint c_;
};

} // namespace

#endif
