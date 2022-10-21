///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/ConstraintFixer.hpp"

namespace realpaver {

ConstraintFixer::ConstraintFixer(VarVarMapType* vvm, VarIntervalMapType* vim)
      : vvm_(vvm),
        vim_(vim),
        c_()
{}

Constraint ConstraintFixer::getConstraint() const
{
   return c_;
}

void ConstraintFixer::apply(const ConstraintEq* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() == vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintLe* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() <= vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintLt* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() < vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintGe* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() >= vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintGt* c)
{
   TermFixer vl(vvm_, vim_);
   c->left().acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   c->right().acceptVisitor(vr);

   c_ = (vl.getTerm() > vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintIn* c)
{
   TermFixer vis(vvm_, vim_);
   c->term().acceptVisitor(vis);

   c_ = in(vis.getTerm(), c->image());
}

void ConstraintFixer::apply(const ConstraintITable* c)
{
   // TODO
}

} // namespace
