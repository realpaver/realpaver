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
 * @file   Domain.cpp
 * @brief  Classes of domains
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Domain.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

Domain::~Domain()
{
}

Domain::Domain(DomainType type)
    : type_(type)
{
}

DomainType Domain::type() const
{
   return type_;
}

bool Domain::isConnected() const
{
   return false;
}

bool Domain::isBinary() const
{
   return false;
}

bool Domain::isInteger() const
{
   return false;
}

bool Domain::isReal() const
{
   return false;
}

double Domain::discreteSize(double tol) const
{
   if (isReal())
   {
      Interval x = intervalHull();
      if (x.isEmpty())
         return 0.0;

      else if (x.isCanonical())
         return 1.0;

      else if (x.isInf())
         return Double::floor(Double::greatest());

      else
      {
         double a = x.width() / tol, b = Double::floor(a);
         return (a == b) ? b : b + 1.0;
      }
   }
   else
   {
      return size();
   }
}

std::ostream &operator<<(std::ostream &os, const Domain &dom)
{
   dom.print(os);
   return os;
}

/*----------------------------------------------------------------------------*/

IntervalDomain::IntervalDomain(const Interval &x)
    : Domain(DomainType::Interval)
    , val_(x)
{
}

double IntervalDomain::size() const
{
   return val_.isEmpty() ? 0.0 : val_.width();
}

bool IntervalDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool IntervalDomain::isCanonical() const
{
   return val_.isCanonical();
}

bool IntervalDomain::isReal() const
{
   return true;
}

const Interval &IntervalDomain::getVal() const
{
   return val_;
}

void IntervalDomain::setVal(const Interval &x)
{
   val_ = x;
}

Interval IntervalDomain::intervalHull() const
{
   return val_;
}

void IntervalDomain::contractInterval(Interval &x) const
{
   x &= val_;
}

void IntervalDomain::contract(const Interval &x)
{
   val_ &= x;
}

bool IntervalDomain::isConnected() const
{
   return true;
}

IntervalDomain *IntervalDomain::clone() const
{
   return new IntervalDomain(*this);
}

void IntervalDomain::print(std::ostream &os) const
{
   os << val_;
}

bool IntervalDomain::equals(const Domain &dom) const
{
   const IntervalDomain *d = dynamic_cast<const IntervalDomain *>(&dom);
   return (d == nullptr) ? false : d->val_.isSetEq(val_);
}

/*----------------------------------------------------------------------------*/

IntervalUnionDomain::IntervalUnionDomain(const IntervalUnion &u)
    : Domain(DomainType::IntervalUnion)
    , val_(u)
{
}

IntervalUnionDomain::IntervalUnionDomain(const std::initializer_list<Interval> &l)
    : Domain(DomainType::IntervalUnion)
    , val_(l)
{
}

const IntervalUnion &IntervalUnionDomain::getVal() const
{
   return val_;
}

void IntervalUnionDomain::setVal(const IntervalUnion &u)
{
   val_ = u;
}

double IntervalUnionDomain::size() const
{
   return val_.width();
}

bool IntervalUnionDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool IntervalUnionDomain::isCanonical() const
{
   return (val_.size() == 1) && val_[0].isCanonical();
}

bool IntervalUnionDomain::isReal() const
{
   return true;
}

Interval IntervalUnionDomain::intervalHull() const
{
   return val_.hull();
}

void IntervalUnionDomain::contractInterval(Interval &x) const
{
   val_.contractInterval(x);
}

void IntervalUnionDomain::contract(const Interval &x)
{
   val_.contract(x);
}

IntervalUnionDomain *IntervalUnionDomain::clone() const
{
   return new IntervalUnionDomain(*this);
}

void IntervalUnionDomain::print(std::ostream &os) const
{
   os << val_;
}

bool IntervalUnionDomain::equals(const Domain &dom) const
{
   const IntervalUnionDomain *d = dynamic_cast<const IntervalUnionDomain *>(&dom);
   return (d == nullptr) ? false : d->val_.equals(val_);
}

/*----------------------------------------------------------------------------*/

RangeDomain::RangeDomain(const Range &r)
    : Domain(DomainType::Range)
    , val_(r)
{
}

const Range &RangeDomain::getVal() const
{
   return val_;
}

void RangeDomain::setVal(const Range &r)
{
   val_ = r;
}

double RangeDomain::size() const
{
   return (double)val_.nbElems();
}

bool RangeDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool RangeDomain::isCanonical() const
{
   return val_.isSingleton();
}

bool RangeDomain::isInteger() const
{
   return true;
}

Interval RangeDomain::intervalHull() const
{
   return val_.toInterval();
}

void RangeDomain::contractInterval(Interval &x) const
{
   x = round(x) & intervalHull();
}

void RangeDomain::contract(const Interval &x)
{
   val_ &= Range::roundInward(x);
}

RangeDomain *RangeDomain::clone() const
{
   return new RangeDomain(*this);
}

void RangeDomain::print(std::ostream &os) const
{
   os << val_;
}

bool RangeDomain::equals(const Domain &dom) const
{
   const RangeDomain *d = dynamic_cast<const RangeDomain *>(&dom);
   return (d == nullptr) ? false : d->val_.isSetEq(val_);
}

/*----------------------------------------------------------------------------*/

RangeUnionDomain::RangeUnionDomain(const RangeUnion &u)
    : Domain(DomainType::RangeUnion)
    , val_(u)
{
}

RangeUnionDomain::RangeUnionDomain(const std::initializer_list<Range> &l)
    : Domain(DomainType::RangeUnion)
    , val_(l)
{
}

const RangeUnion &RangeUnionDomain::getVal() const
{
   return val_;
}

void RangeUnionDomain::setVal(const RangeUnion &u)
{
   val_ = u;
}

double RangeUnionDomain::size() const
{
   return (double)val_.nbElems();
}

bool RangeUnionDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool RangeUnionDomain::isCanonical() const
{
   return (val_.size() == 1) && val_[0].isSingleton();
}

bool RangeUnionDomain::isInteger() const
{
   return true;
}

Interval RangeUnionDomain::intervalHull() const
{
   return val_.hull().toInterval();
}

void RangeUnionDomain::contractInterval(Interval &x) const
{
   val_.contractInterval(x);
}

void RangeUnionDomain::contract(const Interval &x)
{
   val_.contract(x);
}

RangeUnionDomain *RangeUnionDomain::clone() const
{
   return new RangeUnionDomain(*this);
}

void RangeUnionDomain::print(std::ostream &os) const
{
   os << val_;
}

bool RangeUnionDomain::equals(const Domain &dom) const
{
   const RangeUnionDomain *d = dynamic_cast<const RangeUnionDomain *>(&dom);
   return (d == nullptr) ? false : d->val_.equals(val_);
}

/*----------------------------------------------------------------------------*/

BinaryDomain::BinaryDomain()
    : Domain(DomainType::Binary)
    , val_()
{
}

BinaryDomain::BinaryDomain(const ZeroOne &zo)
    : Domain(DomainType::Binary)
    , val_(zo)
{
   ASSERT(!zo.isEmpty(), "Creation of an empty binary domain");
}

const ZeroOne &BinaryDomain::getVal() const
{
   return val_;
}

void BinaryDomain::setVal(const ZeroOne &zo)
{
   val_ = zo;
}

double BinaryDomain::size() const
{
   if (val_.isEmpty())
      return 0.0;
   if (val_.isUniverse())
      return 2.0;
   return 1.0;
}

bool BinaryDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool BinaryDomain::isCanonical() const
{
   return val_.isZero() || val_.isOne();
}

bool BinaryDomain::isBinary() const
{
   return true;
}

Interval BinaryDomain::intervalHull() const
{
   return val_.toRange().toInterval();
}

void BinaryDomain::contractInterval(Interval &x) const
{
   x = round(x) & intervalHull();
}

void BinaryDomain::contract(const Interval &x)
{
   if (!x.contains(0.0))
      val_.setZero(false);

   if (!x.contains(1.0))
      val_.setOne(false);
}

BinaryDomain *BinaryDomain::clone() const
{
   return new BinaryDomain(*this);
}

void BinaryDomain::print(std::ostream &os) const
{
   os << val_;
}

bool BinaryDomain::equals(const Domain &dom) const
{
   const BinaryDomain *d = dynamic_cast<const BinaryDomain *>(&dom);
   return (d == nullptr) ? false : d->val_.equals(val_);
}

} // namespace realpaver
