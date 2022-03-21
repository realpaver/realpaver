///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, OpSymbol op)
{
   switch(op)
   {
      case OpSymbol::Add:  return os << "+";
      case OpSymbol::Sub:  return os << "-";
      case OpSymbol::Mul:  return os << "*";
      case OpSymbol::Div:  return os << "/";
      case OpSymbol::Min:  return os << "min";
      case OpSymbol::Max:  return os << "max";
      case OpSymbol::Usb: return os << "-";
      case OpSymbol::Abs:  return os << "abs";
      case OpSymbol::Sgn:  return os << "sgn";
      case OpSymbol::Sqr:  return os << "sqr";
      case OpSymbol::Sqrt: return os << "sqrt";
      case OpSymbol::Pow:  return os << "pow";
      case OpSymbol::Exp:  return os << "exp";
      case OpSymbol::Log:  return os << "log";
      case OpSymbol::Cos:  return os << "cos";
      case OpSymbol::Sin:  return os << "sin";
      case OpSymbol::Tan:  return os << "tan";
      default:             os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

TermRep::TermRep(OpPriority p)
      : hcode_(0),
        constant_(true),
        priority_(p)
{}

TermRep::~TermRep()
{}

bool TermRep::isZero() const
{
   return false;
}

bool TermRep::isOne() const
{
   return false;
}

bool TermRep::isMinusOne() const
{
   return false;
}

bool TermRep::isNumber() const
{
   return false;
}

bool TermRep::isAdd() const
{
   return false;
}

bool TermRep::isVar() const
{
   return false;
}

bool TermRep::isUsb() const
{
   return false;
}

bool TermRep::isSub() const
{
   return false;
}

bool TermRep::isMul() const
{
   return false;
}

bool TermRep::isDiv() const
{
   return false;
}

OpPriority TermRep::priority() const
{
   return priority_;
}

size_t TermRep::hashCode() const
{
   return hcode_;
}

bool TermRep::isConstant() const
{
   return constant_;
}

///////////////////////////////////////////////////////////////////////////////

bool Term::simplify_ = true;

bool Term::isSimplificationOn()
{
   return simplify_;
}

bool Term::isSimplificationOff()
{
   return !simplify_;
}

bool Term::simplification(bool simplify)
{
   bool status = simplify_;
   simplify_ = simplify;
   return status;
}
   
Term::Term(double a) : rep_(std::make_shared<TermConst>(a))
{}

Term::Term(const Interval& x) : rep_(std::make_shared<TermConst>(x))
{}

Term::Term(Variable v) : rep_(std::make_shared<TermVar>(v))
{}

Term::Term(const SharedRep& rep): rep_(rep)
{}

size_t Term::hashCode() const
{
   return rep_->hashCode();
}

void Term::print(std::ostream& os) const
{
   rep_->print(os);
}

Interval Term::evalConst() const
{
   return rep_->evalConst();
}

Interval Term::eval(const IntervalRegion& reg) const
{
   return rep_->eval(reg);
}

void Term::acceptVisitor(TermVisitor& vis) const
{
   rep_->acceptVisitor(vis);
}

bool Term::dependsOn(const Variable& v) const
{
   return rep_->dependsOn(v);
}

void Term::makeScope(Scope& s) const
{
   rep_->makeScope(s);
}

Term::SharedRep Term::rep() const
{
   return rep_;
}

bool Term::isZero() const
{
   return rep_->isZero();
}

bool Term::isOne() const
{
   return rep_->isOne();
}

bool Term::isMinusOne() const
{
   return rep_->isMinusOne();
}

bool Term::isNumber() const
{
   return rep_->isNumber();
}

bool Term::isConstant() const
{
   return rep_->isConstant();
}

bool Term::isLinear() const
{
   return rep_->isLinear();
}

bool Term::isVar() const
{
   return rep_->isVar();
}

bool Term::isUsb() const
{
   return rep_->isUsb();
}

bool Term::isAdd() const
{
   return rep_->isAdd();
}

bool Term::isSub() const
{
   return rep_->isSub();
}

bool Term::isMul() const
{
   return rep_->isMul();
}

bool Term::isDiv() const
{
   return rep_->isDiv();
}

TermRep* Term::cloneRoot() const
{
   return rep_->cloneRoot();
}

std::ostream& operator<<(std::ostream& os, Term t)
{
   t.print(os);
   return os;
}

Term& Term::operator+=(Term other)
{
   Term t( (*this) + other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator-=(Term other)
{
   Term t( (*this) - other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator*=(Term other)
{
   Term t( (*this) * other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator/=(Term other)
{
   Term t( (*this) / other );
   rep_ = t.rep_;
   return *this;
}

Term operator+(Term l, Term r)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermAdd>(l.rep(), r.rep()));

   if (l.isZero())
      return r;

   else if (r.isZero())
      return l;

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() + r.evalConst() );
      return Term(x);
   }

   else if (r.isNumber())
   {
      Interval x( r.evalConst() );
      if (x.isCertainlyLeZero())
      {
         Interval y( -x );
         return l - y;
      }

      else
         return Term(std::make_shared<TermAdd>(r.rep(), l.rep()));
   }

   else if (l.isAdd() && r.isAdd())
   {
      TermAdd* lt = static_cast<TermAdd*>(l.rep().get());
      TermAdd* rt = static_cast<TermAdd*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a + f) + (b + g) -> (a + b) + (f + g)
         Interval x( lt->left()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->right() + rt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a + f) + (g + b) -> (a + b) + (f + g)
         Interval x( lt->left()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->right() + rt->left() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f + a) + (b + g) -> (a + b) + (f + g)
         Interval x( lt->right()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->left() + rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f + a) + (g + b) -> (a + b) + (f + g)
         Interval x( lt->right()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->left() + rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermAdd>(l.rep(),r.rep()));
      }
   }

   else if (l.isAdd() && r.isSub())
   {
      TermAdd* lt = static_cast<TermAdd*>(l.rep().get());
      TermSub* rt = static_cast<TermSub*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a + f) + (b - g) -> (a + b) + (f - g)
         Interval x( lt->left()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->right() - rt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a + f) + (g - b) -> (a - b) + (f + g)
         Interval x( lt->left()->evalConst() - rt->right()->evalConst() );
         Term aux( lt->right() + rt->left() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f + a) + (b - g) -> (a + b) + (f - g)
         Interval x( lt->right()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->left() - rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f + a) + (g - b) -> (a - b) + (f + g)
         Interval x( lt->right()->evalConst() - rt->right()->evalConst() );
         Term aux( lt->left() + rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermAdd>(l.rep(),r.rep()));
      }
   }

   else if (l.isSub() && r.isAdd())
   {
      TermSub* lt = static_cast<TermSub*>(l.rep().get());
      TermAdd* rt = static_cast<TermAdd*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a - f) + (b + g) -> (a + b) + (g - f)
         Interval x( lt->left()->evalConst() + rt->left()->evalConst() );
         Term aux( rt->right() - lt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a - f) + (g + b) -> (a + b) + (g - f)
         Interval x( lt->left()->evalConst() + rt->right()->evalConst() );
         Term aux( rt->left() - lt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f - a) + (b + g) -> (b - a) + (f + g)
         Interval x( rt->left()->evalConst() - lt->right()->evalConst() );
         Term aux( lt->left() + rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f - a) + (g + b) -> (b - a) + (f + g)
         Interval x( rt->right()->evalConst() - lt->right()->evalConst() );
         Term aux( lt->left() + rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermAdd>(l.rep(),r.rep()));
      }
   }

   else if (l.isSub() && r.isSub())
   {
      TermSub* lt = static_cast<TermSub*>(l.rep().get());
      TermSub* rt = static_cast<TermSub*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a - f) + (b - g) -> (f + g) - (a + b)
         Interval x( lt->left()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->right() + rt->right() );
         return aux - x;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a - f) + (g - b) -> (a - b) + (g - f)
         Interval x( lt->left()->evalConst() - rt->right()->evalConst() );
         Term aux( rt->left() - lt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f - a) + (b - g) -> (b - a) + (f - g)
         Interval x( rt->left()->evalConst() - lt->right()->evalConst() );
         Term aux( lt->left() - rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f - a) + (g - b) -> -(a + b) + (f + g)
         Interval x( lt->right()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->left() + rt->left() );
         return (-x) + aux;
      }

      else
      {
         return Term(std::make_shared<TermAdd>(l.rep(), r.rep()));
      }
   }

   else
      return Term(std::make_shared<TermAdd>(l.rep(), r.rep()));
}

Term operator-(Term l, Term r)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermSub>(l.rep(), r.rep()));

   if (r.isZero())
      return l;

   else if (l.isZero())
      return (-r);

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() - r.evalConst() );
      return Term(x);
   }

   else if (r.isNumber())
   {
      Interval x( r.evalConst() );
      if (x.isCertainlyLeZero())
      {
         Interval y( -x );
         return l + y;
      }

      else
         return Term(std::make_shared<TermSub>(l.rep(), r.rep()));
   }

   else if (r.isUsb())
   {
      TermUsb* rt = static_cast<TermUsb*>(r.rep().get());
      return l + Term(rt->child());
   }

   else if (l.isAdd() && r.isAdd())
   {
      TermAdd* lt = static_cast<TermAdd*>(l.rep().get());
      TermAdd* rt = static_cast<TermAdd*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a + f) - (b + g) -> (a - b) + (f - g)
         Interval x( lt->left()->evalConst() - rt->left()->evalConst() );
         Term aux( lt->right() - rt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a + f) - (g + b) -> (a - b) + (f - g)
         Interval x( lt->left()->evalConst() - rt->right()->evalConst() );
         Term aux( lt->right() - rt->left() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f + a) - (b + g) -> (a - b) + (f - g)
         Interval x( lt->right()->evalConst() - rt->left()->evalConst() );
         Term aux( lt->left() - rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f + a) - (g + b) -> (a - b) + (f - g)
         Interval x( lt->right()->evalConst() - rt->right()->evalConst() );
         Term aux( lt->left() - rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
      }
   }

   else if (l.isAdd() && r.isSub())
   {
      TermAdd* lt = static_cast<TermAdd*>(l.rep().get());
      TermSub* rt = static_cast<TermSub*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a + f) - (b - g) -> (a - b) + (f + g)
         Interval x( lt->left()->evalConst() - rt->left()->evalConst() );
         Term aux( lt->right() + rt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a + f) - (g - b) -> (a + b) + (f - g)
         Interval x( lt->left()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->right() - rt->left() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f + a) - (b - g) -> (a - b) + (f + g)
         Interval x( lt->right()->evalConst() - rt->left()->evalConst() );
         Term aux( lt->left() + rt->right() );
         return x + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f + a) - (g - b) -> (a + b) + (f - g)
         Interval x( lt->right()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->left() - rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
      }
   }

   else if (l.isSub() && r.isAdd())
   {
      TermSub* lt = static_cast<TermSub*>(l.rep().get());
      TermAdd* rt = static_cast<TermAdd*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a - f) - (b + g) -> (a - b) - (f + g)
         Interval x( lt->left()->evalConst() - rt->left()->evalConst() );
         Term aux( lt->right() + rt->right() );
         return x - aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a - f) - (g + b) -> (a - b) - (f + g)
         Interval x( lt->left()->evalConst() - rt->right()->evalConst() );
         Term aux( lt->right() + rt->left() );
         return x - aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f - a) - (b + g) -> -(a + b) + (f - g)
         Interval x( lt->right()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->left() - rt->right() );
         return (-x) + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f - a) - (g + b) -> -(a + b) + (f - g)
         Interval x( lt->right()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->left() - rt->left() );
         return (-x) + aux;
      }

      else
      {
         return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
      }
   }

   else if (l.isSub() && r.isSub())
   {
      TermSub* lt = static_cast<TermSub*>(l.rep().get());
      TermSub* rt = static_cast<TermSub*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a - f) - (b - g) -> (a - b) + (g - f)
         Interval x( lt->left()->evalConst() - rt->left()->evalConst() );
         Term aux( rt->right() - lt->right() );
         return x + aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a - f) - (g - b) -> (a + b) - (f + g)
         Interval x( lt->left()->evalConst() + rt->right()->evalConst() );
         Term aux( lt->right() + rt->left() );
         return x - aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f - a) - (b - g) -> -(a + b) + (f + g)
         Interval x( lt->right()->evalConst() + rt->left()->evalConst() );
         Term aux( lt->left() + rt->right() );
         return (-x) + aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f - a) - (g - b) -> (b - a) + (f - g)
         Interval x( rt->right()->evalConst() - lt->right()->evalConst() );
         Term aux( lt->left() - rt->left() );
         return x + aux;
      }

      else
      {
         return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
      }
   }

   else
      return Term(std::make_shared<TermSub>(l.rep(), r.rep()));
}

Term operator*(Term l, Term r)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermMul>(l.rep(), r.rep()));

   if (l.isZero())
      return l;

   else if (r.isZero())
      return r;

   else if (l.isOne())
      return r;

   else if (r.isOne())
      return l;

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() * r.evalConst() );
      return Term(x);
   }

   else if (l.isMinusOne())
      return Term(std::make_shared<TermUsb>(r.rep()));

   else if (r.isMinusOne())
      return Term(std::make_shared<TermUsb>(l.rep()));

   else if (r.isConstant())
      return Term(std::make_shared<TermMul>(r.rep(),l.rep()));

   else if (l.isMul() && r.isMul())
   {
      TermMul* lt = static_cast<TermMul*>(l.rep().get());
      TermMul* rt = static_cast<TermMul*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a * f) * (b * g) -> (a * b) * f * g
         Interval x( lt->left()->evalConst() * rt->left()->evalConst() );
         return x * lt->right() * rt->right();
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a * f) * (g * b) -> (a * b) * f * g
         Interval x( lt->left()->evalConst() * rt->right()->evalConst() );
         return x * lt->right() * rt->left();
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f * a) * (b * g) -> (a * b) * f * g
         Interval x( lt->right()->evalConst() * rt->left()->evalConst() );
         return x * lt->left() * rt->right();
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f * a) * (g * b) -> (a * b) * f * g
         Interval x( lt->right()->evalConst() * rt->right()->evalConst() );
         return x * lt->left() * rt->left();
      }

      else
      {
         return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
      }
   }

   else if (l.isMul() && r.isDiv())
   {
      TermMul* lt = static_cast<TermMul*>(l.rep().get());
      TermDiv* rt = static_cast<TermDiv*>(r.rep().get());


      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a * f) * (b / g) -> (a * b) * (f / g)
         Interval x( lt->left()->evalConst() * rt->left()->evalConst() );
         Term aux = lt->right() / rt->right();
         return x * aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a * f) * (g / b) -> (a / b) * f * g
         Interval x( lt->left()->evalConst() / rt->right()->evalConst() );
         return x * lt->right() * rt->left();
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f * a) * (b / g) -> (a * b) * (f / g)
         Interval x( lt->right()->evalConst() * rt->left()->evalConst() );
         Term aux = lt->left() / rt->right();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f * a) * (g / b) -> (a / b) * f * g
         Interval x( lt->right()->evalConst() / rt->right()->evalConst() );
         return x * lt->left() * rt->left();
      }

      else
      {
         return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
      }
   }

   else if (l.isDiv() && r.isMul())
   {
      TermDiv* lt = static_cast<TermDiv*>(l.rep().get());
      TermMul* rt = static_cast<TermMul*>(r.rep().get());


      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a / f) * (b * g) -> (a * b) * (g / f)
         Interval x( lt->left()->evalConst() * rt->left()->evalConst() );
         Term aux = rt->right() / lt->right();
         return x * aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a / f) * (g * b) -> (a * b) * (g / f)
         Interval x( lt->left()->evalConst() * rt->right()->evalConst() );
         Term aux = rt->left() / lt->right();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f / a) * (b * g) -> (b / a) * f * g
         Interval x( rt->left()->evalConst() / lt->right()->evalConst() );
         return x * lt->left() * rt->right();
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f / a) * (g * b) -> (b / a) * f  * g
         Interval x( rt->right()->evalConst() / lt->right()->evalConst() );
         return x * lt->left() * rt->left();
      }

      else
      {
         return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
      }
   }

   else if (l.isDiv() && r.isDiv())
   {
      TermDiv* lt = static_cast<TermDiv*>(l.rep().get());
      TermDiv* rt = static_cast<TermDiv*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a / f) * (b / g) -> (a * b) / (f * g)
         Interval x( lt->left()->evalConst() * rt->left()->evalConst() );
         Term aux = lt->right() * rt->right();
         return x / aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a / f) * (g / b) -> (a / b) * (g / f)
         Interval x( lt->left()->evalConst() / rt->right()->evalConst() );
         Term aux = rt->left() / lt->right();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f / a) * (b / g) -> (b / a) * (f / g)
         Interval x( rt->left()->evalConst() / lt->right()->evalConst() );
         Term aux = lt->left() / rt->right();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f / a) * (g / b) -> 1/(a * b) * f * g
         Interval x( lt->right()->evalConst() * rt->right()->evalConst() );
         return (1/x) * lt->left() * rt->left();
      }

      else
      {
         return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
      }
   }

   else
      return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
}

Term operator/(Term l, Term r)
{
   ASSERT(!r.isZero(), "Term divided by zero");

   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermDiv>(l.rep(), r.rep()));

   if (l.isZero() || r.isOne())
      return l;

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() / r.evalConst() );
      return Term(x);
   }

   else if (r.isMinusOne())
      return Term(std::make_shared<TermUsb>(l.rep()));

   else if (l.isMul() && r.isMul())
   {
      TermMul* lt = static_cast<TermMul*>(l.rep().get());
      TermMul* rt = static_cast<TermMul*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a * f) / (b * g) -> (a / b) * (f / g)
         Interval x( lt->left()->evalConst() / rt->left()->evalConst() );
         Term aux = lt->right() / rt->right();
         return x * aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a * f) / (g * b) -> (a / b) * (f / g)
         Interval x( lt->left()->evalConst() / rt->right()->evalConst() );
         Term aux = lt->right() / rt->left();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f * a) / (b * g) -> (a / b) * (f / g)
         Interval x( lt->right()->evalConst() / rt->left()->evalConst() );
         Term aux = lt->left() / rt->right();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f * a) / (g * b) -> (a / b) * (f / g)
         Interval x( lt->right()->evalConst() / rt->right()->evalConst() );
         Term aux = lt->left() / rt->left();
         return x * aux;
      }

      else
      {
         return Term(std::make_shared<TermDiv>(l.rep(),r.rep()));
      }
   }

   else if (l.isMul() && r.isDiv())
   {
      TermMul* lt = static_cast<TermMul*>(l.rep().get());
      TermDiv* rt = static_cast<TermDiv*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a * f) / (b / g) -> (a / b) * (f * g)
         Interval x( lt->left()->evalConst() / rt->left()->evalConst() );
         return x * lt->right() * rt->right();
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a * f) / (g / b) -> (a * b) * (f / g)
         Interval x( lt->left()->evalConst() / rt->right()->evalConst() );
         Term aux = lt->right() / rt->left();
         return x * aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f * a) / (b / g) -> (a / b) * (f * g)
         Interval x( lt->right()->evalConst() / rt->left()->evalConst() );
         return x * lt->left() * rt->right();
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f * a) / (g / b) -> (a * b) * (f / g)
         Interval x( lt->right()->evalConst() * rt->right()->evalConst() );
         Term aux = lt->left() / rt->left();
         return x * aux;
      }

      else
      {
         return Term(std::make_shared<TermDiv>(l.rep(),r.rep()));
      }
   }

   else if (l.isDiv() && r.isMul())
   {
      TermDiv* lt = static_cast<TermDiv*>(l.rep().get());
      TermMul* rt = static_cast<TermMul*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a / f) / (b * g) -> (a / b) / (f * g)
         Interval x( lt->left()->evalConst() / rt->left()->evalConst() );
         Term aux = lt->right() * rt->right();
         return x / aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a / f) / (g * b) -> (a / b) / (f * g)
         Interval x( lt->left()->evalConst() / rt->right()->evalConst() );
         Term aux = lt->right() * rt->left();
         return x / aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f / a) / (b * g) -> 1/(a * b) * (f / g)
         Interval x( lt->right()->evalConst() * rt->left()->evalConst() );
         Term aux = lt->left() / rt->right();
         return (1/x) * aux;
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f / a) / (g * b) -> 1/(a * b) * (f / g)
         Interval x( lt->right()->evalConst() * rt->right()->evalConst() );
         Term aux = lt->left() / rt->left();
         return (1/x) * aux;
      }

      else
      {
         return Term(std::make_shared<TermDiv>(l.rep(),r.rep()));
      }
   }

   else if (l.isDiv() && r.isDiv())
   {
      TermDiv* lt = static_cast<TermDiv*>(l.rep().get());
      TermDiv* rt = static_cast<TermDiv*>(r.rep().get());

      if (lt->left()->isConstant() && rt->left()->isConstant())
      {
         // (a / f) / (b / g) -> (a / f) * (g / b) -> (a / b) * (g / f)
         Interval x( lt->left()->evalConst() / rt->left()->evalConst() );
         Term aux = rt->right() / lt->right();
         return x * aux;
      }

      else if (lt->left()->isConstant() && rt->right()->isConstant())
      {
         // (a / f) / (g / b) -> (a / f) * (b / g) -> (a * b) / (f * g)
         Interval x( lt->left()->evalConst() * rt->right()->evalConst() );
         Term aux = lt->right() * rt->left();
         return x / aux;
      }

      else if (lt->right()->isConstant() && rt->left()->isConstant())
      {
         // (f / a) / (b / g) -> (f / a) * (g / b) -> 1/(a * b) * f * g
         Interval x( lt->right()->evalConst() * rt->left()->evalConst() );
         return (1/x) * lt->left() * rt->right();
      }

      else if (lt->right()->isConstant() && rt->right()->isConstant())
      {
         // (f / a) / (g / b) -> (f / a) * (b / g) -> (b / a) * (f / g)
         Interval x( rt->right()->evalConst() / lt->right()->evalConst() );
         Term aux = lt->left() / rt->left();
         return x * aux;
      }

      else
      {
         return Term(std::make_shared<TermDiv>(l.rep(), r.rep()));
      }
   }

   else
      return Term(std::make_shared<TermDiv>(l.rep(), r.rep()));
}

Term MIN(Term l, Term r)
{
   if (Term::isSimplificationOff())
      return  Term(std::make_shared<TermMin>(l.rep(), r.rep()));

   if (l.isConstant() && r.isConstant())
   {
      Interval x( min(l.evalConst(), r.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermMin>(l.rep(), r.rep()));
}

Term MAX(Term l, Term r)
{
   if (Term::isSimplificationOff())
      return  Term(std::make_shared<TermMax>(l.rep(), r.rep()));

   if (l.isConstant() && r.isConstant())
   {
      Interval x( max(l.evalConst(), r.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermMax>(l.rep(),r.rep()));
}

Term operator-(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermUsb>(t.rep()));

   if (t.isZero())
      return t;

   else if (t.isConstant())
   {
      Interval x( -t.evalConst() );
      return Term(x);
   }

   else if (t.isUsb())
   {
      TermUsb* ut = static_cast<TermUsb*>(t.rep().get());
      return Term(ut->child());
   }

   else
      return Term(std::make_shared<TermUsb>(t.rep()));
}

Term abs(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermAbs>(t.rep()));

   if (t.isConstant())
   {
      Interval x( abs(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermAbs>(t.rep()));
}

Term sgn(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermSgn>(t.rep()));

   if (t.isConstant())
   {
      Interval x( sgn(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSgn>(t.rep()));
}

Term sqr(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermSqr>(t.rep()));

   if (t.isConstant())
   {
      Interval x( sqr(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSqr>(t.rep()));
}

Term sqrt(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermSqrt>(t.rep()));

   if (t.isConstant())
   {
      Interval x( sqrt(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSqrt>(t.rep()));
}

Term pow(Term t, int n)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermPow>(t.rep(), n));

   if (t.isConstant())
   {
      Interval x( pow(t.evalConst(),n) );
      return Term(x);
   }
   else
   {
      switch(n)
      {
         case -2: return Term(1.0) / sqr(t);
         case -1: return Term(1.0) / t;
         case 0:  return Term(1.0);
         case 1:  return t;
         case 2:  return sqr(t);
      }

      if (n < 0)
         return pow(t,double(n));

      else
         return Term(std::make_shared<TermPow>(t.rep(), n));
   }
}

Term pow(Term t, double d)
{
   if (Term::isSimplificationOff())
      return exp(d*log(t));

   if (d == 0.0)
      return Term(1.0);

   else if (d == 1.0)
      return t;

   else if (d == 0.5)
      return sqrt(t);

   else if (d == -0.5)
      return Term(1.0) / sqrt(t);

   else if (t.isConstant())
   {
      Interval x( exp(d*log(t.evalConst())) );
      return Term(x);
   }

   else
   {
      int n = int(d);

      if (n == d && n > 0)
         return pow(t,n);

      else
         return exp(d*log(t));
   }
}

Term pow(Term t, const Interval& x)
{
   if (Term::isSimplificationOff())
      return exp(x*log(t));

   if (x.isSingleton())
      return pow(t, x.left());

   else
      return exp(x*log(t));
}

Term exp(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermExp>(t.rep()));

   if (t.isConstant())
   {
      Interval x( exp(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermExp>(t.rep()));
}

Term log(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermLog>(t.rep()));

   if (t.isConstant())
   {
      Interval x( log(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermLog>(t.rep()));
}

Term cos(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermCos>(t.rep()));

   if (t.isConstant())
   {
      Interval x( cos(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermCos>(t.rep()));
}

Term sin(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermSin>(t.rep()));

   if (t.isConstant())
   {
      Interval x( sin(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSin>(t.rep()));
}

Term tan(Term t)
{
   if (Term::isSimplificationOff())
      return Term(std::make_shared<TermTan>(t.rep()));

   if (t.isConstant())
   {
      Interval x( tan(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermTan>(t.rep()));
}

///////////////////////////////////////////////////////////////////////////////

TermConst::TermConst(const Interval& x)
      : TermRep(OpPriority::Low),
        x_(x)
{
   ASSERT(!x.isEmpty(), "Bad constant term " << x);

   hcode_ = x.hashCode();
   constant_ = true;
}

Interval TermConst::getVal() const
{
   return x_;
}

Interval TermConst::evalConst() const
{
   return x_;
}

Interval TermConst::eval(const IntervalRegion& reg) const
{
   return x_;
}

void TermConst::print(std::ostream& os) const
{
   os << x_;
}

void TermConst::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermConst::isZero() const
{
   return x_.isZero();
}

bool TermConst::isOne() const
{
   return (x_.left() == 1.0) && (x_.right() == 1.0);
}

bool TermConst::isMinusOne() const
{
   return (x_.left() == -1.0) && (x_.right() == -1.0);
}

bool TermConst::dependsOn(const Variable& v) const
{
   return false;
}

bool TermConst::isLinear() const
{
   return true;
}

void TermConst::makeScope(Scope& s) const
{}

TermRep* TermConst::cloneRoot() const
{
   return new TermConst(x_);
}

///////////////////////////////////////////////////////////////////////////////

TermVar::TermVar(Variable v)
      : TermRep(OpPriority::Low),
        v_(v)
{
   hcode_ = v.hashCode();
   constant_ = false;
}

Variable TermVar::var() const
{
   return v_;
}

Interval TermVar::evalConst() const
{
   THROW("Evaluation of a non constant term");
   return Interval::universe();
}

Interval TermVar::eval(const IntervalRegion& reg) const
{
   return reg.get(v_);
}

void TermVar::print(std::ostream& os) const
{
   os << v_.getName();
}

void TermVar::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermVar::dependsOn(const Variable& v) const
{
   return v_.getId() == v.getId();
}

bool TermVar::isLinear() const
{
   return true;
}

void TermVar::makeScope(Scope& s) const
{
   s.insert(v_);
}

bool TermVar::isVar() const
{
   return true;
}

TermRep* TermVar::cloneRoot() const
{
   return new TermVar(v_);
}

///////////////////////////////////////////////////////////////////////////////

TermOp::TermOp(const SharedRep& t, OpSymbol op, OpPriority p)
      : TermRep(p),
        v_(),
        op_(op)
{
   constant_ = true;
   insert(t);
}

TermOp::TermOp(const SharedRep& l, const SharedRep& r, OpSymbol op,
               OpPriority p)
      : TermRep(p),
        v_(),
        op_(op)
{
   constant_ = true;
   insert(l);
   insert(r);
}

TermOp::~TermOp()
{}

void TermOp::insert(const SharedRep& t)
{
   v_.push_back(t);

   hcode_ = static_cast<size_t>(opSymbol());
   for (auto sub : v_)
      hcode_ = hash2(sub->hashCode(), hcode_);

   if (!t->isConstant())
      constant_ = false;
}

void TermOp::print(std::ostream& os) const
{
   // default implementation in prefix operation
   os << opSymbol() << "(";
   bool first = false;
   for (auto sub: v_)
   {
      if (first) os << ",";
      sub->print(os);
      first = true;
   }
   os << ")";
}

bool TermOp::dependsOn(const Variable& v) const
{
   for (auto sub : v_)
      if (sub->dependsOn(v))
         return true;

   return false;
}

bool TermOp::isLinear() const
{
   return isConstant();
}

void TermOp::makeScope(Scope& s) const
{
   for (auto sub : v_)
      sub->makeScope(s);
}

size_t TermOp::arity() const
{
   return v_.size();
}

OpSymbol TermOp::opSymbol() const
{
   return op_;
}

TermRep::SharedRep TermOp::subTerm(size_t i) const
{
   ASSERT(i < arity(), "Access out of range to a term operand");

   return v_[i];
}

TermRep::SharedRep TermOp::left() const
{
   ASSERT(arity() == 2, "Access out of range to a term operand");

   return v_[0];
}

TermRep::SharedRep TermOp::right() const
{
   ASSERT(arity() == 2, "Access out of range to a term operand");

   return v_[1];
}

TermRep::SharedRep TermOp::child() const
{
   ASSERT(arity() == 1, "Access out of range to a term operand");

   return v_[0];
}

///////////////////////////////////////////////////////////////////////////////

TermAdd::TermAdd(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Add, OpPriority::AddSub)
{}

Interval TermAdd::evalConst() const
{
   return left()->evalConst() + right()->evalConst();
}

Interval TermAdd::eval(const IntervalRegion& reg) const
{
   return left()->eval(reg) + right()->eval(reg);
}

void TermAdd::print(std::ostream& os) const
{
   left()->print(os);
   os << opSymbol();
   right()->print(os);
}

void TermAdd::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermAdd::isLinear() const
{
   return left()->isLinear() && right()->isLinear();
}

bool TermAdd::isAdd() const
{
   return true;
}

TermRep* TermAdd::cloneRoot() const
{
   return new TermAdd(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

TermSub::TermSub(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Sub, OpPriority::AddSub)
{}

Interval TermSub::evalConst() const
{
   return left()->evalConst() - right()->evalConst();
}

Interval TermSub::eval(const IntervalRegion& reg) const
{
   return left()->eval(reg) - right()->eval(reg);
}

void TermSub::print(std::ostream& os) const
{
   OpPriority p = priority(),
              rp = right()->priority();

   os << "(";
   left()->print(os);
   os << ")";
   os << opSymbol();
   
   if (rp <= p && (!right()->isVar()))
   {
      os << "(";
      right()->print(os);
      os << ")";
   }
   else
      right()->print(os);
}

void TermSub::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermSub::isLinear() const
{
   return left()->isLinear() && right()->isLinear();
}

bool TermSub::isSub() const
{
   return true;
}

TermRep* TermSub::cloneRoot() const
{
   return new TermSub(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

TermMul::TermMul(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Mul, OpPriority::MulDiv)
{}

Interval TermMul::evalConst() const
{
   return left()->evalConst() * right()->evalConst();
}

Interval TermMul::eval(const IntervalRegion& reg) const
{
   return left()->eval(reg) * right()->eval(reg);
}

void TermMul::print(std::ostream& os) const
{
   OpPriority p = priority(),
            lp = left()->priority(),
            rp = right()->priority();

   if (lp < p && lp > OpPriority::Low)
   {
      os << "(";
      left()->print(os);
      os << ")";
   }
   else
      left()->print(os);      

   os << opSymbol();

   if (rp < p && rp > OpPriority::Low)
   {
      os << "(";
      right()->print(os);
      os << ")";
   }
   else
      right()->print(os);
}

void TermMul::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermMul::isLinear() const
{
   return left()->isConstant() || right()->isConstant();
}

bool TermMul::isMul() const
{
   return true;
}

TermRep* TermMul::cloneRoot() const
{
   return new TermMul(left(), right());
}

///////////////////////////////////////////////////////////////////////////////
TermDiv::TermDiv(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Div, OpPriority::MulDiv)
{}

Interval TermDiv::evalConst() const
{
   return left()->evalConst() / right()->evalConst();
}

Interval TermDiv::eval(const IntervalRegion& reg) const
{
   return left()->eval(reg) / right()->eval(reg);
}

void TermDiv::print(std::ostream& os) const
{
   OpPriority lp = left()->priority(),
            rp = right()->priority();

   if (lp == OpPriority::AddSub)
   {
      os << "(";
      left()->print(os);
      os << ")";
   }
   else
      left()->print(os);

   os << opSymbol();
   
   if (rp == OpPriority::AddSub || rp == OpPriority::MulDiv)
   {
      os << "(";
      right()->print(os);
      os << ")";
   }
   else
      right()->print(os);
}

void TermDiv::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermDiv::isDiv() const
{
   return true;
}

TermRep* TermDiv::cloneRoot() const
{
   return new TermDiv(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

TermMin::TermMin(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Min, OpPriority::High)
{}

Interval TermMin::evalConst() const
{
   return min(left()->evalConst(), right()->evalConst());
}

Interval TermMin::eval(const IntervalRegion& reg) const
{
   return min(left()->eval(reg), right()->eval(reg));
}

void TermMin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermMin::cloneRoot() const
{
   return new TermMin(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

TermMax::TermMax(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, OpSymbol::Max, OpPriority::High)
{}

Interval TermMax::evalConst() const
{
   return max(left()->evalConst(), right()->evalConst());
}

Interval TermMax::eval(const IntervalRegion& reg) const
{
   return max(left()->eval(reg), right()->eval(reg));
}

void TermMax::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermMax::cloneRoot() const
{
   return new TermMax(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

TermUsb::TermUsb(const SharedRep& t)
      : TermOp(t, OpSymbol::Usb, OpPriority::AddSub)
{}

Interval TermUsb::evalConst() const
{
   return -child()->evalConst();
}

Interval TermUsb::eval(const IntervalRegion& reg) const
{
   return -child()->eval(reg);
}

void TermUsb::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermUsb::isLinear() const
{
   return child()->isLinear();
}

bool TermUsb::isUsb() const
{
   return true;
}

TermRep* TermUsb::cloneRoot() const
{
   return new TermUsb(child());
}

///////////////////////////////////////////////////////////////////////////////

TermAbs::TermAbs(const SharedRep& t)
      : TermOp(t, OpSymbol::Abs, OpPriority::High)
{}

Interval TermAbs::evalConst() const
{
   return abs(child()->evalConst());
}

Interval TermAbs::eval(const IntervalRegion& reg) const
{
   return abs(child()->eval(reg));
}

void TermAbs::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermAbs::cloneRoot() const
{
   return new TermAbs(child());
}

///////////////////////////////////////////////////////////////////////////////

TermSgn::TermSgn(const SharedRep& t)
      : TermOp(t, OpSymbol::Sgn, OpPriority::High)
{}

Interval TermSgn::evalConst() const
{
   return sgn(child()->evalConst());
}

Interval TermSgn::eval(const IntervalRegion& reg) const
{
   return sgn(child()->eval(reg));
}

void TermSgn::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermSgn::cloneRoot() const
{
   return new TermSgn(child());
}

///////////////////////////////////////////////////////////////////////////////

TermSqr::TermSqr(const SharedRep& t)
      : TermOp(t, OpSymbol::Sqr, OpPriority::High)
{}

Interval TermSqr::evalConst() const
{
   return sqr(child()->evalConst());
}

Interval TermSqr::eval(const IntervalRegion& reg) const
{
   return sqr(child()->eval(reg));
}

void TermSqr::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermSqr::cloneRoot() const
{
   return new TermSqr(child());
}

///////////////////////////////////////////////////////////////////////////////

TermSqrt::TermSqrt(const SharedRep& t)
      : TermOp(t, OpSymbol::Sqrt, OpPriority::High)
{}

Interval TermSqrt::evalConst() const
{
   return sqrt(child()->evalConst());
}

Interval TermSqrt::eval(const IntervalRegion& reg) const
{
   return sqrt(child()->eval(reg));
}

void TermSqrt::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermSqrt::cloneRoot() const
{
   return new TermSqrt(child());
}

///////////////////////////////////////////////////////////////////////////////

TermPow::TermPow(const SharedRep& t, int n)
      : TermOp(t, OpSymbol::Pow, OpPriority::High),
        n_(n)
{
   ASSERT(n > 0, "Bad integral exponent in a term " << n);

   size_t h = hash1<int>(n);
   hcode_ = hash2(h, hcode_);
}

int TermPow::exponent() const
{
   return n_;
}

Interval TermPow::evalConst() const
{
   return pow(child()->evalConst(), n_);
}

Interval TermPow::eval(const IntervalRegion& reg) const
{
   return pow(child()->eval(reg), n_);
}

void TermPow::print(std::ostream& os) const
{
   os << opSymbol() << "(";
   child()->print(os);
   os << "," << n_ << ")";
}

void TermPow::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermPow::cloneRoot() const
{
   return new TermPow(child(), n_);
}

///////////////////////////////////////////////////////////////////////////////

TermExp::TermExp(const SharedRep& t)
      : TermOp(t, OpSymbol::Exp, OpPriority::High)
{}

Interval TermExp::evalConst() const
{
   return exp(child()->evalConst());
}

Interval TermExp::eval(const IntervalRegion& reg) const
{
   return exp(child()->eval(reg));
}

void TermExp::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermExp::cloneRoot() const
{
   return new TermExp(child());
}

///////////////////////////////////////////////////////////////////////////////

TermLog::TermLog(const SharedRep& t)
      : TermOp(t, OpSymbol::Log, OpPriority::High)
{}

Interval TermLog::evalConst() const
{
   return log(child()->evalConst());
}

Interval TermLog::eval(const IntervalRegion& reg) const
{
   return log(child()->eval(reg));
}

void TermLog::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermLog::cloneRoot() const
{
   return new TermLog(child());
}

///////////////////////////////////////////////////////////////////////////////

TermCos::TermCos(const SharedRep& t)
      : TermOp(t, OpSymbol::Cos, OpPriority::High)
{}

Interval TermCos::evalConst() const
{
   return cos(child()->evalConst());
}

Interval TermCos::eval(const IntervalRegion& reg) const
{
   return cos(child()->eval(reg));
}

void TermCos::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermCos::cloneRoot() const
{
   return new TermCos(child());
}

///////////////////////////////////////////////////////////////////////////////

TermSin::TermSin(const SharedRep& t)
      : TermOp(t, OpSymbol::Sin, OpPriority::High)
{}

Interval TermSin::evalConst() const
{
   return sin(child()->evalConst());
}

Interval TermSin::eval(const IntervalRegion& reg) const
{
   return sin(child()->eval(reg));
}

void TermSin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermSin::cloneRoot() const
{
   return new TermSin(child());
}

///////////////////////////////////////////////////////////////////////////////

TermTan::TermTan(const SharedRep& t)
      : TermOp(t, OpSymbol::Tan, OpPriority::High)
{}

Interval TermTan::evalConst() const
{
   return tan(child()->evalConst());
}

Interval TermTan::eval(const IntervalRegion& reg) const
{
   return tan(child()->eval(reg));
}

void TermTan::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermTan::cloneRoot() const
{
   return new TermTan(child());
}

///////////////////////////////////////////////////////////////////////////////

TermVisitor::~TermVisitor()
{}

void TermVisitor::apply(const TermConst* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermVar* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermAdd* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSub* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermMul* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermDiv* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermMin* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermMax* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermUsb* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermAbs* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSgn* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSqr* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSqrt* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermPow* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermExp* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermLog* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermCos* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSin* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermTan* t)
{
   THROW("Visit method not implemented");
}

} // namespace
