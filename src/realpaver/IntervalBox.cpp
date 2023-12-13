///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/IntervalBox.hpp"

namespace realpaver {

IntervalBox::IntervalBox(Scope sco)
      : IntervalVector(sco.size()),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval box");   

   for (const auto& v : sco)
      set(v, v.getDomain()->intervalHull());
}

IntervalBox::IntervalBox(Scope sco, const Interval& x)
      : IntervalVector(sco.size(), x),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval box");   
}

IntervalBox::IntervalBox(Scope sco, const IntervalVector& X)
      : IntervalVector(X),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval box");      
   ASSERT(sco.size() == X.size(), "Bad initialization of an interval box");      
}

IntervalBox::IntervalBox(const RealPoint& pt)
      : IntervalVector(pt),
        scope_(pt.scope())
{}

IntervalBox::IntervalBox(Scope sco, const RealVector& X)
      : IntervalVector(X),
        scope_(sco)
{
   ASSERT(!sco.isEmpty(), "Empty scope used to create an interval box");      
   ASSERT(sco.size() == X.size(), "Bad initialization of an interval box");      
}

IntervalBox::IntervalBox(const IntervalBox& B, Scope sco)
      : IntervalVector(sco.size()),
        scope_(sco)
{
   ASSERT(B.scope().contains(sco), "Bad scope used to create a sub-box");

   for (Variable v : sco)
      set(v, B.get(v));
}

IntervalBox::IntervalBox(const DomainBox& box)
      : IntervalVector(box.scope().size()),
        scope_(box.scope())
{
   for (Variable v : scope_)
      set(v, box.get(v)->intervalHull());
}

Scope IntervalBox::scope() const
{
   return scope_;
}

Interval IntervalBox::get(const Variable& v) const
{
   ASSERT(scope_.contains(v), "Bad access in an interval box");

   return operator[](scope_.index(v));
}

void IntervalBox::set(const Variable& v, const Interval& x)
{
   ASSERT(scope_.contains(v), "Bad access in an interval box");

   operator[](scope_.index(v)) = x;
}

RealPoint IntervalBox::midpoint() const
{
   return RealPoint(scope_, IntervalVector::midpoint());
}

RealPoint IntervalBox::lCorner() const
{
   return RealPoint(scope_, IntervalVector::lCorner());   
}

RealPoint IntervalBox::rCorner() const
{
   return RealPoint(scope_, IntervalVector::rCorner());   
}

RealPoint IntervalBox::corner(const Bitset& bs) const
{
   return RealPoint(scope_, IntervalVector::corner(bs));
}

RealPoint IntervalBox::oppositeCorner(const Bitset& bs) const
{
   return RealPoint(scope_, IntervalVector::oppositeCorner(bs));
}

bool IntervalBox::contains(const IntervalBox& B) const
{
   for (auto v : B.scope())
      if (!get(v).contains(B.get(v))) return false;

   return true;
}

bool IntervalBox::strictlyContains(const IntervalBox& B) const
{
   for (auto v : B.scope())
      if (!get(v).strictlyContains(B.get(v))) return false;

   return true;
}

bool IntervalBox::contains(const RealPoint& pt) const
{
   for (auto v : pt.scope())
      if (!get(v).contains(pt.get(v))) return false;

   return true;
}

bool IntervalBox::strictlyContains(const RealPoint& pt) const
{
   for (auto v : pt.scope())
      if (!get(v).strictlyContains(pt.get(v))) return false;

   return true;
}

bool IntervalBox::overlaps(const IntervalBox& B) const
{
   if (size() != B.size()) return false;

   for (auto v : scope_)
      if (!get(v).overlaps(B.get(v))) return false;

   return true;
}

void IntervalBox::glue(const IntervalBox& B)
{
   glueOnScope(B, scope_);
}

void IntervalBox::glueOnScope(const IntervalBox& B, const Scope& sco)
{
   for (auto v : sco) set(v, get(v) | B.get(v));
}

void IntervalBox::setOnScope(const IntervalBox& B, const Scope& sco)
{
   for (auto v : sco) set(v, B.get(v));
}

RealPoint IntervalBox::midpointOnScope(const Scope& sco) const
{
   RealPoint pt(sco);
   midpointOnScope(sco, pt);
   return pt;
}

void IntervalBox::midpointOnScope(const Scope& sco, RealPoint& pt) const
{
   ASSERT(scope_.contains(sco),
          "Bad scope used t create the midpoint of a box" << sco);

   ASSERT(pt.scope().contains(sco),
          "Bad scope used t create the midpoint of a box" << sco);

   for (auto v : sco)
      pt.set(v, get(v).midpoint());
}


IntervalBox IntervalBox::subRegion(const Scope& sco) const
{
   ASSERT(scope_.contains(sco),
          "Bad scope used to create a sub-box " << sco);

   IntervalBox B(sco);
   for (auto v : sco) B.set(v, get(v));
   return B;
}

double IntervalBox::distance(const IntervalBox& B) const
{
   return distanceOnScope(B, scope_);
}

double IntervalBox::distanceOnScope(const IntervalBox& B,
                                    const Scope& sco) const
{
   ASSERT(scope_.contains(sco) && B.scope_.contains(sco),
          "Bad scopes used to calculate the distance between interval boxes");

   double d = 0.0;
   for (auto v : sco)
   {
      double e = get(v).distance(B.get(v));
      if (e > d) d = e;
   }
   return d;
}

double IntervalBox::gap(const IntervalBox& B) const
{
   return gapOnScope(B, scope_);
}

double IntervalBox::gapOnScope(const IntervalBox& B, const Scope& sco) const
{
   ASSERT(scope_.contains(sco) && B.scope_.contains(sco),
          "Bad scopes used to calculate the gap between interval boxes");

   double gap = 0.0;
   for (auto v : sco)
   {
      double e = get(v).gap(B.get(v));
      if (e > gap) gap = e;
   }
   return gap;
}

void IntervalBox::inflateOnScope(const Scope& sco, double delta, double chi)
{
   ASSERT(delta > 1.0, "Bad parameter delta of inflation: " << delta);
   ASSERT(chi > 0.0, "Bad parameter chi of inflation: " << chi);

   for (auto v : sco)
      set(v, get(v).inflate(delta, chi));
}

double IntervalBox::perimeter() const
{
   return perimeterOnScope(scope_);
}

double IntervalBox::perimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : sco)
      p += get(v).width();

   return p;
}

double IntervalBox::gridPerimeter() const
{
   return gridPerimeterOnScope(scope_);
}

double IntervalBox::gridPerimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : sco)
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

IntervalBox* IntervalBox::clone() const
{
   return new IntervalBox(*this);
}

void IntervalBox::print(std::ostream& os) const
{
   vecPrint(os);
}

void IntervalBox::listPrint(std::ostream& os) const
{
   size_t lmax = scope_.nameMaxLength();

   for (const auto& v : scope_)
   {
      os << v.getName();
      size_t n = v.getName().length();
      for (size_t i=0; i<lmax-n; ++i) os << ' ';
      os << " = " << get(v) << std::endl;
   }
}

void IntervalBox::vecPrint(std::ostream& os) const
{
   os << '(';
   for (size_t i=0; i<size(); ++i)
   {
      if (i!=0) os << ", ";

      Variable v = scope_.var(i);
      os << v.getName() << " = " << get(v);
   }
   os << ')'; 
}

} // namespace
