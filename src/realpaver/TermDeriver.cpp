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
 * @file   TermDeriver.cpp
 * @brief  Symbolic differentiation
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/TermDeriver.hpp"

namespace realpaver {

TermDeriver::TermDeriver(Variable v)
      : v_(v),
        dt_()
{}

Term TermDeriver::getDerivative() const
{
   return dt_;
}

void TermDeriver::apply(const TermConst* t)
{
   dt_ = Term(0);
}

void TermDeriver::apply(const TermVar* t)
{
   if (t->var().id() == v_.id())
      dt_ = Term(1);

   else
      dt_ = Term(0);
}

void TermDeriver::apply(const TermAdd* t)
{
   TermDeriver vl(v_);
   t->left()->acceptVisitor(vl);

   TermDeriver vr(v_);
   t->right()->acceptVisitor(vr);

   dt_ = vl.dt_ + vr.dt_;
}

void TermDeriver::apply(const TermSub* t)
{
   TermDeriver vl(v_);
   t->left()->acceptVisitor(vl);

   TermDeriver vr(v_);
   t->right()->acceptVisitor(vr);

   dt_ = vl.dt_ - vr.dt_;
}

void TermDeriver::apply(const TermMul* t)
{
   TermDeriver vl(v_);
   t->left()->acceptVisitor(vl);

   TermDeriver vr(v_);
   t->right()->acceptVisitor(vr);

   dt_ = vl.dt_*t->right() + vr.dt_*t->left();
}

void TermDeriver::apply(const TermDiv* t)
{
   TermDeriver vl(v_);
   t->left()->acceptVisitor(vl);

   TermDeriver vr(v_);
   t->right()->acceptVisitor(vr);

   dt_ = (vl.dt_*t->right() - vr.dt_*t->left()) / sqr(t->right());
}

void TermDeriver::apply(const TermMin* t)
{
   THROW("function min not derivable");
}

void TermDeriver::apply(const TermMax* t)
{
   THROW("function max not derivable");   
}
   
void TermDeriver::apply(const TermUsb* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = -vis.dt_;
}

void TermDeriver::apply(const TermAbs* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = sgn(t->child())*vis.dt_;
}

void TermDeriver::apply(const TermSgn* t)
{
   dt_ = Term(0);
}

void TermDeriver::apply(const TermSqr* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = (2*vis.dt_)*t->child();
}

void TermDeriver::apply(const TermSqrt* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = (0.5*vis.dt_)/sqrt(t->child());
}

void TermDeriver::apply(const TermPow* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);

   int n = t->exponent();
   dt_ = (n*vis.dt_)*pow(t->child(), n-1);   
}

void TermDeriver::apply(const TermExp* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_*exp(t->child());
}

void TermDeriver::apply(const TermLog* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_ / t->child();
}

void TermDeriver::apply(const TermCos* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = (-vis.dt_)*sin(t->child());
}

void TermDeriver::apply(const TermSin* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_*cos(t->child());
}

void TermDeriver::apply(const TermTan* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = (1.0 + sqr(tan(t->child())))*vis.dt_;
}

void TermDeriver::apply(const TermLin* t)
{
   auto it = t->find(v_);
   if (it == t->end())
      dt_ = Term(0);

   else
      dt_ = Term(t->getCoefSub(it));
}

void TermDeriver::apply(const TermCosh* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_*sinh(t->child());
}

void TermDeriver::apply(const TermSinh* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_*cosh(t->child());
}

void TermDeriver::apply(const TermTanh* t)
{
   TermDeriver vis(v_);
   t->child()->acceptVisitor(vis);
   dt_ = vis.dt_*(1.0 - sqr(tanh(t->child())));
}

} // namespace
