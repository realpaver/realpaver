///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/QuadraticTerm.hpp"

namespace realpaver {

QuadraticTerm::QuadraticTerm()
   : cst_(Interval::zero()),
     sq_(),
     sb_(),
     sl_()
{}

QuadraticTerm::QuadraticTerm(const Term& t)
   : cst_(Interval::zero()),
     sq_(),
     sb_(),
     sl_()
{
   QuadraticTermCreator creator(this);
   t.acceptVisitor(creator);

   if (!creator.visitSuccessfull())
      THROW("Creation of quadratic term failed");
}

void QuadraticTerm::addConstant(const Interval& a)
{
   cst_ += a;
}

void QuadraticTerm::addSquare(const Interval& a, const Variable& v)
{
   if (a.isZero()) return;

   Square s = {a, v};
   auto it = sq_.find(s);

   if (it == sq_.end())
   {
      sq_.insert(s);
   }
   else
   {
      Interval x = a + (*it).coef;
      sq_.erase(it);

      if (!x.isZero())
      {
         s.coef = x;
         sq_.insert(s);
      }
   }
}

void QuadraticTerm::addBilin(const Interval& a, const Variable& v1,
                             const Variable& v2)
{
   if (a.isZero()) return;
   
   if (v1.id() == v2.id())
   {
      addSquare(a, v1);
      return;
   }

   Bilin b = {a, v1, v2};
   if (v1.id()>v2.id()) b = {a, v2, v1};

   auto it = sb_.find(b);

   if (it == sb_.end())
   {
      sb_.insert(b);
   }
   else
   {
      Interval x = a + (*it).coef;
      sb_.erase(it);

      if (!x.isZero())
      {
         b.coef = x;
         sb_.insert(b);
      }
   }
}

void QuadraticTerm::addLin(const Interval& a, const Variable& v)
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

Term QuadraticTerm::toTerm() const
{
   Term t(cst_);
   
   for (const auto& s : sq_)
   {
      if (s.coef.isCertainlyLeZero())
         t -= (-s.coef)*sqr(s.v);

      else
         t += s.coef*sqr(s.v);
   }
   
   for (const auto& s : sb_)
   {
      if (s.coef.isCertainlyLeZero())
         t -= (-s.coef)*s.v1*s.v2;

      else
         t += s.coef*s.v1*s.v2;
   }

   for (const auto& s : sl_)
   {
      if (s.coef.isCertainlyLeZero())
         t -= (-s.coef)*s.v;

      else
         t += s.coef*s.v;
   }

   return t;
}


std::ostream& operator<<(std::ostream& os, const QuadraticTerm& t)
{
   for (const auto& s : t.sq_)
      os << "(" << s.coef << " " << s.v.getName() << "^2)";

   for (const auto& s : t.sb_)
      os << "(" << s.coef << " "
         << s.v1.getName() << " " << s.v2.getName() << ")";

   for (const auto& s : t.sl_)
      os << "(" << s.coef << " " << s.v.getName() << ")";

   if (!t.cst_.isZero())
      os << "(" << t.cst_ << ")";

   return os;
}

Scope QuadraticTerm::makeScope() const
{
   Scope sco;

   for (const auto& s : sq_)
      sco.insert(s.v);

   for (const auto& s : sb_)
   {
      sco.insert(s.v1);
      sco.insert(s.v2);
   }

   for (const auto& s : sl_)
      sco.insert(s.v);

   return sco;
}

Term QuadraticTerm::factorize() const
{
   std::list<Variable> lv;
   sortByOcc(lv);

   Term t(cst_);

   // copy of the set of bilinear terms
   std::set<Bilin, CompBilin> bi(sb_);

   for (const Variable& v : lv)
   {
      Term vfactor(0.0);

      // quadratic term
      Square s = {1.0, v};
      auto it = sq_.find(s);
      if (it != sq_.end())
      {
         if ((*it).coef.isCertainlyLeZero())
            vfactor -= (-(*it).coef) * v;

         else
            vfactor += (*it).coef * v;
      }

      // bilinear terms
      auto kt = bi.begin();
      while (kt != bi.end())
      {
         if ((*kt).v1.id() == v.id())
         {
            if ((*kt).coef.isCertainlyLeZero())
               vfactor -= (-(*kt).coef) * (*kt).v2;

            else
               vfactor += (*kt).coef * (*kt).v2;

            kt = bi.erase(kt);
         }
         else if ((*kt).v2.id() == v.id())
         {
            if ((*kt).coef.isCertainlyLeZero())
               vfactor -= (-(*kt).coef) * (*kt).v1;

            else
               vfactor += (*kt).coef * (*kt).v1;

            kt = bi.erase(kt);            
         }
         else ++kt;
      }

      // linear term
      Lin l = {1.0, v};
      auto jt = sl_.find(l);
      if (jt != sl_.end())
      {
         if ((*jt).coef.isCertainlyLeZero())
            vfactor -= -(*jt).coef;

         else
            vfactor += (*jt).coef;
      }

      // inserts the factorized sub-term in the result
      t += (v * vfactor);
   }

   return t;
}

void QuadraticTerm::sortByOcc(std::list<Variable>& lv) const
{
   Scope sco = makeScope();

   for (const Variable& v : sco)
   {
      int n = sco.count(v);
      auto it = lv.begin();
      bool iter = true;
      while (iter)
      {
         if (it == lv.end())
            iter = false;

         else
         {
            int m = sco.count(*it);
            if (n >= m)
               iter = false;

            else ++it;
         }
      }

      lv.insert(it, v);
   }
}

///////////////////////////////////////////////////////////////////////////////

QuadraticTermCreator::QuadraticTermCreator(QuadraticTerm* qt)
   : qt_(qt),
     plus_(true),
     success_(false)
{
   ASSERT(qt != nullptr, "Null pointer in a creator of quadratic term");
}

bool QuadraticTermCreator::visitSuccessfull() const
{
   return success_;
}

QuadraticTerm* QuadraticTermCreator::getQuadraticTerm() const
{
   return qt_;
}

void QuadraticTermCreator::apply(const TermConst* t)
{
   qt_->addConstant(plus_ ? t->getVal() : -t->getVal());
   success_ = true;
}

void QuadraticTermCreator::apply(const TermVar* t)
{
   qt_->addLin(plus_ ? 1.0 : -1.0, t->var());
   success_ = true;
}

void QuadraticTermCreator::apply(const TermAdd* t)
{
   QuadraticTermCreator vl(qt_);
   vl.plus_ = plus_;
   t->left()->acceptVisitor(vl);

   QuadraticTermCreator vr(qt_);
   vr.plus_ = plus_;
   t->right()->acceptVisitor(vr);

   success_ = vl.success_ & vr.success_;
}

void QuadraticTermCreator::apply(const TermSub* t)
{
   QuadraticTermCreator vl(qt_);
   vl.plus_ = plus_;
   t->left()->acceptVisitor(vl);

   QuadraticTermCreator vr(qt_);
   vr.plus_ = !plus_;
   t->right()->acceptVisitor(vr);

   success_ = vl.success_ & vr.success_;
}

void QuadraticTermCreator::apply(const TermMul* t)
{
   const TermConst* lcst = dynamic_cast<const TermConst*>(t->left().get());
   const TermConst* rcst = dynamic_cast<const TermConst*>(t->right().get());

   const TermVar* lvar = dynamic_cast<const TermVar*>(t->left().get());
   const TermVar* rvar = dynamic_cast<const TermVar*>(t->right().get());

   const TermMul* lmul = dynamic_cast<const TermMul*>(t->left().get());
   const TermMul* rmul = dynamic_cast<const TermMul*>(t->right().get());

   const TermSqr* lsqr = dynamic_cast<const TermSqr*>(t->left().get());
   const TermSqr* rsqr = dynamic_cast<const TermSqr*>(t->right().get());

   if (lcst != nullptr && rcst != nullptr)
   {
      Interval x = lcst->getVal() * rcst->getVal();
      qt_->addConstant(plus_ ? x : -x);
      success_ = true;
   }
   else if (lcst != nullptr && rvar != nullptr)
   {
      qt_->addLin(plus_ ? lcst->getVal() : -lcst->getVal(), rvar->var());
      success_ = true;      
   }
   else if (lcst != nullptr && rmul != nullptr)
   {
      success_ = makeProd(lcst, rmul);
   }
   else if (lcst != nullptr && rsqr != nullptr)
   {
      success_ = makeProd(lcst, rsqr);
   }
   else if (lvar != nullptr && rcst != nullptr)
   {
      qt_->addLin(plus_ ? rcst->getVal() : -rcst->getVal() , lvar->var());
      success_ = true;
   }
   else if (lvar != nullptr && rvar != nullptr)
   {
      qt_->addBilin(plus_ ? 1.0 : -1.0, lvar->var(), rvar->var());
      success_ = true;
   }
   else if (lvar != nullptr && rmul != nullptr)
   {
      success_ =  makeProd(lvar, rmul);
   }
   else if (lmul != nullptr && rcst != nullptr)
   {
      success_ =  makeProd(rcst, rmul);
   }
   else if (lmul != nullptr && rvar != nullptr)
   {
      success_ =  makeProd(rvar, lmul);
   }
   else if (lsqr != nullptr && rcst != nullptr)
   {
      success_ = makeProd(rcst, lsqr);
   }
}

bool QuadraticTermCreator::makeProd(const TermConst* tc, const TermMul* tm)
{
   const TermVar* tvl = dynamic_cast<const TermVar*>(tm->left().get());
   const TermVar* tvr = dynamic_cast<const TermVar*>(tm->right().get());

   if (tvl == nullptr || tvr == nullptr)
      return false;

   else
   {
      qt_->addBilin(plus_ ? tc->getVal() : -tc->getVal(), tvl->var(),
                    tvr->var());
      return true;
   }
}

bool QuadraticTermCreator::makeProd(const TermConst* tc, const TermSqr* ts)
{
   const TermVar* tv = dynamic_cast<const TermVar* >(ts->child().get());
   if (tv == nullptr)
      return false;

   else
   {
      qt_->addSquare(plus_ ? tc->getVal() : -tc->getVal(), tv->var());
      return true;
   }
}

bool QuadraticTermCreator::makeProd(const TermVar* tv, const TermMul* tm)
{
   const TermConst* tcl = dynamic_cast<const TermConst*>(tm->left().get());
   const TermConst* tcr = dynamic_cast<const TermConst*>(tm->right().get());

   const TermVar* tvl = dynamic_cast<const TermVar*>(tm->left().get());
   const TermVar* tvr = dynamic_cast<const TermVar*>(tm->right().get());

   if (tcl != nullptr && tcr != nullptr)
   {
      Interval x = tcl->getVal() * tcr->getVal();
      qt_->addLin(plus_ ? x : -x, tv->var());
      
      return true;
   }
   else if (tcl != nullptr && tvr != nullptr)
   {
      qt_->addBilin(plus_ ? tcl->getVal() : -tcl->getVal(), tv->var(),
                    tvr->var());
      return true;
   }
   else if (tvl != nullptr && tcr != nullptr)
   {
      qt_->addBilin(plus_ ? tcr->getVal() : -tcr->getVal(), tv->var(),
                    tvl->var());      
      return true;
   }

   return false;
}

void QuadraticTermCreator::apply(const TermDiv* t)
{}

void QuadraticTermCreator::apply(const TermMin* t)
{}

void QuadraticTermCreator::apply(const TermMax* t)
{}

void QuadraticTermCreator::apply(const TermUsb* t)
{}

void QuadraticTermCreator::apply(const TermAbs* t)
{}

void QuadraticTermCreator::apply(const TermSgn* t)
{}

void QuadraticTermCreator::apply(const TermSqr* t)
{
   const TermVar* tv = dynamic_cast<const TermVar*>(t->child().get());
   if (tv == nullptr)
   {
      success_ = false;
   }
   else
   {
      qt_->addSquare(plus_ ? 1.0 : -1.0, tv->var());
      success_ = true;
   }
}

void QuadraticTermCreator::apply(const TermSqrt* t)
{}

void QuadraticTermCreator::apply(const TermPow* t)
{}

void QuadraticTermCreator::apply(const TermExp* t)
{}

void QuadraticTermCreator::apply(const TermLog* t)
{}

void QuadraticTermCreator::apply(const TermCos* t)
{}

void QuadraticTermCreator::apply(const TermSin* t)
{}

void QuadraticTermCreator::apply(const TermTan* t)
{}
   
void QuadraticTermCreator::apply(const TermLin* t)
{
   qt_->addConstant(t->getConstantValue());

   for (auto it=t->begin(); it!=t->end(); ++it)
      qt_->addLin(t->getCoefSub(it), t->getVarSub(it));

   success_ = true;
}

} // namespace
