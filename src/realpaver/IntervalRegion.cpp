///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/IntervalRegion.hpp"

namespace realpaver {

IntervalRegion::IntervalRegion(Scope sco, const Interval& x)
      : IntervalVector(sco.size(), x),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval region");   
}

IntervalRegion::IntervalRegion(Scope sco, const IntervalVector& X)
      : IntervalVector(X),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval region");      
   ASSERT(sco.size() == X.size(), "Bad initialization of an interval region");      
}

IntervalRegion::IntervalRegion(const RealPoint& pt)
      : IntervalVector(pt),
        scope_(pt.scope())
{}


IntervalRegion::IntervalRegion(Scope sco, const RealVector& X)
      : IntervalVector(X),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval region");      
   ASSERT(sco.size() == X.size(), "Bad initialization of an interval region");      
}

Scope IntervalRegion::scope() const
{
   return scope_;
}

Interval IntervalRegion::get(const Variable& v) const
{
   ASSERT(scope_.contains(v), "Bad access in an interval region");

   return operator[](scope_.index(v));
}

void IntervalRegion::set(const Variable& v, const Interval& x)
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

bool IntervalRegion::contains(const IntervalRegion& reg) const
{
   for (auto v : reg.scope())
      if (!get(v).contains(reg.get(v))) return false;

   return true;
}

bool IntervalRegion::strictlyContains(const IntervalRegion& reg) const
{
   for (auto v : reg.scope())
      if (!get(v).strictlyContains(reg.get(v))) return false;

   return true;
}

bool IntervalRegion::contains(const RealPoint& pt) const
{
   for (auto v : pt.scope())
      if (!get(v).contains(pt.get(v))) return false;

   return true;
}

bool IntervalRegion::strictlyContains(const RealPoint& pt) const
{
   for (auto v : pt.scope())
      if (!get(v).strictlyContains(pt.get(v))) return false;

   return true;
}

bool IntervalRegion::overlaps(const IntervalRegion& reg) const
{
   if (size() != reg.size()) return false;

   for (auto v : scope_)
      if (!get(v).overlaps(reg.get(v))) return false;

   return true;
}

void IntervalRegion::hullAssignOnScope(const IntervalRegion& reg,
                                       const Scope& sco)
{
   for (auto v : sco) set(v, get(v) | reg.get(v));
}

void IntervalRegion::setOnScope(const IntervalRegion& reg, const Scope& sco)
{
   for (auto v : sco) set(v, reg.get(v));
}

RealPoint IntervalRegion::midpointOnScope(const Scope& sco) const
{
   RealPoint pt(sco);
   for (auto v : sco) pt.set(v, get(v).midpoint());
   return pt;
}

IntervalRegion IntervalRegion::subRegion(const Scope& sco) const
{
   ASSERT(scope_.contains(sco),
          "Bad scope used to create a sub-region " << sco);

   IntervalRegion reg(sco);
   for (auto v : sco) reg.set(v, get(v));
   return reg;
}

double IntervalRegion::distance(const IntervalRegion& reg) const
{
   return distanceOnScope(reg, scope_);
}

double IntervalRegion::distanceOnScope(const IntervalRegion& reg,
                                       const Scope& sco) const
{
   ASSERT(scope_.contains(sco) && reg.scope_.contains(sco),
          "Bad scopes used to calculate the distance between interval regions");

   double d = 0.0;
   for (auto v : sco)
   {
      double e = get(v).distance(reg.get(v));
      if (e > d) d = e;
   }
   return d;
}

double IntervalRegion::gap(const IntervalRegion& reg) const
{
   return gapOnScope(reg, scope_);
}

double IntervalRegion::gapOnScope(const IntervalRegion& reg,
                                       const Scope& sco) const
{
   ASSERT(scope_.contains(sco) && reg.scope_.contains(sco),
          "Bad scopes used to calculate the gap between interval regions");

   double gap = 0.0;
   for (auto v : sco)
   {
      double e = get(v).gap(reg.get(v));
      if (e > gap) gap = e;
   }
   return gap;
}

double IntervalRegion::perimeter() const
{
   return perimeterOnScope(scope_);
}

double IntervalRegion::perimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : scope_)
      p += get(v).width();

   return p;
}

double IntervalRegion::gridPerimeter() const
{
   return gridPerimeterOnScope(scope_);
}

double IntervalRegion::gridPerimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : scope_)
   {
      Interval x = get(v);
      Tolerance tol = v.getTolerance();

      if (!tol.hasTolerance(x))
      {
         if (tol.isAbsolute())
            p += x.width() / tol.getVal();

         else
            p += x.relWidth() / tol.getVal();
      }
   }

   return p;
}

IntervalRegion* IntervalRegion::clone() const
{
   return new IntervalRegion(*this);
}

void IntervalRegion::print(std::ostream& os) const
{
   stdPrint(os);
}

void IntervalRegion::stdPrint(std::ostream& os) const
{
   size_t lmax = scope_.maxVarLength();

   for (const auto& v : scope_)
   {
      os << v.getName();
      size_t n = v.getName().length();
      for (size_t i=0; i<lmax-n; ++i) os << ' ';
      os << " = " << get(v) << std::endl;
   }
}

void IntervalRegion::vecPrint(std::ostream& os) const
{
   IntervalVector::print(os);
}

} // namespace
