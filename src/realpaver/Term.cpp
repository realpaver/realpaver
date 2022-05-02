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

Interval TermRep::ival() const
{
   return ival_;
}

void TermRep::setIval(const Interval& x)
{
   ival_ = x;
}

///////////////////////////////////////////////////////////////////////////////

bool Term::simplify_ = true;
bool Term::idisplay_ = false;

bool Term::simplification()
{
   return simplify_;
}

bool Term::simplification(bool simplify)
{
   bool status = simplify_;
   simplify_ = simplify;
   return status;
}

bool Term::idisplay()
{
   return idisplay_;
}

bool Term::idisplay(bool ok)
{
   bool status = idisplay_;
   idisplay_ = ok;
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
   rep_->eval(reg);
   return rep_->ival_;
}

Interval Term::hc4ReviseForward(const IntervalRegion& reg) const
{
   return eval(reg);
}

Proof Term::hc4ReviseBackward(IntervalRegion& reg, const Interval& img)
{
   if (rep_->ival_.isEmpty())       return Proof::Empty;
   if (img.isDisjoint(rep_->ival_)) return Proof::Empty;
   if (img.contains(rep_->ival_))   return Proof::Inner;

   rep_->ival_ &= img;

   return rep_->contract(reg);   
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

bool Term::isSumOfSquares() const
{
   SumOfSquaresCreator creator;
   rep_->acceptVisitor(creator);
   return creator.sumOfSquaresVisited();
}

TermRep* Term::cloneRoot() const
{
   return rep_->cloneRoot();
}

Proof Term::contract(IntervalRegion& reg, const Interval& img)
{
   hc4ReviseForward(reg);
   return hc4ReviseBackward(reg, img);
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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

   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
      return exp(x*log(t));

   if (x.isSingleton())
      return pow(t, x.left());

   else
      return exp(x*log(t));
}

Term exp(Term t)
{
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   if (!Term::simplification())
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
   ival_ = x_;
}

Interval TermConst::getVal() const
{
   return x_;
}

Interval TermConst::evalConst() const
{
   return x_;
}

void TermConst::eval(const IntervalRegion& reg)
{
   ival_ = x_;
}

Proof TermConst::contract(IntervalRegion& reg)
{
   return Proof::Maybe;
}

void TermConst::print(std::ostream& os) const
{
   if (Term::idisplay())
      os << x_;
   else
      os << x_.midpoint();
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

void TermVar::eval(const IntervalRegion& reg)
{
   ival_ = reg.get(v_);
}

Proof TermVar::contract(IntervalRegion& reg)
{
   reg.set(v_, ival_);
   return ival_.isEmpty() ? Proof::Empty : Proof::Maybe;
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
   return v_.id() == v.id();
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
   for (auto sub : v_)
   {
      if (first) os << ",";
      sub->print(os);
      first = true;
   }
   os << ")";
}

void TermOp::eval(const IntervalRegion& reg)
{
   for (auto sub : v_) sub->eval(reg);

   evalRoot();
}

Proof TermOp::contract(IntervalRegion& reg)
{
   if (ival_.isEmpty()) return Proof::Empty;

   contractRoot();

   for (auto sub : v_)
      if (sub->contract(reg) == Proof::Empty) return Proof::Empty;

   return Proof::Maybe;
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

void TermAdd::evalRoot()
{
   ival_ = left()->ival() + right()->ival();
}

void TermAdd::contractRoot()
{
   left()->setIval(addPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(addPY(left()->ival(), right()->ival(), ival_));
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

void TermSub::evalRoot()
{
   ival_ = left()->ival() - right()->ival();
}

void TermSub::contractRoot()
{
   left()->setIval(subPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(subPY(left()->ival(), right()->ival(), ival_));
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

void TermMul::evalRoot()
{
   ival_ = left()->ival() * right()->ival();
}

void TermMul::contractRoot()
{
   left()->setIval(mulPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(mulPY(left()->ival(), right()->ival(), ival_));
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

void TermDiv::evalRoot()
{
   ival_ = left()->ival() / right()->ival();
}

void TermDiv::contractRoot()
{
   left()->setIval(divPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(divPY(left()->ival(), right()->ival(), ival_));
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

void TermMin::evalRoot()
{
   ival_ = min(left()->ival(), right()->ival());
}

void TermMin::contractRoot()
{
   left()->setIval(minPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(minPY(left()->ival(), right()->ival(), ival_));
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

void TermMax::evalRoot()
{
   ival_ = max(left()->ival(), right()->ival());
}

void TermMax::contractRoot()
{
   left()->setIval(maxPX(left()->ival(), right()->ival(), ival_));
   right()->setIval(maxPY(left()->ival(), right()->ival(), ival_));
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

void TermUsb::evalRoot()
{
   ival_ = -child()->ival();
}

void TermUsb::contractRoot()
{
   child()->setIval(usubPX(child()->ival(), ival_));
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

void TermAbs::evalRoot()
{
   ival_ = abs(child()->ival());
}

void TermAbs::contractRoot()
{
   child()->setIval(absPX(child()->ival(), ival_));
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

void TermSgn::evalRoot()
{
   ival_ = sgn(child()->ival());
}

void TermSgn::contractRoot()
{
   child()->setIval(sgnPX(child()->ival(), ival_));
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

void TermSqr::evalRoot()
{
   ival_ = sqr(child()->ival());
}

void TermSqr::print(std::ostream& os) const
{
   os << "(";
   child()->print(os);
   os << ")^2";
}

void TermSqr::contractRoot()
{
   child()->setIval(sqrPX(child()->ival(), ival_));
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

void TermSqrt::evalRoot()
{
   ival_ = sqrt(child()->ival());
}

void TermSqrt::contractRoot()
{
   child()->setIval(sqrtPX(child()->ival(), ival_));
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

void TermPow::evalRoot()
{
   ival_ = pow(child()->ival(), n_);
}

void TermPow::contractRoot()
{
   child()->setIval(powPX(child()->ival(), n_, ival_));
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

void TermExp::evalRoot()
{
   ival_ = exp(child()->ival());
}

void TermExp::contractRoot()
{
   child()->setIval(expPX(child()->ival(), ival_));
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

void TermLog::evalRoot()
{
   ival_ = log(child()->ival());
}

void TermLog::contractRoot()
{
   child()->setIval(logPX(child()->ival(), ival_));
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

void TermCos::evalRoot()
{
   ival_ = cos(child()->ival());
}

void TermCos::contractRoot()
{
   child()->setIval(cosPX(child()->ival(), ival_));
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

void TermSin::evalRoot()
{
   ival_ = sin(child()->ival());
}

void TermSin::contractRoot()
{
   child()->setIval(sinPX(child()->ival(), ival_));
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

void TermTan::evalRoot()
{
   ival_ = tan(child()->ival());
}

void TermTan::contractRoot()
{
   child()->setIval(tanPX(child()->ival(), ival_));
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

///////////////////////////////////////////////////////////////////////////////

SumOfSquaresCreator::SumOfSquaresCreator()
      : sos_(false),
        v_()
{}

bool SumOfSquaresCreator::sumOfSquaresVisited() const
{
   return sos_;
}

size_t SumOfSquaresCreator::nbSquares() const
{
   return v_.size();
}

Term SumOfSquaresCreator::getSquare(size_t i) const
{
   return v_[i];
}

void SumOfSquaresCreator::apply(const TermConst* t)
{}

void SumOfSquaresCreator::apply(const TermVar* t)
{}

void SumOfSquaresCreator::apply(const TermAdd* t)
{
   SumOfSquaresCreator vl;
   t->left()->acceptVisitor(vl);

   SumOfSquaresCreator vr;
   t->right()->acceptVisitor(vr);

   if (vl.sos_ && vr.sos_)
   {
      sos_ = true;
      
      for (auto t : vl.v_) v_.push_back(t);
      for (auto t : vr.v_) v_.push_back(t);
   }
}

void SumOfSquaresCreator::apply(const TermSub* t)
{}

void SumOfSquaresCreator::apply(const TermMul* t)
{}

void SumOfSquaresCreator::apply(const TermDiv* t)
{}

void SumOfSquaresCreator::apply(const TermMin* t)
{}

void SumOfSquaresCreator::apply(const TermMax* t)
{}

void SumOfSquaresCreator::apply(const TermUsb* t)
{}

void SumOfSquaresCreator::apply(const TermAbs* t)
{}

void SumOfSquaresCreator::apply(const TermSgn* t)
{}

void SumOfSquaresCreator::apply(const TermSqr* t)
{
   if (!t->child()->isConstant())
   {
      sos_ = true;
      v_.push_back(sqr(Term(t->child())));
   }
}

void SumOfSquaresCreator::apply(const TermSqrt* t)
{}

void SumOfSquaresCreator::apply(const TermPow* t)
{}

void SumOfSquaresCreator::apply(const TermExp* t)
{}

void SumOfSquaresCreator::apply(const TermLog* t)
{}

void SumOfSquaresCreator::apply(const TermCos* t)
{}

void SumOfSquaresCreator::apply(const TermSin* t)
{}

void SumOfSquaresCreator::apply(const TermTan* t)
{}

} // namespace
