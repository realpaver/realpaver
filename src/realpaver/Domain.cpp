///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Domain.hpp"

namespace realpaver {

Domain::~Domain()
{}

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

std::ostream& operator<<(std::ostream& os, const Domain& dom)
{
   dom.print(os);
   return os;
}

///////////////////////////////////////////////////////////////////////////////

IntervalDomain::IntervalDomain(const Interval& x)
      : Domain(),
        val_(x)
{}

bool IntervalDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool IntervalDomain::isReal() const
{
   return true;
}

const Interval& IntervalDomain::getVal() const
{
   return val_;
}

void IntervalDomain::setVal(const Interval& x)
{
   val_ = x;
}

Interval IntervalDomain::intervalHull() const
{
   return val_;
}

void IntervalDomain::contractInterval(Interval& x) const
{
   x &= val_;
}

void IntervalDomain::contract(const Interval& x)
{
   val_ &= x;
}

bool IntervalDomain::isConnected() const
{
   return true;
}

IntervalDomain* IntervalDomain::clone() const
{
   return new IntervalDomain(*this);
}

void IntervalDomain::print(std::ostream& os) const
{
   os << val_;
}

///////////////////////////////////////////////////////////////////////////////

IntervalUnionDomain::IntervalUnionDomain(const IntervalUnion& u)
      : Domain(),
        val_(u)
{}

IntervalUnionDomain::IntervalUnionDomain(
   const std::initializer_list<Interval>& l)
      : Domain(),
        val_(l)
{}

const IntervalUnion& IntervalUnionDomain::getVal() const
{
   return val_;
}

void IntervalUnionDomain::setVal(const IntervalUnion& u)
{
   val_ = u;
}

bool IntervalUnionDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool IntervalUnionDomain::isReal() const
{
   return true;
}

Interval IntervalUnionDomain::intervalHull() const
{
   return val_.hull();
}

void IntervalUnionDomain::contractInterval(Interval& x) const
{
   val_.contractInterval(x);
}

void IntervalUnionDomain::contract(const Interval& x)
{
   val_.contract(x);
}

IntervalUnionDomain* IntervalUnionDomain::clone() const
{
   return new IntervalUnionDomain(*this);
}

void IntervalUnionDomain::print(std::ostream& os) const
{
   os << val_;
}

///////////////////////////////////////////////////////////////////////////////

RangeDomain::RangeDomain(const Range& r)
      : Domain(),
        val_(r)
{}

const Range& RangeDomain::getVal() const
{
   return val_;
}

void RangeDomain::setVal(const Range& r)
{
   val_ = r;
}

bool RangeDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool RangeDomain::isInteger() const
{
   return true;
}

Interval RangeDomain::intervalHull() const
{
   return val_.toInterval();
}

void RangeDomain::contractInterval(Interval& x) const
{
   x = round(x) & intervalHull();
}

void RangeDomain::contract(const Interval& x)
{
   val_ &= Range::roundInward(x);
}

RangeDomain* RangeDomain::clone() const
{
   return new RangeDomain(*this);
}

void RangeDomain::print(std::ostream& os) const
{
   os << val_;
}

///////////////////////////////////////////////////////////////////////////////

RangeUnionDomain::RangeUnionDomain(const RangeUnion& u)
      : Domain(),
        val_(u)
{}

RangeUnionDomain::RangeUnionDomain(const std::initializer_list<Range>& l)
      : Domain(),
        val_(l)
{}

const RangeUnion& RangeUnionDomain::getVal() const
{
   return val_;
}

void RangeUnionDomain::setVal(const RangeUnion& u)
{
   val_ = u;
}

bool RangeUnionDomain::isEmpty() const
{
   return val_.isEmpty();
}
 
bool RangeUnionDomain::isInteger() const
{
   return true;
}

Interval RangeUnionDomain::intervalHull() const
{
   return val_.hull().toInterval();
}

void RangeUnionDomain::contractInterval(Interval& x) const
{
   val_.contractInterval(x);
}

void RangeUnionDomain::contract(const Interval& x)
{
   val_.contract(x);
}

RangeUnionDomain* RangeUnionDomain::clone() const
{
   return new RangeUnionDomain(*this);
}

void RangeUnionDomain::print(std::ostream& os) const
{
   os << val_;
}

///////////////////////////////////////////////////////////////////////////////

BinaryDomain::BinaryDomain()
      : Domain(),
        val_()
{}

const ZeroOne& BinaryDomain::getVal() const
{
   return val_;
}

void BinaryDomain::setVal(const ZeroOne& zo)
{
   val_ = zo;
}

bool BinaryDomain::isEmpty() const
{
   return val_.isEmpty();
}

bool BinaryDomain::isBinary() const
{
   return true;
}

Interval BinaryDomain::intervalHull() const
{
   return val_.toRange().toInterval();
}

void BinaryDomain::contractInterval(Interval& x) const
{
   x = round(x) & intervalHull();
}

void BinaryDomain::contract(const Interval& x)
{
   if (!x.contains(0.0))
      val_.setZero(false);

   if (!x.contains(1.0))
      val_.setOne(false);
}

BinaryDomain* BinaryDomain::clone() const
{
   return new BinaryDomain(*this);
}

void BinaryDomain::print(std::ostream& os) const
{
   os << val_;
}

} // namespace
