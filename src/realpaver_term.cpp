// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_term.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const OpSymbol& s)
{
   switch(s)
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

TermRep::TermRep(OpPriority p) : hcode_(0), constant_(true), priority_(p)
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

///////////////////////////////////////////////////////////////////////////////

Term::Term(const double& a) : rep_(std::make_shared<TermConst>(a))
{}

Term::Term(const Interval& x) : rep_(std::make_shared<TermConst>(x))
{}

Term::Term(const Variable& v) : rep_(std::make_shared<TermVar>(v))
{}

Term::Term(const SharedRep& rep): rep_(rep)
{}

std::ostream& operator<<(std::ostream& os, const Term& t)
{
   t.print(os);
   return os;
}

Term& Term::operator+=(const Term& other)
{
   Term t( (*this) + other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator-=(const Term& other)
{
   Term t( (*this) - other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator*=(const Term& other)
{
   Term t( (*this) * other );
   rep_ = t.rep_;
   return *this;
}

Term& Term::operator/=(const Term& other)
{
   Term t( (*this) / other );
   rep_ = t.rep_;
   return *this;
}

Term operator+(const Term& l, const Term& r)
{
   if (l.isZero())
      return r;

   else if (r.isZero())
      return l;

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() + r.evalConst() );
      return Term(x);
   }

   else
      return Term(std::make_shared<TermAdd>(l.rep(),r.rep()));
}

Term operator-(const Term& l, const Term& r)
{
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
         return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
   }

   else
      return Term(std::make_shared<TermSub>(l.rep(),r.rep()));
}

Term operator*(const Term& l, const Term& r)
{
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

   else
      return Term(std::make_shared<TermMul>(l.rep(),r.rep()));
}

Term operator/(const Term& l, const Term& r)
{
   ASSERT(!r.isZero(), "term divided by zero");

   if (l.isZero() || r.isOne())
      return l;

   else if (l.isConstant() && r.isConstant())
   {
      Interval x( l.evalConst() / r.evalConst() );
      return Term(x);
   }

   else if (r.isMinusOne())
      return Term(std::make_shared<TermUsb>(l.rep()));

   else
      return Term(std::make_shared<TermDiv>(l.rep(),r.rep()));
}

Term mini(const Term& l, const Term& r)
{
   if (l.isConstant() && r.isConstant())
   {
      Interval x( min(l.evalConst(), r.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermMin>(l.rep(),r.rep()));
}

Term maxi(const Term& l, const Term& r)
{
   if (l.isConstant() && r.isConstant())
   {
      Interval x( max(l.evalConst(), r.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermMax>(l.rep(),r.rep()));
}

Term operator-(const Term& t)
{
   if (t.isZero())
      return t;

   else if (t.isConstant())
   {
      Interval x( -t.evalConst() );
      return Term(x);
   }

   else
      return Term(std::make_shared<TermUsb>(t.rep()));
}

Term abs(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( abs(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermAbs>(t.rep()));
}

Term sgn(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( sgn(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSgn>(t.rep()));
}

Term sqr(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( sqr(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSqr>(t.rep()));
}

Term sqrt(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( sqrt(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSqrt>(t.rep()));
}

Term pow(const Term& t, int n)
{
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
         return Term(std::make_shared<TermPow>(t.rep(),n));
   }
}

Term pow(const Term& t, const double& d)
{
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

Term exp(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( exp(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermExp>(t.rep()));
}

Term log(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( log(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermLog>(t.rep()));
}

Term cos(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( cos(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermCos>(t.rep()));
}

Term sin(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( sin(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermSin>(t.rep()));
}

Term tan(const Term& t)
{
   if (t.isConstant())
   {
      Interval x( tan(t.evalConst()) );
      return Term(x);
   }
   else
      return Term(std::make_shared<TermTan>(t.rep()));
}

///////////////////////////////////////////////////////////////////////////////

TermConst::TermConst(const Interval& x) :
   TermRep(OpPriority::Low), x_(x)
{
   ASSERT(!x.isEmpty(), "bad constant term: " << x);

   hcode_ = x.hashCode();
   constant_ = true;
}

Interval TermConst::evalConst() const
{
   return x_;
}

Interval TermConst::eval(const Box& B) const
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

///////////////////////////////////////////////////////////////////////////////

TermVar::TermVar(const Variable& v) : TermRep(OpPriority::Low), v_(v)
{
   hcode_ = v.hashCode();
   constant_ = false;
}

Interval TermVar::evalConst() const
{
   THROW("Evaluation of a non constant term");
   return Interval::universe();
}

Interval TermVar::eval(const Box& B) const
{
   return B[v_.id()];
}

void TermVar::print(std::ostream& os) const
{
   os << v_.name();
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

///////////////////////////////////////////////////////////////////////////////

TermOp::TermOp(const SharedRep& t, OpSymbol op, OpPriority p) :
   TermRep(p), v_(), op_(op)
{
   constant_ = true;
   insert(t);
}

TermOp::TermOp(const SharedRep& l, const SharedRep& r, OpSymbol op,
               OpPriority p) : TermRep(p), v_(), op_(op)
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
      hcode_ = Hash2(sub->hashCode(),hcode_);

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

///////////////////////////////////////////////////////////////////////////////

TermAdd::TermAdd(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Add,OpPriority::AddSub)
{}

Interval TermAdd::evalConst() const
{
   return left()->evalConst() + right()->evalConst();
}

Interval TermAdd::eval(const Box& B) const
{
   return left()->eval(B) + right()->eval(B);
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

///////////////////////////////////////////////////////////////////////////////

TermSub::TermSub(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Sub,OpPriority::AddSub)
{}

Interval TermSub::evalConst() const
{
   return left()->evalConst() - right()->evalConst();
}

Interval TermSub::eval(const Box& B) const
{
   return left()->eval(B) - right()->eval(B);
}

void TermSub::print(std::ostream& os) const
{
   OpPriority p = priority(),
            rp = right()->priority();

   left()->print(os);
   os << opSymbol();
   
   if (rp == p)
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

///////////////////////////////////////////////////////////////////////////////

TermMul::TermMul(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Mul,OpPriority::MulDiv)
{}

Interval TermMul::evalConst() const
{
   return left()->evalConst() * right()->evalConst();
}

Interval TermMul::eval(const Box& B) const
{
   return left()->eval(B) * right()->eval(B);
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

///////////////////////////////////////////////////////////////////////////////
TermDiv::TermDiv(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Div,OpPriority::MulDiv)
{}

Interval TermDiv::evalConst() const
{
   return left()->evalConst() / right()->evalConst();
}

Interval TermDiv::eval(const Box& B) const
{
   return left()->eval(B) / right()->eval(B);
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
   
   if (rp == OpPriority::AddSub)
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

///////////////////////////////////////////////////////////////////////////////

TermMin::TermMin(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Min,OpPriority::High)
{}

Interval TermMin::evalConst() const
{
   return min(left()->evalConst(), right()->evalConst());
}

Interval TermMin::eval(const Box& B) const
{
   return min(left()->eval(B), right()->eval(B));
}

void TermMin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermMax::TermMax(const SharedRep& l, const SharedRep& r) :
   TermOp(l,r,OpSymbol::Max,OpPriority::High)
{}

Interval TermMax::evalConst() const
{
   return max(left()->evalConst(), right()->evalConst());
}

Interval TermMax::eval(const Box& B) const
{
   return max(left()->eval(B), right()->eval(B));
}

void TermMax::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermUsb::TermUsb(const SharedRep& t) :
   TermOp(t,OpSymbol::Usb,OpPriority::AddSub)
{}

Interval TermUsb::evalConst() const
{
   return -sub()->evalConst();
}

Interval TermUsb::eval(const Box& B) const
{
   return -sub()->eval(B);
}

void TermUsb::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermUsb::isLinear() const
{
   return sub()->isLinear();
}

///////////////////////////////////////////////////////////////////////////////

TermAbs::TermAbs(const SharedRep& t) :
   TermOp(t,OpSymbol::Abs,OpPriority::High)
{}

Interval TermAbs::evalConst() const
{
   return abs(sub()->evalConst());
}

Interval TermAbs::eval(const Box& B) const
{
   return abs(sub()->eval(B));
}

void TermAbs::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermSgn::TermSgn(const SharedRep& t) :
   TermOp(t,OpSymbol::Sgn,OpPriority::High)
{}

Interval TermSgn::evalConst() const
{
   return sgn(sub()->evalConst());
}

Interval TermSgn::eval(const Box& B) const
{
   return sgn(sub()->eval(B));
}

void TermSgn::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermSqr::TermSqr(const SharedRep& t) :
   TermOp(t,OpSymbol::Sqr,OpPriority::High)
{}

Interval TermSqr::evalConst() const
{
   return sqr(sub()->evalConst());
}

Interval TermSqr::eval(const Box& B) const
{
   return sqr(sub()->eval(B));
}

void TermSqr::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermSqrt::TermSqrt(const SharedRep& t) :
   TermOp(t,OpSymbol::Sqrt,OpPriority::High)
{}

Interval TermSqrt::evalConst() const
{
   return sqrt(sub()->evalConst());
}

Interval TermSqrt::eval(const Box& B) const
{
   return sqrt(sub()->eval(B));
}

void TermSqrt::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermPow::TermPow(const SharedRep& t, int n) :
   TermOp(t,OpSymbol::Pow,OpPriority::High), n_(n)
{
   ASSERT(n > 0, "bad integral exponent in a term: " << n);

   size_t h = Hash1<int>(n);
   hcode_ = Hash2(h,hcode_);
}

Interval TermPow::evalConst() const
{
   return pow(sub()->evalConst(), n_);
}

Interval TermPow::eval(const Box& B) const
{
   return pow(sub()->eval(B), n_);
}

void TermPow::print(std::ostream& os) const
{
   os << opSymbol() << "(";
   sub()->print(os);
   os << "," << n_ << ")";
}

void TermPow::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermExp::TermExp(const SharedRep& t) :
   TermOp(t,OpSymbol::Exp,OpPriority::High)
{}

Interval TermExp::evalConst() const
{
   return exp(sub()->evalConst());
}

Interval TermExp::eval(const Box& B) const
{
   return exp(sub()->eval(B));
}

void TermExp::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermLog::TermLog(const SharedRep& t) :
   TermOp(t,OpSymbol::Log,OpPriority::High)
{}

Interval TermLog::evalConst() const
{
   return log(sub()->evalConst());
}

Interval TermLog::eval(const Box& B) const
{
   return log(sub()->eval(B));
}

void TermLog::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermCos::TermCos(const SharedRep& t) :
   TermOp(t,OpSymbol::Cos,OpPriority::High)
{}

Interval TermCos::evalConst() const
{
   return cos(sub()->evalConst());
}

Interval TermCos::eval(const Box& B) const
{
   return cos(sub()->eval(B));
}

void TermCos::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermSin::TermSin(const SharedRep& t) :
   TermOp(t,OpSymbol::Sin,OpPriority::High)
{}

Interval TermSin::evalConst() const
{
   return sin(sub()->evalConst());
}

Interval TermSin::eval(const Box& B) const
{
   return sin(sub()->eval(B));
}

void TermSin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermTan::TermTan(const SharedRep& t) :
   TermOp(t,OpSymbol::Tan,OpPriority::High)
{}

Interval TermTan::evalConst() const
{
   return tan(sub()->evalConst());
}

Interval TermTan::eval(const Box& B) const
{
   return tan(sub()->eval(B));
}

void TermTan::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

///////////////////////////////////////////////////////////////////////////////

TermVisitor::~TermVisitor()
{}

void TermVisitor::apply(const TermConst* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermVar* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermAdd* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermSub* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermMul* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermDiv* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermMin* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermMax* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermUsb* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermAbs* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermSgn* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermSqr* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermSqrt* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermPow* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermExp* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermLog* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermCos* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermSin* t)
{
   THROW("visit method not implemented");
}

void TermVisitor::apply(const TermTan* t)
{
   THROW("visit method not implemented");
}

} // namespace
