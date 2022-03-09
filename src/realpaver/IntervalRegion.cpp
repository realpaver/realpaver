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
{}

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

void IntervalRegion::hullAssignOnScope(const IntervalRegion& reg,
                                       const Scope& s)
{
   for (auto v : s) set(v, get(v) | reg.get(v));
}

void IntervalRegion::setOnScope(const IntervalRegion& reg, const Scope& s)
{
   for (auto v : s) set(v, reg.get(v));
}

RealVector IntervalRegion::midpointOnScope(const Scope& s) const
{
   RealVector v(s.size());
   toMidpointOnScope(s, v);
   return v;
}

void IntervalRegion::toMidpointOnScope(const Scope& s, RealVector& mid) const
{
   for (auto v : s) mid.set(s.index(v), get(v).midpoint());
}

IntervalRegion* IntervalRegion::clone() const
{
   return new IntervalRegion(*this);
}

} // namespace

