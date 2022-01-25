// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONSTRAINT_FIXER_HPP
#define REALPAVER_CONSTRAINT_FIXER_HPP

#include "realpaver/constraint.hpp"
#include "realpaver/term_fixer.hpp"

namespace realpaver {

/*****************************************************************************
 * Constraint visitors used to replace fixed variables.
 *****************************************************************************/
class ConstraintFixer : public ConstraintVisitor {
public:

   // map type Variable -> Variable
   typedef TermFixer::VVMap VVMap;

   // map type Variable -> Interval
   typedef TermFixer::VIMap VIMap;

   // constructor given the two maps
   ConstraintFixer(VVMap* vvm, VIMap* vim);

   // returns the new term after a visit
   Constraint getConstraint() const;

   // visit methods
   void apply(const ConstraintEq* c);
   void apply(const ConstraintLe* c);
   void apply(const ConstraintLt* c);
   void apply(const ConstraintGe* c);
   void apply(const ConstraintGt* c);
   void apply(const ConstraintIn* c);

private:
   VVMap* vvm_;
   VIMap* vim_;
   Constraint c_;
};

inline Constraint ConstraintFixer::getConstraint() const
{
   return c_;
}

} // namespace

#endif
