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

///////////////////////////////////////////////////////////////////////////////

IntervalDomain::IntervalDomain(const Interval& x)
   : Domain(),
     val_(x)
{}

bool IntervalDomain::isEmpty() const
{
   return val_.isEmpty();
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

void IntervalDomain::contract(Interval& x)
{
   x &= val_;
}

IntervalDomain* IntervalDomain::clone() const
{
   return new IntervalDomain(*this);
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
 
Interval IntervalUnionDomain::intervalHull() const
{
   return val_.hull();
}

void IntervalUnionDomain::contract(Interval& x)
{
   val_.contract(x);
}

IntervalUnionDomain* IntervalUnionDomain::clone() const
{
   return new IntervalUnionDomain(*this);
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

Interval RangeDomain::intervalHull() const
{
   return val_.toInterval();
}

void RangeDomain::contract(Interval& x)
{
   x = round(x) & intervalHull();
}

RangeDomain* RangeDomain::clone() const
{
   return new RangeDomain(*this);
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

Interval BinaryDomain::intervalHull() const
{
   return val_.toRange().toInterval();
}

void BinaryDomain::contract(Interval& x)
{
   x = round(x) & intervalHull();
}

BinaryDomain* BinaryDomain::clone() const
{
   return new BinaryDomain(*this);
}
   
} // namespace
