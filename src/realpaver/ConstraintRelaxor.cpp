///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintRelaxor.hpp"

namespace realpaver {

ConstraintRelaxor::ConstraintRelaxor(double nu)
      : nu_(nu),
        relaxed_(nullptr)
{
   ASSERT(nu_ > 0.0, "Bad relaxation value " << nu);
}

Constraint ConstraintRelaxor::getRelaxedCtr() const
{
   return relaxed_;
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
