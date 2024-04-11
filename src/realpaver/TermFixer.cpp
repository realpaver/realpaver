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
 * @file   TermFixer.cpp
 * @brief  Rewriting of terms
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

TermFixer::TermFixer(VarVarMapType* vvm, VarIntervalMapType* vim)
      : vvm_(vvm),
        vim_(vim),
        t_()
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

void TermFixer::apply(const TermLin* t)
{
   std::shared_ptr<TermLin> tlin = std::make_shared<TermLin>();

   // keeps the constant value
   tlin->addConstant(t->getConstantValue());

   // examines each sub-term
   for (auto it=t->begin(); it!=t->end(); ++it)
   {
      Variable v = t->getVarSub(it);
      Interval x = t->getCoefSub(it);

      auto it1 = vvm_->find(v);
      auto it2 = vim_->find(v);

      if (it1 != vvm_->end())
      {
         Variable w = it1->second;
         tlin->addTerm(x, w);
      }
      else if (it2 != vim_->end())
      {
         Interval y = it2->second;
         tlin->addConstant(x * y);
      }
      else
         THROW("Term fixer error");
   }

   // assigns the result
   if (tlin->isConstant())
      t_ = Term(tlin->evalConst());

   else if (tlin->isVariable())
      t_ = Term(tlin->getVarSub(0));

   else
      t_ = Term(tlin);
}

void TermFixer::apply(const TermCosh* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = cosh(vis.t_);
}

void TermFixer::apply(const TermSinh* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = sinh(vis.t_);
}

void TermFixer::apply(const TermTanh* t)
{
   TermFixer vis(vvm_, vim_);
   t->child()->acceptVisitor(vis);

   t_ = tanh(vis.t_);
}

} // namespace
