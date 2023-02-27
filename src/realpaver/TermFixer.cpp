///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

TermFixer::TermFixer(VarVarMapType* vvm, VarIntervalMapType* vim) :
   vvm_(vvm), vim_(vim), t_()
{}

Term TermFixer::getTerm() const
{
   return t_;
}

void TermFixer::apply(const TermConst* t)
{
   t_ = Term(t->getVal());
}

void TermFixer::apply(const TermVar* t)
{
   auto it1 = vvm_->find(t->var());
   auto it2 = vim_->find(t->var());

   if ((it1 != vvm_->end()) && (it2 != vim_->end()))
      THROW("Term fixer error");

   else if (it1 != vvm_->end())
      t_ = Term(it1->second);

   else if (it2 != vim_->end())
      t_ = Term(it2->second);

   else
      t_ = Term(t->var());
}

void TermFixer::apply(const TermAdd* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = vl.t_ + vr.t_;
}

void TermFixer::apply(const TermSub* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = vl.t_ - vr.t_;   
}

void TermFixer::apply(const TermMul* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = vl.t_ * vr.t_;
}

void TermFixer::apply(const TermDiv* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = vl.t_ / vr.t_;
}

void TermFixer::apply(const TermMin* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = MIN(vl.t_, vr.t_);
}

void TermFixer::apply(const TermMax* t)
{
   TermFixer vl(vvm_, vim_);
   t->left()->acceptVisitor(vl);

   TermFixer vr(vvm_, vim_);
   t->right()->acceptVisitor(vr);

   t_ = MAX(vl.t_, vr.t_);
}

void TermFixer::apply(const TermUsb* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = -vis.t_;
}

void TermFixer::apply(const TermAbs* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = abs(vis.t_);
}

void TermFixer::apply(const TermSgn* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = sgn(vis.t_);
}

void TermFixer::apply(const TermSqr* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = sqr(vis.t_);
}

void TermFixer::apply(const TermSqrt* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = sqrt(vis.t_);
}

void TermFixer::apply(const TermPow* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = pow(vis.t_, t->exponent());
}

void TermFixer::apply(const TermExp* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = exp(vis.t_);
}

void TermFixer::apply(const TermLog* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = log(vis.t_);
}

void TermFixer::apply(const TermCos* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = cos(vis.t_);
}

void TermFixer::apply(const TermSin* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = sin(vis.t_);
}

void TermFixer::apply(const TermTan* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = tan(vis.t_);
}

} // namespace
