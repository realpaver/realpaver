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
 * @file   ConstraintTable.cpp
 * @brief  Table constraints
 * @author Laurent Granvilliers
 * @date   28 may 2025
 */

#include "realpaver/ConstraintTable.hpp"
#include "realpaver/ConstraintVisitor.hpp"
#include <set>

namespace realpaver {

ConstraintTableCol::ConstraintTableCol(Variable v)
    : v_(v)
    , vval_()
{
}

ConstraintTableCol::ConstraintTableCol(Variable v,
                                       const std::initializer_list<Interval> &l)
    : v_(v)
    , vval_(l)
{
}

size_t ConstraintTableCol::size() const
{
   return vval_.size();
}

Variable ConstraintTableCol::getVar() const
{
   return v_;
}

void ConstraintTableCol::addValue(const Interval &x)
{
   vval_.push_back(x);
}

Interval ConstraintTableCol::getVal(size_t i) const
{
   ASSERT(i < size(), "Bad access in a column of a table constraint @ " << i);
   return vval_[i];
}

bool ConstraintTableCol::isInteger() const
{
   return v_.getDomain()->isInteger();
}

void ConstraintTableCol::removeVal(size_t i)
{
   auto it = vval_.begin();
   std::advance(it, i);
   vval_.erase(it);
}

/*----------------------------------------------------------------------------*/

ConstraintTable::ConstraintTable()
    : ConstraintRep(ConstraintRelation::Table)
    , vcol_()
{
}

ConstraintTable::ConstraintTable(const std::initializer_list<ConstraintTableCol> &l)
    : ConstraintRep(ConstraintRelation::Table)
    , vcol_(l)
{
   ASSERT(nbCols() > 0, "Bad initialization of a constraint table");
   ASSERT(nbRows() > 0, "Bad initialization of a constraint table");

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

ConstraintTable::ConstraintTable(const std::initializer_list<Variable> &vars,
                                 const std::initializer_list<Interval> &values)
    : ConstraintRep(ConstraintRelation::Table)
    , vcol_()
{
   size_t nbvar = vars.size();
   size_t nbitv = values.size();

   ASSERT(nbvar > 0 && nbitv > nbvar && (nbitv % nbvar == 0),
          "Bad initialization of a constraint table");

   // creates the columns
   for (auto itv = vars.begin(); itv != vars.end(); ++itv)
   {
      ConstraintTableCol col(*itv);
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

void ConstraintTable::makeScopeAndHashCode()
{
   Scope s;
   hcode_ = static_cast<size_t>(ConstraintRelation::Table);

   for (auto &col : vcol_)
   {
      Variable v = col.getVar();
      s.insert(v);
      hcode_ = hash2(v.hashCode(), hcode_);
   }

   setScope(s);
}

bool ConstraintTable::isInteger() const
{
   for (auto &col : vcol_)
      if (!col.isInteger())
         return false;

   return true;
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

void ConstraintTable::addCol(const ConstraintTableCol &col)
{
   ASSERT(vcol_.empty() || col.size() == nbRows(),
          "Bad insertion of a new column in a table constraint");

   ASSERT(!dependsOn(col.getVar()),
          "Variable already present in a table constraint: " << col.getVar().getName());

   vcol_.push_back(col);

   // makes the scope and calculates the hash code
   makeScopeAndHashCode();
}

bool ConstraintTable::isConstant() const
{
   return vcol_.empty();
}

bool ConstraintTable::isRowConsistent(size_t i, const IntervalBox &B) const
{
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      if (B.get(v).isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

bool ConstraintTable::isRowConsistent(size_t i, const DomainBox &box) const
{
   for (size_t j = 0; j < nbCols(); ++j)
   {
      Variable v = vcol_[j].getVar();
      if (box.get(v)->intervalHull().isDisjoint(vcol_[j].getVal(i)))
         return false;
   }
   return true;
}

Proof ConstraintTable::isSatisfied(const IntervalBox &B)
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

double ConstraintTable::violation(const IntervalBox &B)
{
   double res = Double::inf();

   for (size_t i = 0; i < nbRows(); ++i)
      res = Double::min(res, rowViolation(B, i));

   return res;
}

double ConstraintTable::rowViolation(const IntervalBox &B, size_t i)
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

double ConstraintTable::rowViolation(const DomainBox &box, size_t i)
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

Proof ConstraintTable::contract(IntervalBox &B)
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

Proof ConstraintTable::isSatisfied(const DomainBox &box)
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

double ConstraintTable::violation(const DomainBox &box)
{
   double res = Double::inf();

   for (size_t i = 0; i < nbRows(); ++i)
      res = Double::min(res, rowViolation(box, i));

   return res;
}

Proof ConstraintTable::contract(DomainBox &box)
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

void ConstraintTable::print(std::ostream &os) const
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

void ConstraintTable::acceptVisitor(ConstraintVisitor &vis) const
{
   vis.apply(this);
}

ConstraintRep *ConstraintTable::cloneRoot() const
{
   return new ConstraintTable(*this);
}

bool ConstraintTable::areSameRows(size_t i, size_t j) const
{
   for (size_t k = 0; k < nbCols(); ++k)
   {
      if (vcol_[k].getVal(i).isSetNeq(vcol_[k].getVal(j)))
         return false;
   }
   return true;
}

void ConstraintTable::removeRedundantRows()
{
   // finds the redundant rows
   std::set<size_t> redundant;
   for (size_t i = 0; i < nbRows() - 1; ++i)
      for (size_t j = i + 1; j < nbRows(); ++j)
         if (areSameRows(i, j))
            redundant.insert(j);

   // removes them, nb is the number of removed rows
   size_t nb = 0;
   for (auto it = redundant.begin(); it != redundant.end(); ++it)
   {
      size_t row = *it - nb++;
      for (size_t k = 0; k < nbCols(); ++k)
      {
         vcol_[k].removeVal(row);
      }
   }
}

Constraint table(const std::initializer_list<Variable> &vars,
                 const std::initializer_list<Interval> &values)
{
   std::shared_ptr<ConstraintTable> tbl = std::make_shared<ConstraintTable>(vars, values);
   tbl->removeRedundantRows();
   return Constraint(tbl);
}

Constraint table(const Variable *vars, size_t nvars, const Interval *values,
                 size_t nvalues)
{
   if (nvars == 0 || nvalues % nvars != 0)
      THROW("Bad initialization of a table constraint");

   size_t nrows = nvalues / nvars;

   std::shared_ptr<ConstraintTable> tbl = std::make_shared<ConstraintTable>();

   for (size_t j = 0; j < nvars; ++j)
   {
      ConstraintTableCol col(vars[j]);
      for (size_t i = 0; i < nrows; ++i)
      {
         Interval x = values[i * nvars + j];
         col.addValue(x);
      }
      tbl->addCol(col);
   }
   tbl->removeRedundantRows();
   return Constraint(tbl);
}

} // namespace realpaver
