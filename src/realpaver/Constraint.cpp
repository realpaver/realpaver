///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, RelSymbol rel)
{
   switch(rel)
   {
      case RelSymbol::Eq:     return os << "==";
      case RelSymbol::Le:     return os << "<=";
      case RelSymbol::Lt:     return os << "<";
      case RelSymbol::Ge:     return os << ">=";
      case RelSymbol::Gt:     return os << ">";
      case RelSymbol::In:     return os << "in";
      case RelSymbol::Table:  return os << "table";
      default:            os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

ConstraintRep::ConstraintRep(RelSymbol rel)
      : scop_(),
        rel_(rel),
        hcode_(0)
{}

ConstraintRep::~ConstraintRep()
{}

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

RelSymbol Constraint::relSymbol() const
{
   return rep_->relSymbol();
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

Proof Constraint::isSatisfied(const IntervalBox& B)
{
   return rep_->isSatisfied(B);
}

double Constraint::violation(const IntervalBox& B)
{
   return rep_->violation(B);
}

Proof Constraint::contract(IntervalBox& B)
{
   return rep_->contract(B);
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

std::ostream& operator<<(std::ostream& os, Constraint c)
{
   c.print(os);
   return os;
}

ConstraintRep* Constraint::cloneRoot() const
{
   return rep_->cloneRoot();
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrBinary::ArithCtrBinary(Term l, Term r, RelSymbol rel)
      : ConstraintRep(rel),
        l_(l),
        r_(r)
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
{}

Term ArithCtrBinary::left() const
{
   return l_;
}

Term ArithCtrBinary::right() const
{
   return r_;
}

bool ArithCtrBinary::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

void ArithCtrBinary::print(std::ostream& os) const
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

   return rel == RelSymbol::Ge || rel == RelSymbol::Gt ||
          rel == RelSymbol::Le || rel == RelSymbol::Lt ||
          rel == RelSymbol::In;
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

///////////////////////////////////////////////////////////////////////////////

ArithCtrEq::ArithCtrEq(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Eq)
{}

void ArithCtrEq::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrEq::isSatisfied(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ArithCtrEq::violation(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyEq(r)) return 0.0;
 
   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

Proof ArithCtrEq::contract(IntervalBox& B)
{
   Interval l = left().hc4ReviseForward(B),
            r = right().hc4ReviseForward(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
   {
      Interval img = l & r;

      Proof pl = left().hc4ReviseBackward(B, img),
            pr = right().hc4ReviseBackward(B, img);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator==(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrEq>(l.rep(), r.rep()));
}

ConstraintRep* ArithCtrEq::cloneRoot() const
{
   return new ArithCtrEq(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrLe::ArithCtrLe(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Le)
{}

void ArithCtrLe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrLe::isSatisfied(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ArithCtrLe::violation(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyLe(r)) return 0.0;
 
   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLe::contract(IntervalBox& B)
{
   Interval l = left().hc4ReviseForward(B),
            r = right().hc4ReviseForward(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
   {
      Interval imgl = Interval::lessThan(r.right()),
               imgr = Interval::moreThan(l.left());

      Proof pl = left().hc4ReviseBackward(B, imgl),
            pr = right().hc4ReviseBackward(B, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator<=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLe>(l.rep(), r.rep()));
}

ConstraintRep* ArithCtrLe::cloneRoot() const
{
   return new ArithCtrLe(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrLt::ArithCtrLt(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Lt)
{}

void ArithCtrLt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrLt::isSatisfied(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ArithCtrLt::violation(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyLt(r)) return 0.0;
 
   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLt::contract(IntervalBox& B)
{
   Interval l = left().hc4ReviseForward(B),
            r = right().hc4ReviseForward(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
   {
      Interval imgl = Interval::lessThan(r.right()),
               imgr = Interval::moreThan(l.left());

      Proof pl = left().hc4ReviseBackward(B, imgl),
            pr = right().hc4ReviseBackward(B, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator<(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLt>(l.rep(), r.rep()));
}

ConstraintRep* ArithCtrLt::cloneRoot() const
{
   return new ArithCtrLt(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrGe::ArithCtrGe(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Ge)
{}

void ArithCtrGe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrGe::isSatisfied(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ArithCtrGe::violation(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyGe(r)) return 0.0;
 
   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGe::contract(IntervalBox& B)
{
   Interval l = left().hc4ReviseForward(B),
            r = right().hc4ReviseForward(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
   {
      Interval imgl = Interval::moreThan(r.left()),
               imgr = Interval::lessThan(l.right());

      Proof pl = left().hc4ReviseBackward(B, imgl),
            pr = right().hc4ReviseBackward(B, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator>=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGe>(l.rep(), r.rep()));
}

ConstraintRep* ArithCtrGe::cloneRoot() const
{
   return new ArithCtrGe(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrGt::ArithCtrGt(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Gt)
{}

void ArithCtrGt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrGt::isSatisfied(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
      return Proof::Maybe;

   else
      return Proof::Empty;
}

double ArithCtrGt::violation(const IntervalBox& B)
{
   Interval l = left().eval(B),
            r = right().eval(B);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyGt(r)) return 0.0;
 
   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGt::contract(IntervalBox& B)
{
   Interval l = left().hc4ReviseForward(B),
            r = right().hc4ReviseForward(B);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
   {
      Interval imgl = Interval::moreThan(r.left()),
               imgr = Interval::lessThan(l.right());

      Proof pl = left().hc4ReviseBackward(B, imgl),
            pr = right().hc4ReviseBackward(B, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator>(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGt>(l.rep(), r.rep()));
}

ConstraintRep* ArithCtrGt::cloneRoot() const
{
   return new ArithCtrGt(left(), right());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrIn::ArithCtrIn(Term t, const Interval& x)
      : ArithCtrBinary(t, x, RelSymbol::In), x_(x)
{
   ASSERT(!(x.isEmpty() || x.isUniverse()),
          "Bad interval target in a IN constraint");
}

Interval ArithCtrIn::image() const
{
   return x_;
}

Term ArithCtrIn::term() const
{
   return left();
}

void ArithCtrIn::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrIn::isSatisfied(const IntervalBox& B)
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

double ArithCtrIn::violation(const IntervalBox& B)
{
   Interval e = term().eval(B);

   if (e.isEmpty()) return Double::inf();
   if (x_.overlaps(e)) return 0.0;
 
   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

Proof ArithCtrIn::contract(IntervalBox& B)
{
   Interval e = term().hc4ReviseForward(B);

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
   {
      Interval img = e & x_;

      return term().hc4ReviseBackward(B, img);
   }

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
      return Constraint(std::make_shared<ArithCtrIn>(t.rep(), x));
}

Constraint in(Term t, double a, double b)
{
   return in(t, Interval(a,b));
}

ConstraintRep* ArithCtrIn::cloneRoot() const
{
   return new ArithCtrIn(term(), image());
}

///////////////////////////////////////////////////////////////////////////////

TableCtrCol::TableCtrCol(Variable v)
      : v_(v),
        vval_()
{}

TableCtrCol::TableCtrCol(Variable v, const std::initializer_list<Interval>& l)
      : v_(v),
        vval_(l)
{}

size_t TableCtrCol::size() const
{
   return vval_.size();
}

Variable TableCtrCol::getVar() const
{
   return v_;
}

void TableCtrCol::addValue(const Interval& x)
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

///////////////////////////////////////////////////////////////////////////////

TableCtr::TableCtr()
      : ConstraintRep(RelSymbol::Table),
        vcol_()
{}

TableCtr::TableCtr(
   const std::initializer_list<TableCtrCol>& l)
      : ConstraintRep(RelSymbol::Table),
        vcol_(l)
{
   ASSERT(nbCols() > 0, "Bad initialization of a constraint table");
   ASSERT(nbRows() > 0, "Bad initialization of a constraint table");

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

TableCtr::TableCtr(
   const std::initializer_list<Variable>& vars,
   const std::initializer_list<Interval>& values)
      : ConstraintRep(RelSymbol::Table),
        vcol_()
{
   size_t nbvar = vars.size();
   size_t nbitv = values.size();

   ASSERT(nbvar > 0 && nbitv > nbvar && (nbitv % nbvar == 0),
          "Bad initialization of a constraint table");

   // creates the columns
   for (auto itv=vars.begin(); itv != vars.end(); ++itv)
   {
      TableCtrCol col(*itv);
      vcol_.push_back(col);
   }

   auto itv = values.begin();
   size_t nrows = nbitv / nbvar;

   for (size_t i=0; i<nrows; ++i)
   {
      for (size_t j=0; j<nbvar; ++j, ++itv)
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

   for (auto& col : vcol_)
   {
      Variable v = col.getVar();
      s.insert(v);
      hcode_ = hash2(v.hashCode(), hcode_);
   }

   setScope(s);
}

bool TableCtr::isInteger() const
{
   for (auto& col : vcol_)
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

void TableCtr::addCol(const TableCtrCol& col)
{
   ASSERT(vcol_.empty() || col.size() == nbRows(),
          "Bad insertion of a new column in a table constraint");

   ASSERT(!dependsOn(col.getVar()),
          "Variable already present in a table constraint: " <<
          col.getVar().getName());

   vcol_.push_back(col);

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

bool TableCtr::isConstant() const
{
   return vcol_.empty();
}

bool TableCtr::isRowConsistent(size_t i, const IntervalBox& B) const
{
   for (size_t j=0; j<nbCols(); ++j)
   {      
      Variable v = vcol_[j].getVar();
      if (B.get(v).isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

Proof TableCtr::isSatisfied(const IntervalBox& B)
{
   size_t nbc = 0;

   for (size_t i=0; i<nbRows(); ++i)
      if (isRowConsistent(i, B))
      {
         ++nbc;
         if (nbc > 1) return Proof::Maybe;
      }

   return (nbc == 1) ? Proof::Inner : Proof::Empty;
}

double TableCtr::violation(const IntervalBox& B)
{
   double res = Double::inf();

   for (size_t i=0; i<nbRows(); ++i)
      res = Double::min(res, rowViolation(B, i));

   return res;
}

double TableCtr::rowViolation(const IntervalBox& B, size_t i)
{
   double res = 0.0;
   Double::rndNear();

   for (size_t j=0; j<nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      Interval val = vcol_[j].getVal(i);
      Interval dom = B.get(v);

      double viol = 0.0;

      if (dom.isCertainlyLt(val))
         viol = val.left() - dom.right();

      else if (dom.isCertainlyGt(val))
         viol = dom.left() - val.right();

      if (viol > res) res = viol;
   }

   return res;
}

Proof TableCtr::contract(IntervalBox& B)
{
   Bitset consistent(nbRows());
   consistent.setAllOne();
   size_t nbc = nbRows();     // number of consistent rows

   // checks consistency
   for (size_t i=0; i<nbRows(); ++i)
      if (!isRowConsistent(i, B))
      {
         consistent.setZero(i);
         --nbc;
      }

   if (nbc == 0) return Proof::Empty;

   // contracts the domains
   for (size_t j=0; j<nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();

      // hull of values of this variable occurring in the consistent rows
      Interval h = Interval::emptyset();
      for (size_t i=0; i<nbRows(); ++i)
         if (consistent.get(i))
            h |= vcol_[j].getVal(i);

      Interval x = h & B.get(v);
      if (x.isEmpty()) return Proof::Empty;
      B.set(v, x);
   }

   return (nbc == 1) ? Proof::Inner : Proof::Maybe;
}

void TableCtr::print(std::ostream& os) const
{
   os << "table(";
   // prints the variables
   os << "{";
   for (size_t i=0; i<nbCols(); ++i)
   {
      if (i > 0) os << ", ";
      os << vcol_[i].getVar().getName();
   }
   os << "}, ";

   // prints the rows
   os << "{";
   for (size_t j=0; j<nbRows(); ++j)
   {
      if (j > 0) os << ", ";
      for (size_t i=0; i<nbCols(); ++i)
      {
         if (i> 0) os << ", ";
         os << vcol_[i].getVal(j);
      }
   }
   os << "})";
}

void TableCtr::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Constraint table(const std::initializer_list<Variable>& vars,
                 const std::initializer_list<Interval>& values)
{
   return Constraint(std::make_shared<TableCtr>(vars, values));
}

Constraint table(const Variable* vars, size_t nvars,
                 const Interval* values, size_t nvalues)
{
   if (nvars == 0 || nvalues % nvars != 0)
      THROW("Bad initialization of a table constraint");

   size_t nrows = nvalues / nvars;
   
   Constraint::SharedRep srep = std::make_shared<TableCtr>();
   TableCtr* rep = static_cast<TableCtr*>(srep.get());

   for (size_t j=0; j<nvars; ++j)
   {
      TableCtrCol col(vars[j]);
      for (size_t i=0; i<nrows; ++i)
      {
         Interval x = values[i*nvars + j];
         col.addValue(x);
      }
      rep->addCol(col);
   }
   return Constraint(srep);
}

ConstraintRep* TableCtr::cloneRoot() const
{
   return new TableCtr(*this);
}

///////////////////////////////////////////////////////////////////////////////

CondCtr::CondCtr(Constraint guard, Constraint body)
      : ConstraintRep(RelSymbol::Cond),
        guard_(guard),
        body_(body)
{
   THROW_IF(!(guard.isInteger() || guard.isBoundConstraint()),
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

Proof CondCtr::isSatisfied(const IntervalBox& B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.isSatisfied(B);

   return p;
}

double CondCtr::violation(const IntervalBox& B)
{
   Proof p = guard_.isSatisfied(B);
   return (p == Proof::Inner) ?  body_.violation(B) : 0.0;
}

Proof CondCtr::contract(IntervalBox& B)
{
   Proof p = guard_.isSatisfied(B);

   if (p == Proof::Empty)
      return Proof::Inner;

   if (p == Proof::Inner)
      p = body_.contract(B);

   return p;
}

void CondCtr::print(std::ostream& os) const
{
   os << guard_ << " -> " << body_;
}

void CondCtr::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

ConstraintRep* CondCtr::cloneRoot() const
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

///////////////////////////////////////////////////////////////////////////////

ConstraintVisitor::~ConstraintVisitor()
{}

void ConstraintVisitor::apply(const ArithCtrEq* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrLe* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrLt* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrGe* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrGt* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const ArithCtrIn* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const TableCtr* c)
{
   THROW("Visit method not implemented");
}

void ConstraintVisitor::apply(const CondCtr* c)
{
   THROW("Visit method not implemented");
}

} // namespace
