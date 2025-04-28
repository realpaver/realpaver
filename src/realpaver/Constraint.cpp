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
 * @file   Constraint.cpp
 * @brief  Classes of constraints
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/FlatFunction.hpp"
#include "realpaver/Problem.hpp"
#include "realpaver/ScopeBank.hpp"

#include <numeric>

namespace realpaver {

std::ostream &operator<<(std::ostream &os, RelSymbol rel)
{
   switch (rel)
   {
   case RelSymbol::Eq:
      return os << "==";
   case RelSymbol::Le:
      return os << "<=";
   case RelSymbol::Lt:
      return os << "<";
   case RelSymbol::Ge:
      return os << ">=";
   case RelSymbol::Gt:
      return os << ">";
   case RelSymbol::In:
      return os << "in";
   case RelSymbol::Table:
      return os << "table";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

ConstraintRep::ConstraintRep(RelSymbol rel)
    : scop_()
    , rel_(rel)
    , hcode_(0)
{
}

ConstraintRep::~ConstraintRep()
{
}

size_t ConstraintRep::hashCode() const
{
   return hcode_;
}

Scope ConstraintRep::scope() const
{
   return scop_;
}

void ConstraintRep::setScope(Scope scop)
{
   scop_ = ScopeBank::getInstance()->insertScope(scop);
}

bool ConstraintRep::dependsOn(Variable v) const
{
   return scop_.contains(v);
}

RelSymbol ConstraintRep::relSymbol() const
{
   return rel_;
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

bool ConstraintRep::isBoundConstraint() const
{
   return false;
}

/*----------------------------------------------------------------------------*/

Constraint::Constraint(const SharedRep &rep)
    : rep_(rep)
{
}

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

RelSymbol Constraint::relSymbol() const
{
   return rep_->relSymbol();
}

bool Constraint::isConstant() const
{
   return rep_->isConstant();
}

void Constraint::print(std::ostream &os) const
{
   rep_->print(os);
}

void Constraint::acceptVisitor(ConstraintVisitor &vis) const
{
   rep_->acceptVisitor(vis);
}

Proof Constraint::isSatisfied(const IntervalBox &B)
{
   return rep_->isSatisfied(B);
}

double Constraint::violation(const IntervalBox &B)
{
   return rep_->violation(B);
}

Proof Constraint::contract(IntervalBox &B)
{
   return rep_->contract(B);
}

Proof Constraint::isSatisfied(const DomainBox &box)
{
   return rep_->isSatisfied(box);
}

double Constraint::violation(const DomainBox &box)
{
   return rep_->violation(box);
}

Proof Constraint::contract(DomainBox &box)
{
   return rep_->contract(box);
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

bool Constraint::isBoundConstraint() const
{
   return rep_->isBoundConstraint();
}

bool Constraint::isInteger() const
{
   return rep_->isInteger();
}

std::ostream &operator<<(std::ostream &os, Constraint c)
{
   c.print(os);
   return os;
}

ConstraintRep *Constraint::cloneRoot() const
{
   return rep_->cloneRoot();
}

/*----------------------------------------------------------------------------*/

ArithCtrBinary::ArithCtrBinary(Term l, Term r, RelSymbol rel)
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

ArithCtrBinary::~ArithCtrBinary()
{
}

Term ArithCtrBinary::left() const
{
   return l_;
}

Term ArithCtrBinary::right() const
{
   return r_;
}

std::shared_ptr<FlatFunction> ArithCtrBinary::flatFunction() const
{
   return fun_;
}

bool ArithCtrBinary::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

void ArithCtrBinary::print(std::ostream &os) const
{
   l_.print(os);
   os << " " << relSymbol() << " ";
   r_.print(os);
}

bool ArithCtrBinary::isEquation() const
{
   return relSymbol() == RelSymbol::Eq;
}

bool ArithCtrBinary::isInequality() const
{
   RelSymbol rel = relSymbol();

   return rel == RelSymbol::Ge || rel == RelSymbol::Gt || rel == RelSymbol::Le ||
          rel == RelSymbol::Lt || rel == RelSymbol::In;
}

bool ArithCtrBinary::isLinear() const
{
   return l_.isLinear() && r_.isLinear();
}

bool ArithCtrBinary::isBoundConstraint() const
{
   return (l_.isVar() && r_.isNumber()) || (l_.isNumber() && r_.isVar());
}

bool ArithCtrBinary::isInteger() const
{
   return l_.isInteger() && r_.isInteger();
}

Proof ArithCtrBinary::contract(IntervalBox &B)
{
   return fun_->hc4Revise(B);
}

Proof ArithCtrBinary::contract(DomainBox &box)
{
   IntervalBox B(box);
   Proof proof = contract(B);

   for (const auto &v : scope())
      box.get(v)->contract(B.get(v));

   return proof;
}

/*----------------------------------------------------------------------------*/

ArithCtrEq::ArithCtrEq(Term l, Term r)
    : ArithCtrBinary(l, r, RelSymbol::Eq)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::zero());
}

void ArithCtrEq::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrEq::isSatisfied(const IntervalBox &B)
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

double ArithCtrEq::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyEq(r))
      return 0.0;

   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

Proof ArithCtrEq::isSatisfied(const DomainBox &box)
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

double ArithCtrEq::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyEq(r))
      return 0.0;

   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

Constraint operator==(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrEq>(l.rep(), r.rep()));
}

ConstraintRep *ArithCtrEq::cloneRoot() const
{
   return new ArithCtrEq(left(), right());
}

/*----------------------------------------------------------------------------*/

ArithCtrLe::ArithCtrLe(Term l, Term r)
    : ArithCtrBinary(l, r, RelSymbol::Le)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::negative());
}

void ArithCtrLe::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrLe::isSatisfied(const IntervalBox &B)
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

double ArithCtrLe::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLe(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLe::isSatisfied(const DomainBox &box)
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

double ArithCtrLe::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLe(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Constraint operator<=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLe>(l.rep(), r.rep()));
}

ConstraintRep *ArithCtrLe::cloneRoot() const
{
   return new ArithCtrLe(left(), right());
}

/*----------------------------------------------------------------------------*/

ArithCtrLt::ArithCtrLt(Term l, Term r)
    : ArithCtrBinary(l, r, RelSymbol::Lt)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::negative());
}

void ArithCtrLt::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrLt::isSatisfied(const IntervalBox &B)
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

double ArithCtrLt::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLt(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLt::isSatisfied(const DomainBox &box)
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

double ArithCtrLt::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyLt(r))
      return 0.0;

   Double::rndNear();
   return l.left() - r.right();
}

Constraint operator<(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLt>(l.rep(), r.rep()));
}

ConstraintRep *ArithCtrLt::cloneRoot() const
{
   return new ArithCtrLt(left(), right());
}

/*----------------------------------------------------------------------------*/

ArithCtrGe::ArithCtrGe(Term l, Term r)
    : ArithCtrBinary(l, r, RelSymbol::Ge)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::positive());
}

void ArithCtrGe::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrGe::isSatisfied(const IntervalBox &B)
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

double ArithCtrGe::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGe(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGe::isSatisfied(const DomainBox &box)
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

double ArithCtrGe::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGe(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Constraint operator>=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGe>(l.rep(), r.rep()));
}

ConstraintRep *ArithCtrGe::cloneRoot() const
{
   return new ArithCtrGe(left(), right());
}

/*----------------------------------------------------------------------------*/

ArithCtrGt::ArithCtrGt(Term l, Term r)
    : ArithCtrBinary(l, r, RelSymbol::Gt)
{
   fun_ = std::make_shared<FlatFunction>(l - r, Interval::positive());
}

void ArithCtrGt::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrGt::isSatisfied(const IntervalBox &B)
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

double ArithCtrGt::violation(const IntervalBox &B)
{
   Interval l = left().eval(B), r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGt(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGt::isSatisfied(const DomainBox &box)
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

double ArithCtrGt::violation(const DomainBox &box)
{
   Interval l = left().eval(box)->intervalHull(), r = right().eval(box)->intervalHull();

   if (l.isEmpty() || r.isEmpty())
      return Double::inf();
   if (l.isPossiblyGt(r))
      return 0.0;

   Double::rndNear();
   return r.left() - l.right();
}

Constraint operator>(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGt>(l.rep(), r.rep()));
}

ConstraintRep *ArithCtrGt::cloneRoot() const
{
   return new ArithCtrGt(left(), right());
}

/*----------------------------------------------------------------------------*/

ArithCtrIn::ArithCtrIn(Term t, const Interval &x)
    : ArithCtrBinary(t, x, RelSymbol::In)
    , x_(x)
{
   ASSERT(!(x.isEmpty() || x.isUniverse()), "Bad interval target in a IN constraint");

   fun_ = std::make_shared<FlatFunction>(t, x);
}

Interval ArithCtrIn::image() const
{
   return x_;
}

Term ArithCtrIn::term() const
{
   return left();
}

void ArithCtrIn::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Proof ArithCtrIn::isSatisfied(const IntervalBox &B)
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

double ArithCtrIn::violation(const IntervalBox &B)
{
   Interval e = term().eval(B);

   if (e.isEmpty())
      return Double::inf();
   if (x_.overlaps(e))
      return 0.0;

   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

Constraint in(Term t, const Interval &x)
{
   if (x.isSingleton())
      return t == x;

   else if (x.isInfLeft())
      return t <= x.right();

   else if (x.isInfRight())
      return t >= x.left();

   else
      return Constraint(std::make_shared<ArithCtrIn>(t.rep(), x));
}

Proof ArithCtrIn::isSatisfied(const DomainBox &box)
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

double ArithCtrIn::violation(const DomainBox &box)
{
   Interval e = term().eval(box)->intervalHull();

   if (e.isEmpty())
      return Double::inf();
   if (x_.overlaps(e))
      return 0.0;

   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

Constraint in(Term t, double a, double b)
{
   return in(t, Interval(a, b));
}

ConstraintRep *ArithCtrIn::cloneRoot() const
{
   return new ArithCtrIn(term(), image());
}

/*----------------------------------------------------------------------------*/

TableCtrCol::TableCtrCol(Variable v)
    : v_(v)
    , vval_()
{
}

TableCtrCol::TableCtrCol(Variable v, const std::initializer_list<Interval> &l)
    : v_(v)
    , vval_(l)
{
}

size_t TableCtrCol::size() const
{
   return vval_.size();
}

Variable TableCtrCol::getVar() const
{
   return v_;
}

void TableCtrCol::addValue(const Interval &x)
{
   vval_.push_back(x);
}

Interval TableCtrCol::getVal(size_t i) const
{
   ASSERT(i < size(), "Bad access in a column of a table constraint @ " << i);
   return vval_[i];
}

bool TableCtrCol::isInteger() const
{
   return v_.getDomain()->isInteger();
}

/*----------------------------------------------------------------------------*/

TableCtr::TableCtr()
    : ConstraintRep(RelSymbol::Table)
    , vcol_()
{
}

TableCtr::TableCtr(const std::initializer_list<TableCtrCol> &l)
    : ConstraintRep(RelSymbol::Table)
    , vcol_(l)
{
   ASSERT(nbCols() > 0, "Bad initialization of a constraint table");
   ASSERT(nbRows() > 0, "Bad initialization of a constraint table");

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

TableCtr::TableCtr(const std::initializer_list<Variable> &vars,
                   const std::initializer_list<Interval> &values)
    : ConstraintRep(RelSymbol::Table)
    , vcol_()
{
   size_t nbvar = vars.size();
   size_t nbitv = values.size();

   ASSERT(nbvar > 0 && nbitv > nbvar && (nbitv % nbvar == 0),
          "Bad initialization of a constraint table");

   // creates the columns
   for (auto itv = vars.begin(); itv != vars.end(); ++itv)
   {
      TableCtrCol col(*itv);
      vcol_.push_back(col);
   }

   auto itv = values.begin();
   size_t nrows = nbitv / nbvar;

   for (size_t i = 0; i < nrows; ++i)
   {
      for (size_t j = 0; j < nbvar; ++j, ++itv)
      {
         Interval x = *itv;
         if (x.isEmpty())
            THROW("Bad initialization of a constraint table: " << x);

         vcol_[j].addValue(x);
      }
   }

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

void TableCtr::makeScopeAndHashCode()
{
   Scope s;
   hcode_ = static_cast<size_t>(RelSymbol::Table);

   for (auto &col : vcol_)
   {
      Variable v = col.getVar();
      s.insert(v);
      hcode_ = hash2(v.hashCode(), hcode_);
   }

   setScope(s);
}

bool TableCtr::isInteger() const
{
   for (auto &col : vcol_)
      if (!col.isInteger())
         return false;

   return true;
}

size_t TableCtr::nbCols() const
{
   return vcol_.size();
}

size_t TableCtr::nbRows() const
{
   return vcol_.empty() ? 0 : vcol_[0].size();
}

Variable TableCtr::getVar(size_t j) const
{
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j].getVar();
}

Interval TableCtr::getVal(size_t i, size_t j) const
{
   ASSERT(i < nbRows(), "Bad access to a row in a table constraint @ " << i);
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j].getVal(i);
}

TableCtrCol TableCtr::getCol(size_t j) const
{
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j];
}

void TableCtr::addCol(const TableCtrCol &col)
{
   ASSERT(vcol_.empty() || col.size() == nbRows(),
          "Bad insertion of a new column in a table constraint");

   ASSERT(!dependsOn(col.getVar()),
          "Variable already present in a table constraint: " << col.getVar().getName());

   vcol_.push_back(col);

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

bool TableCtr::isConstant() const
{
   return vcol_.empty();
}

bool TableCtr::isRowConsistent(size_t i, const IntervalBox &B) const
{
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      if (B.get(v).isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

bool TableCtr::isRowConsistent(size_t i, const DomainBox &box) const
{
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      if (box.get(v)->intervalHull().isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

Proof TableCtr::isSatisfied(const IntervalBox &B)
{
   size_t nbc = 0;

   for (size_t i = 0; i < nbRows(); ++i)
      if (isRowConsistent(i, B))
      {
         ++nbc;
         if (nbc > 1)
            return Proof::Maybe;
      }

   return (nbc == 1) ? Proof::Inner : Proof::Empty;
}

double TableCtr::violation(const IntervalBox &B)
{
   double res = Double::inf();

   for (size_t i = 0; i < nbRows(); ++i)
      res = Double::min(res, rowViolation(B, i));

   return res;
}

double TableCtr::rowViolation(const IntervalBox &B, size_t i)
{
   double res = 0.0;
   Double::rndNear();

   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      Interval val = vcol_[j].getVal(i);
      Interval dom = B.get(v);

      double viol = 0.0;

      if (dom.isCertainlyLt(val))
         viol = val.left() - dom.right();

      else if (dom.isCertainlyGt(val))
         viol = dom.left() - val.right();

      if (viol > res)
         res = viol;
   }

   return res;
}

double TableCtr::rowViolation(const DomainBox &box, size_t i)
{
   double res = 0.0;
   Double::rndNear();

   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      Interval val = vcol_[j].getVal(i);
      Interval dom = box.get(v)->intervalHull();

      double viol = 0.0;

      if (dom.isCertainlyLt(val))
         viol = val.left() - dom.right();

      else if (dom.isCertainlyGt(val))
         viol = dom.left() - val.right();

      if (viol > res)
         res = viol;
   }

   return res;
}

Proof TableCtr::contract(IntervalBox &B)
{
   Bitset consistent(nbRows());
   consistent.setAllOne();
   size_t nbc = nbRows(); // number of consistent rows

   // checks consistency
   for (size_t i = 0; i < nbRows(); ++i)
      if (!isRowConsistent(i, B))
      {
         consistent.setZero(i);
         --nbc;
      }

   if (nbc == 0)
      return Proof::Empty;

   // contracts the domains
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();

      // hull of values of this variable occurring in the consistent rows
      Interval h = Interval::emptyset();
      for (size_t i = 0; i < nbRows(); ++i)
         if (consistent.get(i))
            h |= vcol_[j].getVal(i);

      Interval x = h & B.get(v);
      if (x.isEmpty())
         return Proof::Empty;
      B.set(v, x);
   }

   return (nbc == 1) ? Proof::Inner : Proof::Maybe;
}

Proof TableCtr::isSatisfied(const DomainBox &box)
{
   size_t nbc = 0;

   for (size_t i = 0; i < nbRows(); ++i)
      if (isRowConsistent(i, box))
      {
         ++nbc;
         if (nbc > 1)
            return Proof::Maybe;
      }

   return (nbc == 1) ? Proof::Inner : Proof::Empty;
}

double TableCtr::violation(const DomainBox &box)
{
   double res = Double::inf();

   for (size_t i = 0; i < nbRows(); ++i)
      res = Double::min(res, rowViolation(box, i));

   return res;
}

Proof TableCtr::contract(DomainBox &box)
{
   Bitset consistent(nbRows());
   consistent.setAllOne();
   size_t nbc = nbRows(); // number of consistent rows

   // checks consistency
   for (size_t i = 0; i < nbRows(); ++i)
      if (!isRowConsistent(i, box))
      {
         consistent.setZero(i);
         --nbc;
      }

   if (nbc == 0)
      return Proof::Empty;

   // contracts the domains
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();

      // hull of values of this variable occurring in the consistent rows
      Interval x = Interval::emptyset();
      for (size_t i = 0; i < nbRows(); ++i)
         if (consistent.get(i))
            x |= vcol_[j].getVal(i);

      Domain *domain = box.get(v);
      domain->contract(x);
      if (domain->isEmpty())
         return Proof::Empty;
   }

   return (nbc == 1) ? Proof::Inner : Proof::Maybe;
}

void TableCtr::print(std::ostream &os) const
{
   os << "table(";
   // prints the variables
   os << "{";
   for (size_t i = 0; i < nbCols(); ++i)
   {
      if (i > 0)
         os << ", ";
      os << vcol_[i].getVar().getName();
   }
   os << "}, ";

   // prints the rows
   os << "{";
   for (size_t j = 0; j < nbRows(); ++j)
   {
      if (j > 0)
         os << ", ";
      for (size_t i = 0; i < nbCols(); ++i)
      {
         if (i > 0)
            os << ", ";
         os << vcol_[i].getVal(j);
      }
   }
   os << "})";
}

void TableCtr::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

Constraint table(const std::initializer_list<Variable> &vars,
                 const std::initializer_list<Interval> &values)
{
   return Constraint(std::make_shared<TableCtr>(vars, values));
}

Constraint table(const Variable *vars, size_t nvars, const Interval *values,
                 size_t nvalues)
{
   if (nvars == 0 || nvalues % nvars != 0)
      THROW("Bad initialization of a table constraint");

   size_t nrows = nvalues / nvars;

   Constraint::SharedRep srep = std::make_shared<TableCtr>();
   TableCtr *rep = static_cast<TableCtr *>(srep.get());

   for (size_t j = 0; j < nvars; ++j)
   {
      TableCtrCol col(vars[j]);
      for (size_t i = 0; i < nrows; ++i)
      {
         Interval x = values[i * nvars + j];
         col.addValue(x);
      }
      rep->addCol(col);
   }
   return Constraint(srep);
}

ConstraintRep *TableCtr::cloneRoot() const
{
   return new TableCtr(*this);
}

/*----------------------------------------------------------------------------*/

CondCtr::CondCtr(Constraint guard, Constraint body)
    : ConstraintRep(RelSymbol::Cond)
    , guard_(guard)
    , body_(body)
{
   THROW_IF(!(guard.isInteger() || guard.isInequality()),
            "Bad guard of conditional constraint: " << guard);

   hcode_ = guard.hashCode();
   hcode_ = hash2(body.hashCode(), hcode_);

   Scope scop;
   scop.insert(guard.scope());
   scop.insert(body.scope());
   setScope(scop);
}

Constraint CondCtr::guard() const
{
   return guard_;
}

Constraint CondCtr::body() const
{
   return body_;
}

bool CondCtr::isConstant() const
{
   return guard_.isConstant() && body_.isConstant();
}

Proof CondCtr::isSatisfied(const IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.isSatisfied(B);

   return p;
}

double CondCtr::violation(const IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);
   return (p == Proof::Inner) ? body_.violation(B) : 0.0;
}

Proof CondCtr::contract(IntervalBox &B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.contract(B);

   return p;
}

Proof CondCtr::isSatisfied(const DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.isSatisfied(box);

   return p;
}

double CondCtr::violation(const DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);
   return (p == Proof::Inner) ? body_.violation(box) : 0.0;
}

Proof CondCtr::contract(DomainBox &box)
{
   Proof p = guard_.isSatisfied(box);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.contract(box);

   return p;
}

void CondCtr::print(std::ostream &os) const
{
   os << guard_ << " -> " << body_;
}

void CondCtr::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

ConstraintRep *CondCtr::cloneRoot() const
{
   return new CondCtr(guard_, body_);
}

bool CondCtr::isInteger() const
{
   return guard_.isInteger() && body_.isInteger();
}

Constraint cond(Constraint guard, Constraint body)
{
   return Constraint(std::make_shared<CondCtr>(guard, body));
}

/*----------------------------------------------------------------------------*/

PiecewiseCtr::PiecewiseCtr(Variable v, const std::initializer_list<Variable> &binaries,
                           const std::initializer_list<Interval> &intervals,
                           const std::initializer_list<Constraint> &constraints)
    : ConstraintRep(RelSymbol::Piecewise)
    , v_(v)
    , binaries_(binaries)
    , intervals_(intervals)
    , constraints_()
    , guard_hull_(Interval::emptyset())
{
   ASSERT(binaries.size() == intervals.size() && intervals.size() == constraints.size(),
          "Error: inconsistent number of pieces");
   hcode_ = 0;
   Scope scop;
   scop.insert(v_);
   for (auto b : binaries_)
   {
      ASSERT(b.isBinary(), "Error a variable is not binary");
      scop.insert(b);
   }
   for (auto iv : intervals_)
   {
      guard_hull_ |= iv;
   }
   // creates the guards and bodies
   for (auto it = constraints.begin(); it != constraints.end(); ++it)
   {
      Constraint ctr(*it);
      constraints_.push_back(ctr);
      hcode_ = hash2(ctr.hashCode(), hcode_);
      scop.insert(ctr.scope());
   }
   setScope(scop);
}

PiecewiseCtr::PiecewiseCtr(Variable v, const std::vector<Variable> &binaries,
                           const std::vector<Interval> &intervals,
                           const std::vector<Constraint> &constraints)
    : ConstraintRep(RelSymbol::Piecewise)
    , v_(v)
    , binaries_(binaries)
    , intervals_(intervals)
    , constraints_()
    , guard_hull_(Interval::emptyset())
{
   ASSERT(binaries.size() == intervals.size() && intervals.size() == constraints.size(),
          "Error: inconsistent number of pieces");
   hcode_ = 0;
   Scope scop;
   scop.insert(v_);
   for (auto b : binaries_)
   {
      ASSERT(b.isBinary(), "Error a variable is not binary");
      scop.insert(b);
   }
   for (auto iv : intervals_)
   {
      guard_hull_ |= iv;
   }
   // creates the guards and bodies
   for (auto it = constraints.begin(); it != constraints.end(); ++it)
   {
      Constraint ctr(*it);
      constraints_.push_back(ctr);
      hcode_ = hash2(ctr.hashCode(), hcode_);
      scop.insert(ctr.scope());
   }
   setScope(scop);
}

const Variable &PiecewiseCtr::variable() const
{
   return v_;
}

const std::vector<Variable> &PiecewiseCtr::binaries() const
{
   return binaries_;
}

const Variable &PiecewiseCtr::binary(size_t i) const
{
   return binaries_[i];
}

const std::vector<Interval> &PiecewiseCtr::intervals() const
{
   return intervals_;
}

const Interval &PiecewiseCtr::interval(size_t i) const
{
   return intervals_[i];
}

const std::vector<Constraint> &PiecewiseCtr::constraints() const
{
   return constraints_;
}

const Constraint &PiecewiseCtr::constraint(size_t i) const
{
   return constraints_[i];
}

size_t PiecewiseCtr::nb_pieces() const
{
   return intervals_.size();
}

bool PiecewiseCtr::isConstant() const
{
   bool is_constant = true;
   size_t i = 0;
   while (is_constant && i < constraints_.size())
   {
      is_constant &= constraints_[i].isConstant();
      i++;
   }
   return is_constant;
}

Proof PiecewiseCtr::isSatisfied(const IntervalBox &B)
{
   Interval iv = B.get(v_);
   Proof p_c = Proof::Maybe;
   if (!guard_hull_.contains(iv))
      p_c = Proof::Empty;

   // Check cases status:
   size_t nb_active; // count the number of active intervals and binary variables (should
                     // be <=1)
   size_t nb_possibly_active;
   size_t num_active;  // store the index of the active interval and binary variable
   size_t nb_inactive; // count the number of inactive intervals and binary variables
   checkActive(B, nb_active, nb_possibly_active, nb_inactive, num_active);

   if (nb_active > 1)
   {
      return Proof::Empty;
   }
   if (nb_inactive == nb_pieces())
   {
      return Proof::Empty;
   }
   if (num_active != nb_pieces())
      p_c = constraints_[num_active].isSatisfied(B);

   return p_c;
}

double PiecewiseCtr::violation(const IntervalBox &B)
{
   double violation = 0.0;
   Interval iv = B.get(v_);
   bool active = false;
   for (size_t i = 0; i < intervals_.size(); i++)
   {
      const Interval b_val = binaries_[i].getDomain()->intervalHull();
      Interval iv2 = intervals_[i] & iv;
      if (!iv2.isEmpty() && b_val.isOne())
      {
         active = true;
         IntervalBox Bi(B);
         violation += constraints_[i].violation(Bi);
      }
      else if (iv.isEmpty() && b_val.isOne())
      {
         active = true;
         violation += intervals_[i].width();
      }
   }
   if (!active)
   {
      violation = guard_hull_.width();
   }
   return violation;
}

void PiecewiseCtr::checkActive(const IntervalBox &B, size_t &nb_active,
                               size_t &nb_possibly_active, size_t &nb_inactive,
                               size_t &num_active) const
{
   Interval iv = B.get(v_);
   // Search for active guards, i.e. intervals and binary variables
   nb_active =
       0; // count the number of active intervals and binary variables (should be <=1)
   nb_possibly_active = 0;
   num_active = nb_pieces(); // store the index of the active interval and binary variable
   nb_inactive = 0; // count the number of inactive intervals and binary variables
   for (size_t i = 0; i < nb_pieces(); i++)
   {
      // intersect ith interval with current box interval for v_
      Interval iv2 = intervals_[i] & iv;
      Interval b_dom = B.get(binaries_[i]);
      if (b_dom.left() == 1 && b_dom.right() == 1 && !iv2.isEmpty()) // Active case
      {
         if (nb_active > 0) // Inconsistency: more than 1 active binary variable
         {
            nb_active = nb_pieces();
            nb_possibly_active = nb_pieces();
            nb_inactive = nb_pieces();
            num_active = nb_pieces();
            return;
         }
         nb_active++;
         num_active = i;
      }
      else if (b_dom.left() == 0 && b_dom.right() == 1 && !iv2.isEmpty()) // Unknown case
      {
         nb_possibly_active++;
         if (nb_active == 0) // store in num_active the last possible activable piece
            num_active = i;
      }
      else if (b_dom.left() == 1 && b_dom.right() == 1 &&
               iv2.isEmpty()) // Inconsistency: interval empty but binary active
      {
         nb_active = nb_pieces();
         nb_possibly_active = nb_pieces();
         nb_inactive = nb_pieces();
         num_active = nb_pieces();
         return;
      }
      else if ((b_dom.left() == 0 && b_dom.right() == 0) ||
               iv2.isEmpty()) // Inactive case
         nb_inactive++;
   }
}

Proof PiecewiseCtr::contract(IntervalBox &B)
{
   Interval iv = B.get(v_);
   if (guard_hull_.isDisjoint(iv))
      return Proof::Empty; /// Inconsistent case: no contraint can be applied
   // Force the domain of v to be included in guards hull
   if (iv.strictlyContains(guard_hull_))
   {
      iv &= guard_hull_;
   }

   // Check cases status:
   size_t nb_active; // count the number of active intervals and binary variables (should
                     // be <=1)
   size_t nb_possibly_active;
   size_t num_active;  // store the index of the active interval and binary variable
   size_t nb_inactive; // count the number of inactive intervals and binary variables
   checkActive(B, nb_active, nb_possibly_active, nb_inactive, num_active);

   if (nb_active > 1 || num_active == nb_pieces())
      return Proof::Empty; /// Inconsistent case!
   else if ((nb_active == 0 && nb_possibly_active == 0) || nb_inactive == nb_pieces())
      return Proof::Empty;

   // Only a single case active
   if (nb_active == 1)
   {
      if (nb_possibly_active > 0) // Prune undetermined binaries to 0
      {
         for (size_t i = 0; i < nb_pieces(); i++)
         {
            if (i != num_active)
               B.set(binaries_[i], Interval::zero());
         }
      }
      iv &= intervals_[num_active];
      B.set(v_, iv); // Prune v_ domain to be sure, useless?
      return constraints_[num_active].contract(B);
   }
   else // nb_active==0 && nb_possibly_active>0
   {
      if (nb_possibly_active == 1) // domain of v_ intersect a single piece where binary
                                   // variable is undetermined
      {
         iv &= intervals_[num_active];
         B.set(v_, iv);
         B.set(binaries_[num_active], Interval::one());
         return constraints_[num_active].contract(B);
      }
   }

   return Proof::Maybe;
}

Proof PiecewiseCtr::isSatisfied(const DomainBox &box)
{
   Interval iv = box.get(v_)->intervalHull();
   Proof p_c = Proof::Maybe;
   if (!guard_hull_.contains(iv))
      p_c = Proof::Empty;

   // Check cases status:
   size_t nb_active; // count the number of active intervals and binary variables (should
                     // be <=1)
   size_t nb_possibly_active;
   size_t num_active;  // store the index of the active interval and binary variable
   size_t nb_inactive; // count the number of inactive intervals and binary variables
   checkActive(box, nb_active, nb_possibly_active, nb_inactive, num_active);

   if (nb_active > 1)
   {
      return Proof::Empty;
   }
   if (nb_inactive == nb_pieces())
   {
      return Proof::Empty;
   }
   if (num_active != nb_pieces())
      p_c = constraints_[num_active].isSatisfied(box);

   return p_c;
}

double PiecewiseCtr::violation(const DomainBox &box)
{
   double violation = 0.0;
   Interval iv = box.get(v_)->intervalHull();
   bool active = false;
   for (size_t i = 0; i < intervals_.size(); i++)
   {
      const Interval b_val = binaries_[i].getDomain()->intervalHull();
      Interval iv2 = intervals_[i] & iv;
      if (!iv2.isEmpty() && b_val.isOne())
      {
         active = true;
         IntervalBox Bi(box); // Bi.set(v_,iv2);
         violation += constraints_[i].violation(Bi);
      }
      else if (iv.isEmpty() && b_val.isOne())
      {
         active = true;
         violation += intervals_[i].width();
      }
      if (!active)
      {
         violation = guard_hull_.width();
      }
   }
   return violation;
}

void PiecewiseCtr::checkActive(const DomainBox &B, size_t &nb_active,
                               size_t &nb_possibly_active, size_t &nb_inactive,
                               size_t &num_active) const
{
   Interval iv = B.get(v_)->intervalHull();
   // Search for active guards, i.e. intervals and binary variables
   nb_active =
       0; // count the number of active intervals and binary variables (should be <=1)
   nb_possibly_active = 0;
   num_active = nb_pieces(); // store the index of the active interval and binary variable
   nb_inactive = 0; // count the number of inactive intervals and binary variables
   for (size_t i = 0; i < nb_pieces(); i++)
   {
      // intersect ith interval with current box interval for v_
      Interval iv2 = intervals_[i] & iv;
      const Interval b_val = B.get(binaries_[i])->intervalHull();
      if (b_val.isOne() && !iv2.isEmpty())
      {
         if (nb_active > 0) // Inconsistency: more than 1 active binary variable
         {
            nb_active = nb_pieces();
            nb_possibly_active = nb_pieces();
            nb_inactive = nb_pieces();
            num_active = nb_pieces();
            return;
         }
         nb_active++;
         num_active = i;
      }
      else if (!b_val.isSingleton() && !iv2.isEmpty())
      {
         nb_possibly_active++;
         if (nb_active == 0) // store in num_active the last possible activable piece
            num_active = i;
      }
      else if (b_val.isOne() && iv2.isEmpty())
      { /// Inconsistent case!
         nb_active = nb_pieces();
         nb_possibly_active = nb_pieces();
         nb_inactive = nb_pieces();
         num_active = nb_pieces();
         return;
      }
      else if (b_val.isZero() || iv2.isEmpty())
         nb_inactive++;
   }
}

Proof PiecewiseCtr::contract(DomainBox &box)
{
   Interval iv = box.get(v_)->intervalHull();
   if (guard_hull_.isDisjoint(iv))
   {
      return Proof::Empty; /// Inconsistent case: no contraint can be applieds
   }
   // Force the domain of v to be included in guards hull
   if (iv.strictlyContains(guard_hull_))
   {
      iv &= guard_hull_;
      box.get(v_)->contractInterval(iv);
   }

   // Check cases status:
   size_t nb_active; // count the number of active intervals and binary variables (should
                     // be <=1)
   size_t nb_possibly_active;
   size_t num_active;  // store the index of the active interval and binary variable
   size_t nb_inactive; // count the number of inactive intervals and binary variables
   checkActive(box, nb_active, nb_possibly_active, nb_inactive, num_active);

   if (nb_active > 1 || num_active == nb_pieces())
   {
      return Proof::Empty; /// Inconsistent case!
   }
   else if ((nb_active == 0 && nb_possibly_active == 0) || nb_inactive == nb_pieces())
   {
      return Proof::Empty;
   }

   // Only a single case active
   if (nb_active == 1)
   {
      if (nb_possibly_active > 0) // Prune undetermined binaries to 0
      {
         for (size_t i = 0; i < nb_pieces(); i++)
         {
            if (i != num_active)
            {
               BinaryDomain *b_dom = dynamic_cast<BinaryDomain *>(box.get(binaries_[i]));
               b_dom->setVal(ZeroOne::zero());
            }
         }
      }

      iv &= intervals_[num_active];
      box.get(v_)->contractInterval(iv); // Prune v_ domain to be sure, useless?
      return constraints_[num_active].contract(box);
   }
   else // nb_active==0 && nb_possibly_active>0
   {
      if (nb_possibly_active == 1) // domain of v_ intersect a single piece where binary
                                   // variable is undetermined
      {
         iv &= intervals_[num_active];
         box.get(v_)->contractInterval(iv);
         BinaryDomain *b_dom =
             dynamic_cast<BinaryDomain *>(box.get(binaries_[num_active]));
         b_dom->setVal(ZeroOne::one());
         return constraints_[num_active].contract(box);
      }
   }
   return Proof::Maybe;
}

void PiecewiseCtr::print(std::ostream &os) const
{
   os << "{ ";
   if (intervals_.size() > 0)
      os << v_.getName() << " in " << intervals_[0] << " -> " << constraints_[0];
   for (size_t i = 1; i < intervals_.size(); i++)
      os << ", " << v_.getName() << " in " << intervals_[i] << " -> " << constraints_[i];
   os << " }";
}

void PiecewiseCtr::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

ConstraintRep *PiecewiseCtr::cloneRoot() const
{
   return new PiecewiseCtr(v_, binaries_, intervals_, constraints_);
}

bool PiecewiseCtr::isInteger() const
{
   bool is_integer = true;
   size_t i = 0;
   while (i < constraints_.size() && is_integer)
   {
      is_integer &= constraints_[i].isInteger();
      i++;
   }
   return is_integer;
}

/**
 * Argsort(currently support ascending sort)
 * @param array input array
 * @return indices w.r.t sorted array
 */
std::vector<size_t> argsort(const std::vector<Interval> &array)
{
   std::vector<size_t> indices(array.size());
   std::iota(indices.begin(), indices.end(), 0);
   std::sort(indices.begin(), indices.end(), [&array](int left, int right) -> bool {
      // sort indices according to corresponding array element
      return array[left].left() < array[right].left();
   });

   return indices;
}

bool checkNonOverlappingIntervals(const std::vector<Interval> &intervals)
{
   // bool complementarity = true;
   if (intervals.size() > 0)
   {
      // check consecutive images do not overlap
      std::vector<size_t> idx = argsort(intervals);
      for (size_t i = 0; i < idx.size() - 1; i++)
      {
         // only check if ith right bound is lower equal i+1th left bound
         if (Double::prevDouble(intervals[idx[i]].right()) >
             Double::nextDouble(intervals[idx[i + 1]].left()))
         {
            return false;
         }
      }
   }
   return true;
}

size_t PiecewiseCtr::nb_binaries_ = 0;
const string PiecewiseCtr::binary_prefix_ = "__z";

string PiecewiseCtr::getNextBinaryName()
{
   std::stringstream ss;
   ss << PiecewiseCtr::nb_binaries_;
   PiecewiseCtr::nb_binaries_++;
   return PiecewiseCtr::binary_prefix_ + ss.str();
}

Constraint piecewise(Variable v, const std::initializer_list<Variable> &bins,
                     const std::initializer_list<Interval> &ivs,
                     const std::initializer_list<Constraint> &ctrs)
{
   return Constraint(std::make_shared<PiecewiseCtr>(v, bins, ivs, ctrs));
}

Constraint piecewise(Variable v, const std::initializer_list<Interval> &ivs,
                     const std::initializer_list<Constraint> &ctrs, Problem *pb)
{
   std::vector<Variable> bins;
   for (size_t i = 0; i < ivs.size(); i++)
   {
      bins.push_back(pb->addBinaryVar(PiecewiseCtr::getNextBinaryName()));
   }

   return Constraint(std::make_shared<PiecewiseCtr>(v, bins, ivs, ctrs));
}

Constraint piecewise(Variable v, const std::vector<Variable> &bins,
                     const std::vector<Interval> &ivs,
                     const std::vector<Constraint> &ctrs)
{
   return Constraint(std::make_shared<PiecewiseCtr>(v, bins, ivs, ctrs));
}

Constraint piecewise(Variable v, const std::vector<Interval> &ivs,
                     const std::vector<Constraint> &ctrs, Problem *pb)
{
   std::vector<Variable> bins;
   for (size_t i = 0; i < ivs.size(); i++)
   {
      bins.push_back(pb->addBinaryVar(PiecewiseCtr::getNextBinaryName()));
   }

   return Constraint(std::make_shared<PiecewiseCtr>(v, bins, ivs, ctrs));
}

/*----------------------------------------------------------------------------*/

ConstraintVisitor::~ConstraintVisitor()
{
}

void ConstraintVisitor::apply(const ArithCtrEq *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrLe *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrLt *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrGe *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrGt *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrIn *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const TableCtr *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const CondCtr *c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const PiecewiseCtr *c)
{
   THROW("Visit method not implemented");
}

} // namespace realpaver
