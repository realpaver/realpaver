///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/IntervalRegion.hpp"

namespace realpaver {

IntervalRegion::IntervalRegion(const Scope& s, const Interval& x)
      : IntervalVector(s.size(), x),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create an interval region");   
}

IntervalRegion::IntervalRegion(const Scope& s, const IntervalVector& X)
      : IntervalVector(X),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create an interval region");      
   ASSERT(s.size() == X.size(), "Bad initialization of an interval region");      
}

Scope IntervalRegion::scope() const
{
   return scope_;
}

Interval IntervalRegion::get(Variable v) const
{
   ASSERT(scope_.contains(v), "Bad access in an interval region");

   return operator[](scope_.index(v));
}

void IntervalRegion::set(Variable v, const Interval& x)
{
   ASSERT(scope_.contains(v), "Bad access in an interval region");

   operator[](scope_.index(v)) = x;
}

RealPoint IntervalRegion::midpoint() const
{
   return RealPoint(scope_, IntervalVector::midpoint());
}

RealPoint IntervalRegion::lCorner() const
{
   return RealPoint(scope_, IntervalVector::lCorner());   
}

RealPoint IntervalRegion::rCorner() const
{
   return RealPoint(scope_, IntervalVector::rCorner());   
}

RealPoint IntervalRegion::corner(const Bitset& bs) const
{
   return RealPoint(scope_, IntervalVector::corner(bs));
}

RealPoint IntervalRegion::oppositeCorner(const Bitset& bs) const
{
   return RealPoint(scope_, IntervalVector::oppositeCorner(bs));
}

void IntervalRegion::hullAssignOnScope(const IntervalRegion& reg,
                                       const Scope& s)
{
   for (auto v : s) set(v, get(v) | reg.get(v));
}

void IntervalRegion::setOnScope(const IntervalRegion& reg, const Scope& s)
{
   for (auto v : s) set(v, reg.get(v));
}

RealPoint IntervalRegion::midpointOnScope(const Scope& s) const
{
   RealPoint pt(s);
   for (auto v : s) pt.set(v, get(v).midpoint());
   return pt;
}

IntervalRegion IntervalRegion::subRegion(const Scope& s) const
{
   ASSERT(scope_.contains(s), "Bad scope used to create a sub-region " << s);

   IntervalRegion reg(s);
   for (auto v : s) reg.set(v, get(v));
   return reg;
}

IntervalRegion* IntervalRegion::clone() const
{
   return new IntervalRegion(*this);
}

} // namespace

