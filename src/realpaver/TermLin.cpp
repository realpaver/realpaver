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
 * @file   TermLin.hpp
 * @brief  Linear expression
 * @author Laurent Granvilliers
 * @date   2024-6-22
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ScopeBank.hpp"
#include "realpaver/TermLin.hpp"

namespace realpaver {

TermLin::TermLin()
      : cst_(Interval::zero()),
        sl_()
{}

TermLin::TermLin(const Term& t)
      : cst_(Interval::zero()),
        sl_()
{
   TermLinCreator creator(this);
   t.acceptVisitor(creator);

   if (!creator.visitSuccessfull())
      THROW("Creation of linear term failed");
}

bool TermLin::isConstant() const
{
   return sl_.empty();
}

void TermLin::addConstant(const Interval& a)
{
   cst_ += a;
}

void TermLin::addLin(const Interval& a, const Variable& v)
{
   if (a.isZero()) return;

   Lin l = {a, v};
   auto it = sl_.find(l);

   if (it == sl_.end())
   {
      sl_.insert(l);
   }
   else
   {
      Interval x = a + (*it).coef;
      sl_.erase(it);

      if (!x.isZero())
      {
         l.coef = x;
         sl_.insert(l);
      }
   }
}

Term TermLin::toTerm() const
{
   Term t(cst_);

   for (const auto& s : sl_)
   {
      if (s.coef.isCertainlyLeZero())
         t -= (-s.coef)*s.v;

      else
         t += s.coef*s.v;
   }

   return t;
}

Scope TermLin::makeScope() const
{
   Scope scop;

   for (const auto& s : sl_)
      scop.insert(s.v);

   return ScopeBank::getInstance()->insertScope(scop);
}

std::ostream& operator<<(std::ostream& os, const TermLin& t)
{

   bool printed = false;

   for (const auto& s : t.sl_)
   {
      if (printed) os << " ";

      if (s.coef.isPositive())
      {
         if (printed) os << "+ ";
      }
      else
      {
         os << "- ";
      }

      Interval x = abs(s.coef);
         
      if (x.isOne())
      {
         os << s.v.getName();
      }
      else
      {
         os << x << "*" << s.v.getName();
      }
         
      printed = true;
   }

   Interval x = abs(t.cst_);

   if (t.cst_.isZero())
   {
      if (!printed) os << "0";
   }
   else if (t.cst_.isPositive())
   {
      if (printed) os << " + ";
      os << x;
   }
   else
   {
      if (printed) os << " ";
      os << "- " << x;
   }

   return os;
}

/*----------------------------------------------------------------------------*/

TermLinCreator::TermLinCreator(TermLin* lt, const Interval& factor)
      : lt_(lt),
        factor_(factor),
        success_(false)
{}

bool TermLinCreator::visitSuccessfull() const
{
   return success_;
}

void TermLinCreator::apply(const TermCst* t)
{
   lt_->addConstant(factor_ * t->getVal());
   success_ = true;
}

void TermLinCreator::apply(const TermVar* t)
{
   lt_->addLin(factor_, t->var());
   success_ = true;
}

void TermLinCreator::apply(const TermAdd* t)
{
   TermLinCreator vl(lt_, factor_);
   t->left()->acceptVisitor(vl);

   if (!vl.success_) return;

   TermLinCreator vr(lt_, factor_);
   t->right()->acceptVisitor(vr);

   success_ = vr.success_;
}

void TermLinCreator::apply(const TermSub* t)
{
   TermLinCreator vl(lt_, factor_);
   t->left()->acceptVisitor(vl);

   if (!vl.success_) return;

   TermLinCreator vr(lt_, -factor_);
   t->right()->acceptVisitor(vr);

   success_ = vr.success_;
}

void TermLinCreator::apply(const TermMul* t)
{
   if (t->left()->isConstant())
   {
      Interval x = t->left()->evalConst();
      TermLinCreator vis(lt_, x*factor_);
      t->right()->acceptVisitor(vis);
      success_ = vis.success_;
   }
   else if (t->right()->isConstant())
   {
      Interval x = t->right()->evalConst();
      TermLinCreator vis(lt_, x*factor_);
      t->left()->acceptVisitor(vis);
      success_ = vis.success_;   
   }   
}

void TermLinCreator::apply(const TermDiv* t)
{
   if (t->right()->isConstant())
   {
      Interval x = t->right()->evalConst();
      
      if (!x.isZero())
      {
         TermLinCreator vis(lt_, factor_ / x);
         t->left()->acceptVisitor(vis);
         success_ = vis.success_;
      }
   }
}

void TermLinCreator::apply(const TermMin* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermMax* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermUsb* t)
{
   TermLinCreator vis(lt_, -factor_);
   t->child()->acceptVisitor(vis);

   success_ = vis.success_;
}

void TermLinCreator::apply(const TermAbs* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermSgn* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermSqr* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermSqrt* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermPow* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermExp* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermLog* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermCos* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermSin* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermTan* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermCosh* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}


void TermLinCreator::apply(const TermSinh* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}

void TermLinCreator::apply(const TermTanh* t)
{
   if (t->isConstant())
   {
      Interval x = t->evalConst() * factor_;
      lt_->addConstant(x);
      success_ = true;
   }
}

} // namespace
