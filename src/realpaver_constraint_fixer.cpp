// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_constraint_fixer.hpp"

namespace realpaver {

ConstraintFixer::ConstraintFixer(VVMap* vvm, VIMap* vim) :
   vvm_(vvm), vim_(vim), c_(nullptr)
{}

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

} // namespace
