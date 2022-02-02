// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/constraint.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const RelSymbol& s)
{
   switch(s)
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

ConstraintRep::ConstraintRep() : scope_(), bs_(), hcode_(0)
{}

ConstraintRep::~ConstraintRep()
{}

void ConstraintRep::setScope(const Scope& s)
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

std::ostream& operator<<(std::ostream& os, const Constraint& c)
{
   c.print(os);
   return os;
}

///////////////////////////////////////////////////////////////////////////////

ConstraintBin::ConstraintBin(const Term& l, const Term& r,
                                   RelSymbol rel) :
   ConstraintRep(), l_(l), r_(r), rel_(rel)
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

void ConstraintBin::print(std::ostream& os) const
{
   l_.print(os);
   os << " " << rel_ << " ";
   r_.print(os);
}

bool ConstraintBin::dependsOn(const Variable& v) const
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

ConstraintEq::ConstraintEq(const Term& l, const Term& r) :
   ConstraintBin(l, r, RelSymbol::Eq)
{}

void ConstraintEq::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintEq::isSat(const IntervalVector& X) const
{
   Interval l = left().eval(X),
            r = right().eval(X);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator==(const Term& l, const Term& r)
{
   return Constraint(std::make_shared<ConstraintEq>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintLe::ConstraintLe(const Term& l, const Term& r) :
   ConstraintBin(l, r, RelSymbol::Le)
{}

void ConstraintLe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintLe::isSat(const IntervalVector& X) const
{
   Interval l = left().eval(X),
            r = right().eval(X);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator<=(const Term& l, const Term& r)
{
   return Constraint(std::make_shared<ConstraintLe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintLt::ConstraintLt(const Term& l, const Term& r) :
   ConstraintBin(l, r, RelSymbol::Lt)
{}

void ConstraintLt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintLt::isSat(const IntervalVector& X) const
{
   Interval l = left().eval(X),
            r = right().eval(X);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator<(const Term& l, const Term& r)
{
   return Constraint(std::make_shared<ConstraintLt>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintGe::ConstraintGe(const Term& l, const Term& r) :
   ConstraintBin(l, r, RelSymbol::Ge)
{}

void ConstraintGe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintGe::isSat(const IntervalVector& X) const
{
   Interval l = left().eval(X),
            r = right().eval(X);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator>=(const Term& l, const Term& r)
{
   return Constraint(std::make_shared<ConstraintGe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintGt::ConstraintGt(const Term& l, const Term& r) :
   ConstraintBin(l, r, RelSymbol::Gt)
{}

void ConstraintGt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintGt::isSat(const IntervalVector& X) const
{
   Interval l = left().eval(X),
            r = right().eval(X);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint operator>(const Term& l, const Term& r)
{
   return Constraint(std::make_shared<ConstraintGt>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintIn::ConstraintIn(const Term& t, const Interval& x):
   ConstraintBin(t, x, RelSymbol::In), x_(x)
{
   ASSERT(!(x.isEmpty() || x.isUniverse()),
          "bad interval target in an 'in' constraint");
}

void ConstraintIn::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ConstraintIn::isSat(const IntervalVector& X) const
{
   Interval e = term().eval(X);

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

Constraint in(const Term& l, const Interval& x)
{
   if (x.isSingleton())
      return l == x;

   else if (x.isInfLeft())
      return l <= x.right();

   else if (x.isInfRight())
      return l >= x.left();

   else
      return Constraint(std::make_shared<ConstraintIn>(l.rep(), x));
}

Constraint in(const Term& l, const double& a, const double& b)
{
   return in(l, Interval(a,b));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintVisitor::~ConstraintVisitor()
{}

void ConstraintVisitor::apply(const ConstraintEq* c)
{
   THROW("visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLe* c)
{
   THROW("visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintLt* c)
{
   THROW("visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGe* c)
{
   THROW("visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintGt* c)
{
   THROW("visit method not implemented");
}

void ConstraintVisitor::apply(const ConstraintIn* c)
{
   THROW("visit method not implemented");
}

} // namespace
