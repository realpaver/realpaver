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
 * @file   ConstraintArithmetic.cpp
 * @brief  Arithmetic constraints
 * @author Laurent Granvilliers
 * @date   29 May 2025
 */

#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintVisitor.hpp"
#include "realpaver/FlatFunction.hpp"
#include "realpaver/TermLin.hpp"

namespace realpaver {

ConstraintBinary::ConstraintBinary(Term l, Term r, ConstraintRelation rel)
    : ConstraintRep(rel)
    , fun_(nullptr)
    , l_(l)
    , r_(r)
{
   hcode_ = static_cast<size_t>(rel);
   hcode_ = hash2(l.hashCode(), hcode_);
   hcode_ = hash2(r.hashCode(), hcode_);

   Scope s;
   l.makeScope(s);
   r.makeScope(s);
   setScope(s);
}

ConstraintBinary::~ConstraintBinary()
{
}

Term ConstraintBinary::left() const
{
   return l_;
}

Term ConstraintBinary::right() const
{
   return r_;
}

std::shared_ptr<FlatFunction> ConstraintBinary::flatFunction() const
{
   return fun_;
}

bool ConstraintBinary::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

void ConstraintBinary::print(std::ostream &os) const
{
   l_.print(os);
   os << " " << relation() << " ";
   r_.print(os);
}

bool ConstraintBinary::isEquation() const
{
   return relation() == ConstraintRelation::Eq;
}

bool ConstraintBinary::isInequality() const
{
   ConstraintRelation rel = relation();

   return rel == ConstraintRelation::Ge || rel == ConstraintRelation::Gt ||
          rel == ConstraintRelation::Le || rel == ConstraintRelation::Lt ||
          rel == ConstraintRelation::In;
}

bool ConstraintBinary::isLinear() const
{
   return l_.isLinear() && r_.isLinear();
}

bool ConstraintBinary::isBoundConstraint() const
{
   return (l_.isVar() && r_.isNumber()) || (l_.isNumber() && r_.isVar());
}

bool ConstraintBinary::isInteger() const
{
   return l_.isInteger() && r_.isInteger();
}

Proof ConstraintBinary::contract(IntervalBox &B)
{
   return fun_->hc4Revise(B);
}

Proof ConstraintBinary::contract(DomainBox &box)
{
   IntervalBox B(box);
   Proof proof = contract(B);

   for (const auto &v : scope())
      box.get(v)->contract(B.get(v));

   return proof;
}

Term ConstraintBinary::simplify(Term t)
{
   try
   {
      TermLin tl(t);
      return tl.toTerm();
   }
   catch (Exception &e)
   {
      return t;
   }
}

/*----------------------------------------------------------------------------*/

ConstraintEq::ConstraintEq(Term l, Term r)
    : ConstraintBinary(l, r, ConstraintRelation::Eq)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::zero());
}

void ConstraintEq::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintEq::isSatisfied(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintEq::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyEq(r))
      return 0.0;

   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

Proof ConstraintEq::isSatisfied(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintEq::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyEq(r))
      return 0.0;

   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

ConstraintRep *ConstraintEq::cloneRoot() const
{
   return new ConstraintEq(left(), right());
}

Constraint operator==(Term l, Term r)
{
   Term rew = ConstraintBinary::simplify(l - r);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << l << " == " << r);
   }
   return Constraint(std::make_shared<ConstraintEq>(rew, Term(0.0)));
}

/*----------------------------------------------------------------------------*/

ConstraintLe::ConstraintLe(Term l, Term r)
    : ConstraintBinary(l, r, ConstraintRelation::Le)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::negative());
}

void ConstraintLe::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintLe::isSatisfied(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintLe::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLe(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Proof ConstraintLe::isSatisfied(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintLe::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLe(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

ConstraintRep *ConstraintLe::cloneRoot() const
{
   return new ConstraintLe(left(), right());
}

Constraint operator<=(Term l, Term r)
{
   Term rew = ConstraintBinary::simplify(l - r);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << l << " <= " << r);
   }
   return Constraint(std::make_shared<ConstraintLe>(rew, Term(0.0)));
}

/*----------------------------------------------------------------------------*/

ConstraintLt::ConstraintLt(Term l, Term r)
    : ConstraintBinary(l, r, ConstraintRelation::Lt)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::negative());
}

void ConstraintLt::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintLt::isSatisfied(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintLt::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLt(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Proof ConstraintLt::isSatisfied(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintLt::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLt(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

ConstraintRep *ConstraintLt::cloneRoot() const
{
   return new ConstraintLt(left(), right());
}

Constraint operator<(Term l, Term r)
{
   Term rew = ConstraintBinary::simplify(l - r);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << l << " < " << r);
   }
   return Constraint(std::make_shared<ConstraintLt>(rew, Term(0.0)));
}

/*----------------------------------------------------------------------------*/

ConstraintGe::ConstraintGe(Term l, Term r)
    : ConstraintBinary(l, r, ConstraintRelation::Ge)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::positive());
}

void ConstraintGe::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintGe::isSatisfied(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintGe::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGe(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Proof ConstraintGe::isSatisfied(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintGe::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGe(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

ConstraintRep *ConstraintGe::cloneRoot() const
{
   return new ConstraintGe(left(), right());
}

Constraint operator>=(Term l, Term r)
{
   Term rew = ConstraintBinary::simplify(l - r);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << l << " >= " << r);
   }
   return Constraint(std::make_shared<ConstraintGe>(rew, Term(0.0)));
}

/*----------------------------------------------------------------------------*/

ConstraintGt::ConstraintGt(Term l, Term r)
    : ConstraintBinary(l, r, ConstraintRelation::Gt)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::positive());
}

void ConstraintGt::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintGt::isSatisfied(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintGt::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGt(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Proof ConstraintGt::isSatisfied(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintGt::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGt(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

ConstraintRep *ConstraintGt::cloneRoot() const
{
   return new ConstraintGt(left(), right());
}

Constraint operator>(Term l, Term r)
{
   Term rew = ConstraintBinary::simplify(l - r);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << l << " > " << r);
   }
   return Constraint(std::make_shared<ConstraintGt>(rew, Term(0.0)));
}

/*----------------------------------------------------------------------------*/

ConstraintIn::ConstraintIn(Term t, const Interval &x)
    : ConstraintBinary(t, x, ConstraintRelation::In)
    , x_(x)
{
   REALPAVER_ASSERT(!(x.isEmpty() || x.isUniverse()),
                    "Bad interval target in a IN constraint");

   fun_ = std::make_shared<FlatFunction>(t, x);
}

Interval ConstraintIn::image() const
{
   return x_;
}

Term ConstraintIn::term() const
{
   return left();
}

void ConstraintIn::print(std::ostream &os) const
{
   os << left() << " in " << x_;
}

void ConstraintIn::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ConstraintIn::isSatisfied(const IntervalBox &B)
{
   Interval e = term().eval(B);

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintIn::violation(const IntervalBox &B)
{
   Interval e = term().eval(B);

   if (e.isEmpty())
      return Double::inf();
   if (x_.overlaps(e))
      return 0.0;

   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

Proof ConstraintIn::isSatisfied(const DomainBox &box)
{
   Interval e = term().eval(box)->intervalHull();

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ConstraintIn::violation(const DomainBox &box)
{
   Interval e = term().eval(box)->intervalHull();

   if (e.isEmpty())
      return Double::inf();
   if (x_.overlaps(e))
      return 0.0;

   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

ConstraintRep *ConstraintIn::cloneRoot() const
{
   return new ConstraintIn(term(), image());
}

Constraint in(Term t, const Interval &x)
{
   if (x.isEmpty())
   {
      THROW("Constraint with an empty interval: " << t << " in " << x);
   }

   Term rew = ConstraintBinary::simplify(t);
   if (rew.isConstant())
   {
      THROW("Bad constraint: " << t << " in " << x);
   }

   if (x.isSingleton())
      return rew == x;

   else if (x.isInfLeft())
      return rew <= x.right();

   else if (x.isInfRight())
      return rew >= x.left();

   else
      return Constraint(std::make_shared<ConstraintIn>(rew, x));
}

Constraint in(Term t, double a, double b)
{
   return in(t, Interval(a, b));
}

} // namespace realpaver
