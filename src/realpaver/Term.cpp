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
 * @file   Term.cpp
 * @brief  Terms (nonlinear expressions)
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/ScopeBank.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, TermSymbol op)
{
   switch(op)
   {
      case TermSymbol::Cst:  return os << "cst";
      case TermSymbol::Var:  return os << "var";
      case TermSymbol::Add:  return os << "+";
      case TermSymbol::Sub:  return os << "-";
      case TermSymbol::Mul:  return os << "*";
      case TermSymbol::Div:  return os << "/";
      case TermSymbol::Min:  return os << "min";
      case TermSymbol::Max:  return os << "max";
      case TermSymbol::Usb:  return os << "-";
      case TermSymbol::Abs:  return os << "abs";
      case TermSymbol::Sgn:  return os << "sgn";
      case TermSymbol::Sqr:  return os << "sqr";
      case TermSymbol::Sqrt: return os << "sqrt";
      case TermSymbol::Pow:  return os << "pow";
      case TermSymbol::Exp:  return os << "exp";
      case TermSymbol::Log:  return os << "log";
      case TermSymbol::Cos:  return os << "cos";
      case TermSymbol::Sin:  return os << "sin";
      case TermSymbol::Tan:  return os << "tan";
      case TermSymbol::Cosh: return os << "cosh";
      case TermSymbol::Sinh: return os << "sinh";
      case TermSymbol::Tanh: return os << "tanh";
      default:             os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

TermRep::TermRep(TermSymbol symb, NodePriority p)
      : symb_(symb),
        hcode_(0),
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

TermSymbol TermRep::symbol() const
{
   return symb_;
}

NodePriority TermRep::priority() const
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

/*----------------------------------------------------------------------------*/

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

Term::Term(double a) : rep_(std::make_shared<TermCst>(a))
{}

Term::Term(const Interval& x) : rep_(std::make_shared<TermCst>(x))
{}

Term::Term(Variable v) : rep_(std::make_shared<TermVar>(v))
{}

Term::Term(const SharedRep& rep): rep_(rep)
{}

size_t Term::hashCode() const
{
   return rep_->hashCode();
}

size_t Term::nbNodes() const
{
   return rep_->nbNodes();
}

void Term::print(std::ostream& os) const
{
   rep_->print(os);
}

Interval Term::evalConst() const
{
   return rep_->evalConst();
}

Interval Term::eval(const IntervalBox& B) const
{
   rep_->eval(B);
   return rep_->ival_;
}

std::unique_ptr<Domain> Term::eval(const DomainBox& box) const
{
   rep_->eval(box);
   return std::make_unique<IntervalDomain>(rep_->ival_);
}

void Term::acceptVisitor(TermVisitor& vis) const
{
   rep_->acceptVisitor(vis);
}

bool Term::dependsOn(const Variable& v) const
{
   return rep_->dependsOn(v);
}

void Term::makeScope(Scope& scop) const
{
   rep_->makeScope(scop);
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

bool Term::isConstant() const
{
   return rep_->isConstant();
}

bool Term::isLinear() const
{
   return rep_->isLinear();
}

bool Term::isInteger() const
{
   return rep_->isInteger();
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
   Scope scop;
   makeScope(scop);
   return ScopeBank::getInstance()->insertScope(scop);
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

Term pow(Term t, Term e)
{
   if (e.isConstant())
   {
      Interval d = e.evalConst();
      return pow(t, d);
   }
   else
      return exp(e*log(t));
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

Term cosh(Term t)
{
   if (!Term::simplification())
      return Term(std::make_shared<TermCosh>(t.rep()));

   if (t.isConstant())
   {
      Interval x( cosh(t.evalConst()) );
      return Term(x);
   }

   else
      return Term(std::make_shared<TermCosh>(t.rep()));
}

Term sinh(Term t)
{
   if (!Term::simplification())
      return Term(std::make_shared<TermSinh>(t.rep()));

   if (t.isConstant())
   {
      Interval x( sinh(t.evalConst()) );
      return Term(x);
   }

   else
      return Term(std::make_shared<TermSinh>(t.rep()));
}

Term tanh(Term t)
{
   if (!Term::simplification())
      return Term(std::make_shared<TermTanh>(t.rep()));

   if (t.isConstant())
   {
      Interval x( tanh(t.evalConst()) );
      return Term(x);
   }

   else
      return Term(std::make_shared<TermTanh>(t.rep()));
}

/*----------------------------------------------------------------------------*/

TermCst::TermCst(const Interval& x)
      : TermRep(TermSymbol::Cst, NodePriority::Low),
        x_(x)
{
   ASSERT(!x.isEmpty(), "Bad constant term " << x);

   hcode_ = x.hashCode();
   constant_ = true;
   ival_ = x_;
}

size_t TermCst::nbNodes() const
{
   return 1;
}

Interval TermCst::getVal() const
{
   return x_;
}

Interval TermCst::evalConst() const
{
   return x_;
}

void TermCst::eval(const IntervalBox& B)
{
   ival_ = x_;
}

void TermCst::eval(const DomainBox& box)
{
   ival_ = x_;
}

void TermCst::print(std::ostream& os) const
{
   if (Term::idisplay())
      os << x_;
   else
      os << x_.midpoint();
}

void TermCst::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermCst::isNumber() const
{
   return true;
}

bool TermCst::isZero() const
{
   return x_.isZero();
}

bool TermCst::isOne() const
{
   return (x_.left() == 1.0) && (x_.right() == 1.0);
}

bool TermCst::isMinusOne() const
{
   return (x_.left() == -1.0) && (x_.right() == -1.0);
}

bool TermCst::dependsOn(const Variable& v) const
{
   return false;
}

bool TermCst::isLinear() const
{
   return true;
}

bool TermCst::isInteger() const
{
   return x_.isAnInt();
}

void TermCst::makeScope(Scope& scop) const
{}

TermRep* TermCst::cloneRoot() const
{
   return new TermCst(x_);
}

TermRep* TermCst::clone() const
{
   return new TermCst(x_);
}

/*----------------------------------------------------------------------------*/

TermVar::TermVar(Variable v)
      : TermRep(TermSymbol::Var, NodePriority::Low),
        v_(v)
{
   hcode_ = v.hashCode();
   constant_ = false;
}

size_t TermVar::nbNodes() const
{
   return 1;
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

void TermVar::eval(const IntervalBox& B)
{
   ival_ = B.get(v_);
}

void TermVar::eval(const DomainBox& box)
{
   ival_ = box.get(v_)->intervalHull();
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

bool TermVar::isInteger() const
{
   return v_.getDomain()->isInteger();
}

void TermVar::makeScope(Scope& scop) const
{
   scop.insert(v_);
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

/*----------------------------------------------------------------------------*/

TermOp::TermOp(const SharedRep& t, TermSymbol symb, NodePriority p)
      : TermRep(symb, p),
        v_()
{
   constant_ = true;
   insert(t);
}

TermOp::TermOp(const SharedRep& l, const SharedRep& r, TermSymbol symb,
               NodePriority p)
      : TermRep(symb, p),
        v_()
{
   constant_ = true;
   insert(l);
   insert(r);
}

TermOp::~TermOp()
{}

size_t TermOp::nbNodes() const
{
   size_t nb = 1;
   for (const auto& sub : v_)
      nb += sub->nbNodes();
   return nb;
}

bool TermOp::isAdd() const
{
   return symbol() == TermSymbol::Add;
}

bool TermOp::isSub() const
{
   return symbol() == TermSymbol::Sub;
}

bool TermOp::isMul() const
{
   return symbol() == TermSymbol::Mul;
}

bool TermOp::isDiv() const
{
   return symbol() == TermSymbol::Div;
}

bool TermOp::isUsb() const
{
   return symbol() == TermSymbol::Usb;
}

bool TermOp::isIntegerRoot() const
{
   return false;
}

bool TermOp::isInteger() const
{
   if (!isIntegerRoot()) return false;

   for (const auto& sub : v_)
      if (!sub->isInteger())
         return false;

   return true;
}

bool TermOp::isLinear() const
{
   return isConstant();
}

void TermOp::insert(const SharedRep& t)
{
   v_.push_back(t);

   hcode_ = static_cast<size_t>(symbol());
   for (auto sub : v_)
      hcode_ = hash2(sub->hashCode(), hcode_);

   if (!t->isConstant())
      constant_ = false;
}

void TermOp::print(std::ostream& os) const
{
   // default implementation in prefix operation
   os << symbol() << "(";
   bool first = false;
   for (auto sub : v_)
   {
      if (first) os << ",";
      sub->print(os);
      first = true;
   }
   os << ")";
}

void TermOp::eval(const IntervalBox& B)
{
   for (auto sub : v_) sub->eval(B);
   evalRoot();
}

void TermOp::eval(const DomainBox& box)
{
   for (auto sub : v_) sub->eval(box);
   evalRoot();
}

bool TermOp::dependsOn(const Variable& v) const
{
   for (auto sub : v_)
      if (sub->dependsOn(v))
         return true;

   return false;
}

void TermOp::makeScope(Scope& scop) const
{
   for (auto sub : v_)
      sub->makeScope(scop);
}

size_t TermOp::arity() const
{
   return v_.size();
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

/*----------------------------------------------------------------------------*/

TermAdd::TermAdd(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Add, NodePriority::AddSub)
{}

Interval TermAdd::evalConst() const
{
   return left()->evalConst() + right()->evalConst();
}

void TermAdd::evalRoot()
{
   ival_ = left()->ival() + right()->ival();
}

bool TermAdd::isIntegerRoot() const
{
   return true;
}

void TermAdd::print(std::ostream& os) const
{
   left()->print(os);
   os << " " << symbol() << " ";
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

/*----------------------------------------------------------------------------*/

TermSub::TermSub(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Sub, NodePriority::AddSub)
{}

Interval TermSub::evalConst() const
{
   return left()->evalConst() - right()->evalConst();
}

void TermSub::evalRoot()
{
   ival_ = left()->ival() - right()->ival();
}

void TermSub::print(std::ostream& os) const
{
   NodePriority p = priority(),
              rp = right()->priority();

   left()->print(os);
   os << " " << symbol() << " ";

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

bool TermSub::isIntegerRoot() const
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

/*----------------------------------------------------------------------------*/

TermMul::TermMul(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Mul, NodePriority::MulDiv)
{}

Interval TermMul::evalConst() const
{
   return left()->evalConst() * right()->evalConst();
}

void TermMul::evalRoot()
{
   ival_ = left()->ival() * right()->ival();
}

void TermMul::print(std::ostream& os) const
{
   NodePriority p = priority(),
            lp = left()->priority(),
            rp = right()->priority();

   if (lp < p && lp > NodePriority::Low)
   {
      os << "(";
      left()->print(os);
      os << ")";
   }
   else
      left()->print(os);

   os << symbol();

   if (rp < p && rp > NodePriority::Low)
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

bool TermMul::isIntegerRoot() const
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

/*----------------------------------------------------------------------------*/
TermDiv::TermDiv(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Div, NodePriority::MulDiv)
{}

Interval TermDiv::evalConst() const
{
   return left()->evalConst() / right()->evalConst();
}

void TermDiv::evalRoot()
{
   ival_ = left()->ival() / right()->ival();
}

void TermDiv::print(std::ostream& os) const
{
   NodePriority lp = left()->priority(),
            rp = right()->priority();

   if (lp == NodePriority::AddSub)
   {
      os << "(";
      left()->print(os);
      os << ")";
   }
   else
      left()->print(os);

   os << symbol();

   if (rp == NodePriority::AddSub || rp == NodePriority::MulDiv)
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

bool TermDiv::isIntegerRoot() const
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

/*----------------------------------------------------------------------------*/

TermMin::TermMin(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Min, NodePriority::High)
{}

Interval TermMin::evalConst() const
{
   return min(left()->evalConst(), right()->evalConst());
}

void TermMin::evalRoot()
{
   ival_ = min(left()->ival(), right()->ival());
}

void TermMin::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermMin::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermMax::TermMax(const SharedRep& l, const SharedRep& r)
      : TermOp(l, r, TermSymbol::Max, NodePriority::High)
{}

Interval TermMax::evalConst() const
{
   return max(left()->evalConst(), right()->evalConst());
}

void TermMax::evalRoot()
{
   ival_ = max(left()->ival(), right()->ival());
}

void TermMax::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermMax::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermUsb::TermUsb(const SharedRep& t)
      : TermOp(t, TermSymbol::Usb, NodePriority::AddSub)
{}

Interval TermUsb::evalConst() const
{
   return -child()->evalConst();
}

void TermUsb::evalRoot()
{
   ival_ = -child()->ival();
}

void TermUsb::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermUsb::isIntegerRoot() const
{
   return true;
}

bool TermUsb::isLinear() const
{
   return child()->isLinear();
}

void TermUsb::print(std::ostream& os) const
{
   NodePriority p = priority(),
              rp = child()->priority();

   os << symbol();

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

/*----------------------------------------------------------------------------*/

TermAbs::TermAbs(const SharedRep& t)
      : TermOp(t, TermSymbol::Abs, NodePriority::High)
{}

Interval TermAbs::evalConst() const
{
   return abs(child()->evalConst());
}

void TermAbs::evalRoot()
{
   ival_ = abs(child()->ival());
}

void TermAbs::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermAbs::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermSgn::TermSgn(const SharedRep& t)
      : TermOp(t, TermSymbol::Sgn, NodePriority::High)
{}

Interval TermSgn::evalConst() const
{
   return sgn(child()->evalConst());
}

void TermSgn::evalRoot()
{
   ival_ = sgn(child()->ival());
}

void TermSgn::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermSgn::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermSqr::TermSqr(const SharedRep& t)
      : TermOp(t, TermSymbol::Sqr, NodePriority::High)
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
   NodePriority p = priority(),
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

void TermSqr::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermSqr::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermSqrt::TermSqrt(const SharedRep& t)
      : TermOp(t, TermSymbol::Sqrt, NodePriority::High)
{}

Interval TermSqrt::evalConst() const
{
   return sqrt(child()->evalConst());
}

void TermSqrt::evalRoot()
{
   ival_ = sqrt(child()->ival());
}

void TermSqrt::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermSqrt::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermPow::TermPow(const SharedRep& t, int n)
      : TermOp(t, TermSymbol::Pow, NodePriority::High),
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

void TermPow::print(std::ostream& os) const
{
   os << symbol() << "(";
   child()->print(os);
   os << "," << n_ << ")";
}

void TermPow::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

bool TermPow::isIntegerRoot() const
{
   return true;
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

/*----------------------------------------------------------------------------*/

TermExp::TermExp(const SharedRep& t)
      : TermOp(t, TermSymbol::Exp, NodePriority::High)
{}

Interval TermExp::evalConst() const
{
   return exp(child()->evalConst());
}

void TermExp::evalRoot()
{
   ival_ = exp(child()->ival());
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

/*----------------------------------------------------------------------------*/

TermLog::TermLog(const SharedRep& t)
      : TermOp(t, TermSymbol::Log, NodePriority::High)
{}

Interval TermLog::evalConst() const
{
   return log(child()->evalConst());
}

void TermLog::evalRoot()
{
   ival_ = log(child()->ival());
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

/*----------------------------------------------------------------------------*/

TermCos::TermCos(const SharedRep& t)
      : TermOp(t, TermSymbol::Cos, NodePriority::High)
{}

Interval TermCos::evalConst() const
{
   return cos(child()->evalConst());
}

void TermCos::evalRoot()
{
   ival_ = cos(child()->ival());
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

/*----------------------------------------------------------------------------*/

TermSin::TermSin(const SharedRep& t)
      : TermOp(t, TermSymbol::Sin, NodePriority::High)
{}

Interval TermSin::evalConst() const
{
   return sin(child()->evalConst());
}

void TermSin::evalRoot()
{
   ival_ = sin(child()->ival());
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

/*----------------------------------------------------------------------------*/

TermTan::TermTan(const SharedRep& t)
      : TermOp(t, TermSymbol::Tan, NodePriority::High)
{}

Interval TermTan::evalConst() const
{
   return tan(child()->evalConst());
}

void TermTan::evalRoot()
{
   ival_ = tan(child()->ival());
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

/*----------------------------------------------------------------------------*/

TermCosh::TermCosh(const SharedRep& t)
      : TermOp(t, TermSymbol::Cosh, NodePriority::High)
{}

Interval TermCosh::evalConst() const
{
   return cosh(child()->evalConst());
}

void TermCosh::evalRoot()
{
   ival_ = cosh(child()->ival());
}

void TermCosh::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermCosh::cloneRoot() const
{
   return new TermCosh(child());
}

TermRep* TermCosh::clone() const
{
   SharedRep sc(child()->clone());
   return new TermCosh(sc);
}

/*----------------------------------------------------------------------------*/

TermSinh::TermSinh(const SharedRep& t)
      : TermOp(t, TermSymbol::Sinh, NodePriority::High)
{}

Interval TermSinh::evalConst() const
{
   return sinh(child()->evalConst());
}

void TermSinh::evalRoot()
{
   ival_ = sinh(child()->ival());
}

void TermSinh::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermSinh::cloneRoot() const
{
   return new TermSinh(child());
}

TermRep* TermSinh::clone() const
{
   SharedRep sc(child()->clone());
   return new TermSinh(sc);
}

/*----------------------------------------------------------------------------*/

TermTanh::TermTanh(const SharedRep& t)
      : TermOp(t, TermSymbol::Tanh, NodePriority::High)
{}

Interval TermTanh::evalConst() const
{
   return tanh(child()->evalConst());
}

void TermTanh::evalRoot()
{
   ival_ = tanh(child()->ival());
}

void TermTanh::acceptVisitor(TermVisitor& vis) const
{
   vis.apply(this);
}

TermRep* TermTanh::cloneRoot() const
{
   return new TermTanh(child());
}

TermRep* TermTanh::clone() const
{
   SharedRep sc(child()->clone());
   return new TermTanh(sc);
}

/*----------------------------------------------------------------------------*/

TermVisitor::~TermVisitor()
{}

void TermVisitor::apply(const TermCst* t)
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

void TermVisitor::apply(const TermCosh* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermSinh* t)
{
   THROW("Visit method not implemented");
}

void TermVisitor::apply(const TermTanh* t)
{
   THROW("Visit method not implemented");
}

/*----------------------------------------------------------------------------*/

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

void SumOfSquaresCreator::apply(const TermCst* t)
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

void SumOfSquaresCreator::apply(const TermCosh* t)
{}

void SumOfSquaresCreator::apply(const TermSinh* t)
{}

void SumOfSquaresCreator::apply(const TermTanh* t)
{}

} // namespace
