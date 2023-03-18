///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
   dt_ = (1 + sqr(tan(t->child())))*vis.dt_;
}

void TermDeriver::apply(const TermLin* t)
{
   auto it = t->find(v_);
   if (it == t->end())
      dt_ = Term(0);

   else
      dt_ = Term(t->getCoefSub(it));
}

} // namespace
