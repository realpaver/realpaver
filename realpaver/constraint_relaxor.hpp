// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONSTRAINT_RELAXOR_HPP
#define REALPAVER_CONSTRAINT_RELAXOR_HPP

#include "realpaver/constraint.hpp"

namespace realpaver {

/*****************************************************************************
 * Constraint visitor used to relax a constraint.
 * 
 * Given a real number nu>0, every equation f(x) = 0 is relaxed as
 * f(x) in [-nu, +nu]. Every other constraint is unchanged.
 *****************************************************************************/
class ConstraintRelaxor : public ConstraintVisitor {
public:
   // constructor
   ConstraintRelaxor(const double& nu);

   // visit methods
   void apply(const ConstraintEq* c);
   void apply(const ConstraintLe* c);
   void apply(const ConstraintLt* c);
   void apply(const ConstraintGe* c);
   void apply(const ConstraintGt* c);
   void apply(const ConstraintIn* c);

   // returns the relaxed constraint after a visit
   Constraint getRelaxed() const;

private:
   double nu_;
   Constraint relaxed_;
};

inline Constraint ConstraintRelaxor::getRelaxed() const
{
   return relaxed_;
}

} // namespace

#endif
