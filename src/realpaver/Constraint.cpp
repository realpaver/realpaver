///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Constraint.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, RelSymbol rel)
{
   switch(rel)
   {
      case RelSymbol::Eq: return os << "==";
      case RelSymbol::Le: return os << "<=";
      case RelSymbol::Lt: return os << "<";
      case RelSymbol::Ge: return os << ">=";
      case RelSymbol::Gt: return os << ">";
      case RelSymbol::In: return os << "in";
      default:            os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

ConstraintRep::ConstraintRep()
      : scope_(),
        bs_(),
        hcode_(0)
{}

ConstraintRep::~ConstraintRep()
{}

size_t ConstraintRep::hashCode() const
{
   return hcode_;
}

Bitset ConstraintRep::bitset() const
{
   return bs_;
}

Scope ConstraintRep::scope() const
{
   return scope_;
}

void ConstraintRep::setScope(Scope s)
{
   scope_ = s;
   
   if (s.isEmpty())
      bs_ = Bitset(1,0);

   else
      bs_ = scope_.toBitset();
}

bool ConstraintRep::isEquation() const
{
   return false;
}

bool ConstraintRep::isInequality() const
{
   return false;
}

bool ConstraintRep::isLinear() const
{
   return false;
}

///////////////////////////////////////////////////////////////////////////////

Constraint::Constraint(const SharedRep& rep) : rep_(rep)
{}

Constraint::SharedRep Constraint::rep() const
{
   return rep_;
}

size_t Constraint::hashCode() const
{
   return rep_->hashCode();
}

Scope Constraint::scope() const
{
   return rep_->scope();
}

Bitset Constraint::bitset() const
{
   return rep_->bitset();
}

bool Constraint::isConstant() const
{
   return rep_->isConstant();
}

void Constraint::print(std::ostream& os) const
{
   rep_->print(os);
}

void Constraint::acceptVisitor(ConstraintVisitor& vis) const
{
   rep_->acceptVisitor(vis);
}

Proof Constraint::isSatisfied(const IntervalRegion& reg) const
{
   return rep_->isSatisfied(reg);
}

bool Constraint::dependsOn(Variable v) const
{
   return rep_->dependsOn(v);
}

bool Constraint::isEquation() const
{
   return rep_->isEquation();
}

bool Constraint::isInequality() const
{
   return rep_->isInequality();
}

bool Constraint::isLinear() const
{
   return rep_->isLinear();
}

std::ostream& operator<<(std::ostream& os, Constraint c)
{
   c.print(os);
   return os;
}

///////////////////////////////////////////////////////////////////////////////

ConstraintBin::ConstraintBin(Term l, Term r, RelSymbol rel)
      : ConstraintRep(),
        l_(l),
        r_(r),
        rel_(rel)
{
   hcode_ = static_cast<size_t>(rel);
   hcode_ = hash2(l.hashCode(), hcode_);
   hcode_ = hash2(r.hashCode(), hcode_);

   Scope s;
   l.makeScope(s);
   r.makeScope(s);
   setScope(s);
}

ConstraintBin::~ConstraintBin()
{}

Term ConstraintBin::left() const
{
   return l_;
}

Term ConstraintBin::right() const
{
   return r_;
}

RelSymbol ConstraintBin::relSymbol() const
{
   return rel_;
}

bool ConstraintBin::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

void ConstraintBin::print(std::ostream& os) const
{
   l_.print(os);
   os << " " << rel_ << " ";
   r_.print(os);
}

bool ConstraintBin::dependsOn(Variable v) const
{
   return l_.dependsOn(v) || r_.dependsOn(v);
}

bool ConstraintBin::isEquation() const
{
   return rel_ == RelSymbol::Eq;
}

bool ConstraintBin::isInequality() const
{
   return rel_ == RelSymbol::Ge || rel_ == RelSymbol::Gt ||
          rel_ == RelSymbol::Le || rel_ == RelSymbol::Lt ||
          rel_ == RelSymbol::In;
}

bool ConstraintBin::isLinear() const
{
   return l_.isLinear() && r_.isLinear();
}

///////////////////////////////////////////////////////////////////////////////

ConstraintEq::ConstraintEq(Term l, Term r)
      : ConstraintBin(l, r, RelSymbol::Eq)
{}

void ConstraintEq::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintEq::isSatisfied(const IntervalRegion& reg) const
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator==(Term l, Term r)
{
   return Constraint(std::make_shared<ConstraintEq>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintLe::ConstraintLe(Term l, Term r)
      : ConstraintBin(l, r, RelSymbol::Le)
{}

void ConstraintLe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintLe::isSatisfied(const IntervalRegion& reg) const
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator<=(Term l, Term r)
{
   return Constraint(std::make_shared<ConstraintLe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintLt::ConstraintLt(Term l, Term r)
      : ConstraintBin(l, r, RelSymbol::Lt)
{}

void ConstraintLt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintLt::isSatisfied(const IntervalRegion& reg) const
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator<(Term l, Term r)
{
   return Constraint(std::make_shared<ConstraintLt>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintGe::ConstraintGe(Term l, Term r)
      : ConstraintBin(l, r, RelSymbol::Ge)
{}

void ConstraintGe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintGe::isSatisfied(const IntervalRegion& reg) const
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator>=(Term l, Term r)
{
   return Constraint(std::make_shared<ConstraintGe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintGt::ConstraintGt(Term l, Term r)
      : ConstraintBin(l, r, RelSymbol::Gt)
{}

void ConstraintGt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintGt::isSatisfied(const IntervalRegion& reg) const
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator>(Term l, Term r)
{
   return Constraint(std::make_shared<ConstraintGt>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintIn::ConstraintIn(Term t, const Interval& x)
      : ConstraintBin(t, x, RelSymbol::In), x_(x)
{
   ASSERT(!(x.isEmpty() || x.isUniverse()),
          "Bad interval target in a IN constraint");
}

Interval ConstraintIn::image() const
{
   return x_;
}

Term ConstraintIn::term() const
{
   return left();
}

void ConstraintIn::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintIn::isSatisfied(const IntervalRegion& reg) const
{
   Interval e = term().eval(reg);

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint in(Term t, const Interval& x)
{
   if (x.isSingleton())
      return t == x;

   else if (x.isInfLeft())
      return t <= x.right();

   else if (x.isInfRight())
      return t >= x.left();

   else
      return Constraint(std::make_shared<ConstraintIn>(t.rep(), x));
}

Constraint in(Term t, double a, double b)
{
   return in(t, Interval(a,b));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintVisitor::~ConstraintVisitor()
{}

void ConstraintVisitor::apply(const ConstraintEq* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLe* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLt* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGe* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGt* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintIn* c)
{
   THROW("Visit method not implemented");
}

} // namespace
