// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/constraint_relaxor.hpp"

namespace realpaver {

ConstraintRelaxor::ConstraintRelaxor(const double& nu) :
   nu_(nu), relaxed_(nullptr)
{
   ASSERT(nu_ > 0.0, "Bad relaxation value");
}

void ConstraintRelaxor::apply(const ConstraintEq* c)
{
   relaxed_ = in(c->left() - c->right(), Interval(-nu_, nu_));
}

void ConstraintRelaxor::apply(const ConstraintLe* c)
{
   relaxed_ = (c->left() <= c->right());
}

void ConstraintRelaxor::apply(const ConstraintLt* c)
{
   relaxed_ = (c->left() < c->right());
}

void ConstraintRelaxor::apply(const ConstraintGe* c)
{
   relaxed_ = (c->left() >= c->right());
}

void ConstraintRelaxor::apply(const ConstraintGt* c)
{
   relaxed_ = (c->left() > c->right());
}

void ConstraintRelaxor::apply(const ConstraintIn* c)
{
   relaxed_ = in(c->term(), c->image());
}

} // namespace
