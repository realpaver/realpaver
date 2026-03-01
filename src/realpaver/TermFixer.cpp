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
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/TermFixer.hpp"

namespace realpaver {

TermFixer::TermFixer(SharedProblemBridge bridge)
    : bridge_(bridge)
    , rt_()
{
   REALPAVER_ASSERT(bridge != nullptr, "Bad initialization of a term fixer");
}

Term TermFixer::getTerm() const
{
   return rt_;
}

void TermFixer::apply(const TermCst *t)
{
   rt_ = Term(t->getVal());
}

void TermFixer::apply(const TermVar *t)
{
   Variable v = t->var();
   if (bridge_->isFixed(v))
   {
      rt_ = Term(bridge_->getFixedDomain(v));
   }
   else
   {
      rt_ = Term(bridge_->srcToDestVar(v));
   }
}

void TermFixer::apply(const TermAdd *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = vl.rt_ + vr.rt_;
}

void TermFixer::apply(const TermSub *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = vl.rt_ - vr.rt_;
}

void TermFixer::apply(const TermMul *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = vl.rt_ * vr.rt_;
}

void TermFixer::apply(const TermDiv *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = vl.rt_ / vr.rt_;
}

void TermFixer::apply(const TermMin *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = MIN(vl.rt_, vr.rt_);
}

void TermFixer::apply(const TermMax *t)
{
   TermFixer vl(bridge_), vr(bridge_);
   t->left()->acceptVisitor(vl);
   t->right()->acceptVisitor(vr);
   rt_ = MAX(vl.rt_, vr.rt_);
}

void TermFixer::apply(const TermUsb *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = -vis.rt_;
}

void TermFixer::apply(const TermAbs *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = abs(vis.rt_);
}

void TermFixer::apply(const TermSgn *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = sgn(vis.rt_);
}

void TermFixer::apply(const TermSqr *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = sqr(vis.rt_);
}

void TermFixer::apply(const TermSqrt *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = sqrt(vis.rt_);
}

void TermFixer::apply(const TermPow *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = pow(vis.rt_, t->exponent());
}

void TermFixer::apply(const TermPowr *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = pow(vis.rt_, t->exponent());
}

void TermFixer::apply(const TermExp *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = exp(vis.rt_);
}

void TermFixer::apply(const TermLog *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = log(vis.rt_);
}

void TermFixer::apply(const TermCos *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = cos(vis.rt_);
}

void TermFixer::apply(const TermSin *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = sin(vis.rt_);
}

void TermFixer::apply(const TermTan *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = tan(vis.rt_);
}

void TermFixer::apply(const TermCosh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = cosh(vis.rt_);
}

void TermFixer::apply(const TermSinh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = sinh(vis.rt_);
}

void TermFixer::apply(const TermTanh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = tanh(vis.rt_);
}

void TermFixer::apply(const TermAcos *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = acos(vis.rt_);
}

void TermFixer::apply(const TermAsin *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = asin(vis.rt_);
}

void TermFixer::apply(const TermAtan *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = atan(vis.rt_);
}

void TermFixer::apply(const TermAcosh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = acosh(vis.rt_);
}

void TermFixer::apply(const TermAsinh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = asinh(vis.rt_);
}

void TermFixer::apply(const TermAtanh *t)
{
   TermFixer vis(bridge_);
   t->child()->acceptVisitor(vis);
   rt_ = atanh(vis.rt_);
}

} // namespace realpaver
