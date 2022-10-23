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

Proof ConstraintEq::contract(IntervalRegion& reg)
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

Proof ConstraintLe::contract(IntervalRegion& reg)
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

Proof ConstraintLt::contract(IntervalRegion& reg)
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

Proof ConstraintGe::contract(IntervalRegion& reg)
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

Proof ConstraintGt::contract(IntervalRegion& reg)
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

Proof ConstraintIn::contract(IntervalRegion& reg)
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
      return Constraint(std::make_shared<ConstraintIn>(t.rep(), x));
}

Constraint in(Term t, double a, double b)
{
   return in(t, Interval(a,b));
}

///////////////////////////////////////////////////////////////////////////////

ConstraintTableCol::ConstraintTableCol(Variable v)
      : v_(v),
      vval_()
{}

ConstraintTableCol::ConstraintTableCol(Variable v,
                                       const std::initializer_list<Interval>& l)
      : v_(v),
        vval_(l)
{}

size_t ConstraintTableCol::size() const
{
   return vval_.size();
}

Variable ConstraintTableCol::getVar() const
{
   return v_;
}

void ConstraintTableCol::addValue(const Interval& x)
{
   vval_.push_back(x);
}

Interval ConstraintTableCol::getVal(size_t i) const
{
   ASSERT(i < size(), "Bad access in a column of a table constraint @ " << i);
   return vval_[i];
}

///////////////////////////////////////////////////////////////////////////////

ConstraintTable::ConstraintTable()
      : ConstraintRep(),
        vcol_()
{}

ConstraintTable::ConstraintTable(
   const std::initializer_list<ConstraintTableCol>& l)
      : ConstraintRep(),
        vcol_(l)
{
   ASSERT(nbCols() > 0, "Bad initialization of a constraint table");
   ASSERT(nbRows() > 0, "Bad initialization of a constraint table");

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

ConstraintTable::ConstraintTable(
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
      ConstraintTableCol col(*itv);
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

void ConstraintTable::makeScopeAndHashCode()
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

size_t ConstraintTable::nbCols() const
{
   return vcol_.size();
}

size_t ConstraintTable::nbRows() const
{
   return vcol_.empty() ? 0 : vcol_[0].size();
}

Variable ConstraintTable::getVar(size_t j) const
{
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j].getVar();
}

Interval ConstraintTable::getVal(size_t i, size_t j) const
{
   ASSERT(i < nbRows(), "Bad access to a row in a table constraint @ " << i);
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j].getVal(i);   
}


ConstraintTableCol ConstraintTable::getCol(size_t j) const
{
   ASSERT(j < nbCols(), "Bad access to a column in a table constraint @ " << j);
   return vcol_[j];
}

void ConstraintTable::addCol(const ConstraintTableCol& col)
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

bool ConstraintTable::isConstant() const
{
   return vcol_.empty();
}

bool ConstraintTable::isRowConsistent(size_t i, const IntervalRegion& reg) const
{
   for (size_t j=0; j<nbCols(); ++j)
   {      
      Variable v = vcol_[j].getVar();
      if (reg.get(v).isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

Proof ConstraintTable::isSatisfied(const IntervalRegion& reg) const
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

Proof ConstraintTable::contract(IntervalRegion& reg)
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

void ConstraintTable::print(std::ostream& os) const
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

void ConstraintTable::acceptVisitor(ConstraintVisitor& vis) const
{
   vis.apply(this);
}

Constraint table(const std::initializer_list<Variable>& vars,
                 const std::initializer_list<Interval>& values)
{
   return Constraint(std::make_shared<ConstraintTable>(vars, values));
}

Constraint table(const Variable* vars, size_t nvars,
                 const Interval* values, size_t nvalues)
{
   if (nvars == 0 || nvalues % nvars != 0)
      THROW("Bad initialization of a table constraint");

   size_t nrows = nvalues / nvars;
   
   Constraint::SharedRep srep = std::make_shared<ConstraintTable>();
   ConstraintTable* rep = static_cast<ConstraintTable*>(srep.get());

   for (size_t j=0; j<nvars; ++j)
   {
      ConstraintTableCol col(vars[j]);
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

void ConstraintVisitor::apply(const ConstraintTable* c)
{
   THROW("Visit method not implemented");
}

} // namespace
