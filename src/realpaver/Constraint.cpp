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

bool ConstraintRep::dependsOn(Variable v) const
{
   return scope_.contains(v);
}

bool ConstraintRep::dependsOn(const Bitset& bs) const
{
   return bs_.overlaps(bs);
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

Proof Constraint::isSatisfied(const IntervalRegion& reg)
{
   return rep_->isSatisfied(reg);
}

double Constraint::violation(const IntervalRegion& reg)
{
   return rep_->violation(reg);
}

Proof Constraint::contract(IntervalRegion& reg)
{
   return rep_->contract(reg);
}

bool Constraint::dependsOn(Variable v) const
{
   return rep_->dependsOn(v);
}

bool Constraint::dependsOn(const Bitset& bs) const
{
   return rep_->dependsOn(bs);
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

std::ostream& operator<<(std::ostream& os, Constraint c)
{
   c.print(os);
   return os;
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrBinary::ArithCtrBinary(Term l, Term r, RelSymbol rel)
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

RelSymbol ArithCtrBinary::relSymbol() const
{
   return rel_;
}

bool ArithCtrBinary::isConstant() const
{
   return l_.isConstant() && r_.isConstant();
}

void ArithCtrBinary::print(std::ostream& os) const
{
   l_.print(os);
   os << " " << rel_ << " ";
   r_.print(os);
}

bool ArithCtrBinary::isEquation() const
{
   return rel_ == RelSymbol::Eq;
}

bool ArithCtrBinary::isInequality() const
{
   return rel_ == RelSymbol::Ge || rel_ == RelSymbol::Gt ||
          rel_ == RelSymbol::Le || rel_ == RelSymbol::Lt ||
          rel_ == RelSymbol::In;
}

bool ArithCtrBinary::isLinear() const
{
   return l_.isLinear() && r_.isLinear();
}

bool ArithCtrBinary::isBoundConstraint() const
{
   return (l_.isVar() && r_.isNumber()) || (l_.isNumber() && r_.isVar());
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrEq::ArithCtrEq(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Eq)
{}

void ArithCtrEq::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrEq::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrEq::violation(const IntervalRegion& reg)
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyEq(r)) return 0.0;
 
   Double::rndNear();
   return (l.isCertainlyLt(r)) ? r.left() - l.right() : l.left() - r.right();
}

Proof ArithCtrEq::contract(IntervalRegion& reg)
{
   Interval l = left().hc4ReviseForward(reg),
            r = right().hc4ReviseForward(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyEq(r))
      return Proof::Inner;

   else if (l.isPossiblyEq(r))
   {
      Interval img = l & r;

      Proof pl = left().hc4ReviseBackward(reg, img),
            pr = right().hc4ReviseBackward(reg, img);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator==(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrEq>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrLe::ArithCtrLe(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Le)
{}

void ArithCtrLe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrLe::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrLe::violation(const IntervalRegion& reg)
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyLe(r)) return 0.0;
 
   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLe::contract(IntervalRegion& reg)
{
   Interval l = left().hc4ReviseForward(reg),
            r = right().hc4ReviseForward(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLe(r))
      return Proof::Inner;

   else if (l.isPossiblyLe(r))
   {
      Interval imgl = Interval::lessThan(r.right()),
               imgr = Interval::moreThan(l.left());

      Proof pl = left().hc4ReviseBackward(reg, imgl),
            pr = right().hc4ReviseBackward(reg, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator<=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrLt::ArithCtrLt(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Lt)
{}

void ArithCtrLt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrLt::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrLt::violation(const IntervalRegion& reg)
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyLt(r)) return 0.0;
 
   Double::rndNear();
   return l.left() - r.right();
}

Proof ArithCtrLt::contract(IntervalRegion& reg)
{
   Interval l = left().hc4ReviseForward(reg),
            r = right().hc4ReviseForward(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyLt(r))
      return Proof::Inner;

   else if (l.isPossiblyLt(r))
   {
      Interval imgl = Interval::lessThan(r.right()),
               imgr = Interval::moreThan(l.left());

      Proof pl = left().hc4ReviseBackward(reg, imgl),
            pr = right().hc4ReviseBackward(reg, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator<(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrLt>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrGe::ArithCtrGe(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Ge)
{}

void ArithCtrGe::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrGe::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrGe::violation(const IntervalRegion& reg)
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyGe(r)) return 0.0;
 
   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGe::contract(IntervalRegion& reg)
{
   Interval l = left().hc4ReviseForward(reg),
            r = right().hc4ReviseForward(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGe(r))
      return Proof::Inner;

   else if (l.isPossiblyGe(r))
   {
      Interval imgl = Interval::moreThan(r.left()),
               imgr = Interval::lessThan(l.right());

      Proof pl = left().hc4ReviseBackward(reg, imgl),
            pr = right().hc4ReviseBackward(reg, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator>=(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGe>(l.rep(), r.rep()));
}

///////////////////////////////////////////////////////////////////////////////

ArithCtrGt::ArithCtrGt(Term l, Term r)
      : ArithCtrBinary(l, r, RelSymbol::Gt)
{}

void ArithCtrGt::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Proof ArithCtrGt::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrGt::violation(const IntervalRegion& reg)
{
   Interval l = left().eval(reg),
            r = right().eval(reg);

   if (l.isEmpty() || r.isEmpty()) return Double::inf();
   if (l.isPossiblyGt(r)) return 0.0;
 
   Double::rndNear();
   return r.left() - l.right();
}

Proof ArithCtrGt::contract(IntervalRegion& reg)
{
   Interval l = left().hc4ReviseForward(reg),
            r = right().hc4ReviseForward(reg);

   if (l.isEmpty() || r.isEmpty())
      return Proof::Empty;

   else if (l.isCertainlyGt(r))
      return Proof::Inner;

   else if (l.isPossiblyGt(r))
   {
      Interval imgl = Interval::moreThan(r.left()),
               imgr = Interval::lessThan(l.right());

      Proof pl = left().hc4ReviseBackward(reg, imgl),
            pr = right().hc4ReviseBackward(reg, imgr);

      return std::min(pl, pr);
   }

   else
      return Proof::Empty;
}

Constraint operator>(Term l, Term r)
{
   return Constraint(std::make_shared<ArithCtrGt>(l.rep(), r.rep()));
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

Proof ArithCtrIn::isSatisfied(const IntervalRegion& reg)
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

double ArithCtrIn::violation(const IntervalRegion& reg)
{
   Interval e = term().eval(reg);

   if (e.isEmpty()) return Double::inf();
   if (x_.overlaps(e)) return 0.0;
 
   Double::rndNear();
   return (x_.isCertainlyGt(e)) ? x_.left() - e.right() : e.left() - x_.right();
}

Proof ArithCtrIn::contract(IntervalRegion& reg)
{
   Interval e = term().hc4ReviseForward(reg);

   if (e.isEmpty())
      return Proof::Empty;

   else if (x_.contains(e))
      return Proof::Inner;

   else if (x_.overlaps(e))
   {
      Interval img = e & x_;

      return term().hc4ReviseBackward(reg, img);
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

///////////////////////////////////////////////////////////////////////////////

TableCtrCol::TableCtrCol(Variable v)
      : v_(v),
        vval_()
{}

TableCtrCol::TableCtrCol(Variable v,
                                       const std::initializer_list<Interval>& l)
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

///////////////////////////////////////////////////////////////////////////////

TableCtr::TableCtr()
      : ConstraintRep(),
        vcol_()
{}

TableCtr::TableCtr(
   const std::initializer_list<TableCtrCol>& l)
      : ConstraintRep(),
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
      : ConstraintRep(),
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

bool TableCtr::isRowConsistent(size_t i, const IntervalRegion& reg) const
{
   for (size_t j=0; j<nbCols(); ++j)
   {      
      Variable v = vcol_[j].getVar();
      if (reg.get(v).isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

Proof TableCtr::isSatisfied(const IntervalRegion& reg)
{
   size_t nbc = 0;

   for (size_t i=0; i<nbRows(); ++i)
      if (isRowConsistent(i, reg))
      {
         ++nbc;
         if (nbc > 1) return Proof::Maybe;
      }

   return (nbc == 1) ? Proof::Inner : Proof::Empty;
}

double TableCtr::violation(const IntervalRegion& reg)
{
   double res = Double::inf();

   for (size_t i=0; i<nbRows(); ++i)
      res = Double::min(res, rowViolation(reg, i));

   return res;
}

double TableCtr::rowViolation(const IntervalRegion& reg, size_t i)
{
   double res = 0.0;
   Double::rndNear();

   for (size_t j=0; j<nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      Interval val = vcol_[j].getVal(i);
      Interval dom = reg.get(v);

      double viol = 0.0;

      if (dom.isCertainlyLt(val))
         viol = val.left() - dom.right();

      else if (dom.isCertainlyGt(val))
         viol = dom.left() - val.right();

      if (viol > res) res = viol;
   }

   return res;
}

Proof TableCtr::contract(IntervalRegion& reg)
{
   Bitset consistent(nbRows(), 1);
   size_t nbc = nbRows();     // number of consistent rows

   // checks consistency
   for (size_t i=0; i<nbRows(); ++i)
      if (!isRowConsistent(i, reg))
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

      Interval x = h & reg.get(v);
      if (x.isEmpty()) return Proof::Empty;
      reg.set(v, x);
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

} // namespace
