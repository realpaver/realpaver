///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
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
      case OpSymbol::Usb:  return os << "-";
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
      case OpSymbol::Lin:  return os << "LIN";
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

bool TermRep::isLin() const
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

bool Term::isLin() const
{
   return rep_->isLin();
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

Term Term::clone() const
{
   SharedRep srep(rep_->clone());
   return Term(srep);
}

Scope Term::scope() const
{
   Scope s;
   makeScope(s);
   return s;
}

Proof Term::contract(IntervalRegion& reg, const Interval& img)
{
   hc4ReviseForward(reg);
   return hc4ReviseBackward(reg, img);
}

std::ostream& operator<<(std::ostream& os, const Term& t)
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

Term simplifyLin(Term tl)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());

   if (tlin->isConstant())
      return Term(tlin->evalConst());

   else if (tlin->isVariable())
      return Term(tlin->getVarSub(0));

   else
      return tl;
}

Term addConstVar(Term tc, Term tv)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>();
   res->addConstant(x);
   res->addTerm(1.0, tvar->var());
   return Term(res);
}

Term addConstLin(Term tc, Term tl)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->addConstant(x);
   return Term(res);
}

Term addVarVar(Term tv, Term tw)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   TermVar* twar = static_cast<TermVar*>(tw.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>();
   res->addTerm(1.0, tvar->var());
   res->addTerm(1.0, twar->var());
   return simplifyLin(Term(res));
}

Term addVarLin(Term tv, Term tl)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->addTerm(1.0, tvar->var());
   return simplifyLin(Term(res));
}

Term addLinLin(Term tl, Term tm)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());
   TermLin* tmin = static_cast<TermLin*>(tm.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->addTermLin(*tmin);
   return simplifyLin(Term(res));
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

   else if (l.isConstant() && r.isVar())
      return addConstVar(l, r);

   else if (l.isVar() && r.isConstant())
      return addConstVar(r, l);

   else if (l.isConstant() && r.isLin())
      return addConstLin(l, r);

   else if (l.isLin() && r.isConstant())
      return addConstLin(r, l);

   else if (l.isVar() && r.isVar())
      return addVarVar(l, r);

   else if (l.isVar() && r.isLin())
      return addVarLin(l, r);

   else if (l.isLin() && r.isVar())
      return addVarLin(r, l);

   else if (l.isLin() && r.isLin())
      return addLinLin(l, r);

   else
      return Term(std::make_shared<TermAdd>(l.rep(), r.rep()));
}

Term subConstVar(Term tc, Term tv)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>();
   res->addConstant(x);
   res->addTerm(-1.0, tvar->var());

   return Term(res);
}

Term subVarConst(Term tv, Term tc)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>();
   res->addConstant(-x);
   res->addTerm(1.0, tvar->var());

   return Term(res);
}

Term subConstLin(Term tc, Term tl)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->toOpposite();
   res->addConstant(x);
   return Term(res);
}

Term subLinConst(Term tl, Term tc)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->subConstant(x);
   return Term(res);
}

Term subVarVar(Term tv, Term tw)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   TermVar* twar = static_cast<TermVar*>(tw.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>();
   res->addTerm(1.0, tvar->var());
   res->addTerm(-1.0, twar->var());
   return simplifyLin(Term(res));
}

Term subVarLin(Term tv, Term tl)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->toOpposite();
   res->addTerm(1.0, tvar->var());

   return simplifyLin(Term(res));
}

Term subLinVar(Term tl, Term tv)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->addTerm(-1.0, tvar->var());

   return simplifyLin(Term(res));
}

Term subLinLin(Term tl, Term tm)
{
   TermLin* tlin = static_cast<TermLin*>(tl.rep().get());
   TermLin* tmin = static_cast<TermLin*>(tm.rep().get());

   std::shared_ptr<TermLin> res = std::make_shared<TermLin>(*tlin);
   res->subTermLin(*tmin);
   return simplifyLin(Term(res));
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

   else if (l.isConstant() && r.isVar())
      return subConstVar(l, r);

   else if (l.isVar() && r.isConstant())
      return subVarConst(l, r);

   else if (l.isConstant() && r.isLin())
      return subConstLin(l, r);

   else if (l.isLin() && r.isConstant())
      return subLinConst(l, r);

   else if (l.isVar() && r.isVar())
      return subVarVar(l, r);

   else if (l.isVar() && r.isLin())
      return subVarLin(l, r);

   else if (l.isLin() && r.isVar())
      return subLinVar(l, r);

   else if (l.isLin() && r.isLin())
      return subLinLin(l, r);

   else
      return Term(std::make_shared<TermSub>(l.rep(), r.rep()));
}

Term makeProdLin(Term tc, Term tv)
{
   TermVar* tvar = static_cast<TermVar*>(tv.rep().get());
   Interval x = tc.evalConst();

   std::shared_ptr<TermLin> tlin = std::make_shared<TermLin>();
   tlin->addTerm(x, tvar->var());

   return Term(tlin);
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

   else if (l.isVar() && r.isConstant())
      return makeProdLin(r, l);

   else if (l.isConstant() && r.isVar())
      return makeProdLin(l, r);      

   else if (l.isMinusOne())
      return Term(std::make_shared<TermUsb>(r.rep()));

   else if (r.isMinusOne())
      return Term(std::make_shared<TermUsb>(l.rep()));

   else
      return Term(std::make_shared<TermMul>(l.rep(), r.rep()));
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
         return pow(t, double(n));

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

bool TermConst::isNumber() const
{
   return true;
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

TermRep* TermConst::clone() const
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
   ival_ &= reg.get(v_);

#if LOG_ON
   LOG_FULL("term contract variable " << v_.getName() << " -> " << ival_);
#endif

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

TermRep* TermVar::clone() const
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
   os << " " << opSymbol() << " ";
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

TermRep* TermAdd::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermAdd(sl, sr);
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

   left()->print(os);
   os << " " << opSymbol() << " ";
   
   if (rp <= p && (!right()->isVar()) && (!right()->isNumber()))
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

TermRep* TermSub::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermSub(sl, sr);
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
   return (left()->isConstant() && right()->isLinear()) ||
          (left()->isLinear() && right()->isConstant());
}

bool TermMul::isMul() const
{
   return true;
}

TermRep* TermMul::cloneRoot() const
{
   return new TermMul(left(), right());
}

TermRep* TermMul::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermMul(sl, sr);
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

TermRep* TermDiv::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermDiv(sl, sr);
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

TermRep* TermMin::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermMin(sl, sr);
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

TermRep* TermMax::clone() const
{
   SharedRep sl(left()->clone()),
             sr(right()->clone());
   return new TermMax(sl, sr);
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

void TermUsb::print(std::ostream& os) const
{
   OpPriority p = priority(),
              rp = child()->priority();

   os << opSymbol();
   
   if (rp <= p && (!child()->isVar()))
   {
      os << "(";
      child()->print(os);
      os << ")";
   }
   else
      child()->print(os);
}

TermRep* TermUsb::cloneRoot() const
{
   return new TermUsb(child());
}

TermRep* TermUsb::clone() const
{
   SharedRep sc(child()->clone());
   return new TermUsb(sc);
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

TermRep* TermAbs::clone() const
{
   SharedRep sc(child()->clone());
   return new TermAbs(sc);
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

TermRep* TermSgn::clone() const
{
   SharedRep sc(child()->clone());
   return new TermSgn(sc);
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
   OpPriority p = priority(),
              rp = child()->priority();

   if (p <= rp || child()->isVar())
   {
      child()->print(os);
      os << "^2";
   }
   else
   {
      os << "(";
      child()->print(os);
      os << ")^2";
   }
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

TermRep* TermSqr::clone() const
{
   SharedRep sc(child()->clone());
   return new TermSqr(sc);
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

TermRep* TermSqrt::clone() const
{
   SharedRep sc(child()->clone());
   return new TermSqrt(sc);
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

TermRep* TermPow::clone() const
{
   SharedRep sc(child()->clone());
   return new TermPow(sc, n_);
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

TermRep* TermExp::clone() const
{
   SharedRep sc(child()->clone());
   return new TermExp(sc);
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

TermRep* TermLog::clone() const
{
   SharedRep sc(child()->clone());
   return new TermLog(sc);
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

TermRep* TermCos::clone() const
{
   SharedRep sc(child()->clone());
   return new TermCos(sc);
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

TermRep* TermSin::clone() const
{
   SharedRep sc(child()->clone());
   return new TermSin(sc);
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

TermRep* TermTan::clone() const
{
   SharedRep sc(child()->clone());
   return new TermTan(sc);
}

///////////////////////////////////////////////////////////////////////////////

TermLin::TermLin()
      : TermRep(OpPriority::AddSub),
        cst_(0.0),
        terms_()
{
   constant_ = true;
}

void TermLin::addConstant(const Interval& val)
{
   cst_ += val;

   makeHashCode();
}

void TermLin::subConstant(const Interval& val)
{
   cst_ -= val;

   makeHashCode();
}

void TermLin::addTerm(const Interval& x, Variable v)
{
   Item itm = {x, v, Interval::zero()};
   auto it = terms_.find(itm);

   if (it == terms_.end())
   {
      terms_.insert(itm);
   }
   else
   {
      Interval z = (*it).coef + x;
      terms_.erase(it);

      if (!z.isZero())
      {
         itm.coef = z;
         terms_.insert(itm);
      }
   }
   constant_ = terms_.empty();

   makeHashCode();
}

void TermLin::subTerm(const Interval& x, Variable v)
{
   addTerm(-x, v);
}

void TermLin::addTermLin(const TermLin& t)
{
   addConstant(t.cst_);
   for (const Item& itm : t.terms_)
      addTerm(itm.coef, itm.var);
}

void TermLin::subTermLin(const TermLin& t)
{
   subConstant(t.cst_);
   for (const Item& itm : t.terms_)
      subTerm(itm.coef, itm.var);
}

Interval TermLin::getConstantValue() const
{
   return cst_;
}

size_t TermLin::getNbSub() const
{
   return terms_.size();
}

Interval TermLin::getCoefSub(size_t i) const
{
   ASSERT(i<getNbSub(), "Bad access to a sub-term in a linear term");

   auto it = terms_.begin();
   std::advance(it, i);
   return (*it).coef;
}

Variable TermLin::getVarSub(size_t i) const
{
   ASSERT(i<getNbSub(), "Bad access to a sub-term in a linear term");

   auto it = terms_.begin();
   std::advance(it, i);
   return (*it).var;
}

bool TermLin::isVariable() const
{
   if (!cst_.isZero()) return false;
   if (terms_.size() != 1) return false;
   auto it = terms_.begin();
   return (*it).coef.isSetEq(1.0);
}

void print_coef(std::ostream& os, const Interval& x, bool first)
{
   if (x.isSetEq(1.0))
   {
      if (!first) os << "+";
   }
   else if (x.isSetEq(-1.0))
   {
      os << "-";
   }
   else if (x.isNegative())
   {
      Interval y = -x;
      os << "-" << y << "*";
   }
   else
   {
      if (!first) os << "+";
      os << x << "*";
   }
}

void TermLin::print(std::ostream& os) const
{
   bool first = true;

   for (const Item& itm : terms_)
   {
      print_coef(os, itm.coef, first);
      os << itm.var.getName();
      first = false;
   }

   if (!cst_.isZero())
   {
      if (cst_.isCertainlyLeZero())
         os << "-" << (-cst_);

      else
      {
         if (!first) os << "+";
         os << cst_;
      }
   }
}

Interval TermLin::evalConst() const
{
   ASSERT(terms_.empty(), "Constant evaluation of a non-constant linear term");

   return cst_;
}

void TermLin::eval(const IntervalRegion& reg)
{
   ival_ = cst_;

   for (const Item& citm : terms_)
   {
      // const cast necessary since the elements of a set must be constants
      // we do that since the modification of ival does not affect the ordering
      // of the elements (only the variable identifiers are used as keys)
      Item& itm = const_cast<Item&>(citm);
      itm.ival = itm.coef * reg.get(itm.var);
      ival_ += itm.ival;
   }
}

Proof TermLin::contract(IntervalRegion& reg)
{
   for (auto it=terms_.begin(); it!=terms_.end(); ++it)
   {
      // contracts the domain of it->var
      Interval x = ival_ - cst_;

      auto jt = terms_.begin();
      while (jt != it)
      {
         x -= jt->ival;
         ++jt;
      }
      ++jt;
      while (jt != terms_.end())
      {
         x -= jt->ival;
         ++jt;
      }

      Interval dom = reg.get(it->var);
      dom = mulPY(it->coef, dom, x);
      reg.set(it->var, dom);

      if (dom.isEmpty())
         return Proof::Empty;
   }
   
   return Proof::Maybe;
}

void TermLin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermLin::isLinear() const
{
   return true;
}

bool TermLin::isLin() const
{
   return true;
}

bool TermLin::dependsOn(const Variable& v) const
{
   Item itm = {Interval::zero(), v, Interval::zero()};
   auto it = terms_.find(itm);
   return it != terms_.end();
}

void TermLin::makeScope(Scope& s) const
{
   for (const auto& itm : terms_)
      s.insert(itm.var);
}

TermRep* TermLin::cloneRoot() const
{
   return new TermLin(*this);
}

TermRep* TermLin::clone() const
{
   return new TermLin(*this);
}

void TermLin::toOpposite()
{
   cst_ = -cst_;
   for (const Item& citm : terms_)
   {
      Item& itm = const_cast<Item&>(citm);
      itm.coef = -itm.coef;
   }
}

TermLin::const_iterator TermLin::begin() const
{
    return terms_.cbegin();
}

TermLin::const_iterator TermLin::end() const
{
    return terms_.cend();
}

TermLin::const_iterator TermLin::find(const Variable& v) const
{
   Item itm = {Interval::zero(), v, Interval::zero()};
   return terms_.find(itm);
}

Interval TermLin::getCoefSub(const_iterator it) const
{
    return (*it).coef;
}

Variable TermLin::getVarSub(const_iterator it) const
{
    return (*it).var;    
}

void TermLin::makeHashCode()
{
   hcode_ = static_cast<size_t>(OpSymbol::Lin);
   hcode_ = hash2(cst_.hashCode(), hcode_);

   for (const auto& itm : terms_)
   {
      hcode_ = hash2(itm.coef.hashCode(), hcode_);
      hcode_ = hash2(itm.var.hashCode(), hcode_);
   }
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

void TermVisitor::apply(const TermLin* t)
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

void SumOfSquaresCreator::apply(const TermLin* t)
{}

} // namespace
