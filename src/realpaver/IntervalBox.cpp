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
 * @file   IntervalBox.cpp
 * @brief  Class of interval boxes
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainBox.hpp"
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

IntervalBox::IntervalBox(Scope scop)
      : IntervalVector(scop.size()),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create an interval box");   

   for (const auto& v : scop)
      set(v, v.getDomain()->intervalHull());
}

IntervalBox::IntervalBox(Scope scop, const Interval& x)
      : IntervalVector(scop.size(), x),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create an interval box");   
}

IntervalBox::IntervalBox(Scope scop, const IntervalVector& X)
      : IntervalVector(X),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create an interval box");      
   ASSERT(scop.size() == X.size(), "Bad initialization of an interval box");      
}

IntervalBox::IntervalBox(const RealPoint& pt)
      : IntervalVector(pt),
        scop_(pt.scope())
{}

IntervalBox::IntervalBox(Scope scop, const RealVector& X)
      : IntervalVector(X),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create an interval box");      
   ASSERT(scop.size() == X.size(), "Bad initialization of an interval box");      
}

IntervalBox::IntervalBox(const IntervalBox& B, Scope scop)
      : IntervalVector(scop.size()),
        scop_(scop)
{
   ASSERT(B.scope().contains(scop), "Bad scope used to create a sub-box");

   for (const auto& v : scop)
      set(v, B.get(v));
}

IntervalBox::IntervalBox(const DomainBox& box)
      : IntervalVector(box.scope().size()),
        scop_(box.scope())
{
   for (const auto& v : scop_)
      set(v, box.get(v)->intervalHull());
}

Scope IntervalBox::scope() const
{
   return scop_;
}

Interval IntervalBox::get(const Variable& v) const
{
   ASSERT(scop_.contains(v), "Bad access in an interval box");

   return operator[](scop_.index(v));
}

void IntervalBox::set(const Variable& v, const Interval& x)
{
   ASSERT(scop_.contains(v), "Bad access in an interval box");

   operator[](scop_.index(v)) = x;
}

RealPoint IntervalBox::midpoint() const
{
   return RealPoint(scop_, IntervalVector::midpoint());
}

RealPoint IntervalBox::lCorner() const
{
   return RealPoint(scop_, IntervalVector::lCorner());   
}

RealPoint IntervalBox::rCorner() const
{
   return RealPoint(scop_, IntervalVector::rCorner());   
}

RealPoint IntervalBox::corner(const Bitset& bs) const
{
   return RealPoint(scop_, IntervalVector::corner(bs));
}

RealPoint IntervalBox::oppositeCorner(const Bitset& bs) const
{
   return RealPoint(scop_, IntervalVector::oppositeCorner(bs));
}

bool IntervalBox::contains(const IntervalBox& B) const
{
   for (const auto& v : B.scope())
      if (!get(v).contains(B.get(v))) return false;

   return true;
}

bool IntervalBox::strictlyContains(const IntervalBox& B) const
{
   for (const auto& v : B.scope())
      if (!get(v).strictlyContains(B.get(v))) return false;

   return true;
}

bool IntervalBox::contains(const RealPoint& pt) const
{
   for (const auto& v : pt.scope())
      if (!get(v).contains(pt.get(v))) return false;

   return true;
}

bool IntervalBox::strictlyContains(const RealPoint& pt) const
{
   for (const auto& v : pt.scope())
      if (!get(v).strictlyContains(pt.get(v))) return false;

   return true;
}

bool IntervalBox::overlaps(const IntervalBox& B) const
{
   if (size() != B.size()) return false;

   for (const auto& v : scop_)
      if (!get(v).overlaps(B.get(v))) return false;

   return true;
}

void IntervalBox::glue(const IntervalBox& B)
{
   glueOnScope(B, scop_);
}

void IntervalBox::glueOnScope(const IntervalBox& B, const Scope& scop)
{
   for (const auto& v : scop)
      set(v, get(v) | B.get(v));
}

void IntervalBox::setOnScope(const IntervalBox& B, const Scope& scop)
{
   for (const auto& v : scop)
      set(v, B.get(v));
}

RealPoint IntervalBox::midpointOnScope(const Scope& scop) const
{
   RealPoint pt(scop);
   midpointOnScope(scop, pt);
   return pt;
}

void IntervalBox::midpointOnScope(const Scope& scop, RealPoint& pt) const
{
   ASSERT(scop_.contains(scop),
          "Bad scope used t create the midpoint of a box" << scop);

   ASSERT(pt.scope().contains(scop),
          "Bad scope used t create the midpoint of a box" << scop);

   for (const auto& v : scop)
      pt.set(v, get(v).midpoint());
}


IntervalBox IntervalBox::subRegion(const Scope& scop) const
{
   ASSERT(scop_.contains(scop),
          "Bad scope used to create a sub-box " << scop);

   IntervalBox B(scop);
   for (const auto& v : scop) B.set(v, get(v));
   return B;
}

double IntervalBox::distance(const IntervalBox& B) const
{
   return distanceOnScope(B, scop_);
}

double IntervalBox::distanceOnScope(const IntervalBox& B,
                                    const Scope& scop) const
{
   ASSERT(scop_.contains(scop) && B.scop_.contains(scop),
          "Bad scopes used to calculate the distance between interval boxes");

   double d = 0.0;
   for (const auto& v : scop)
   {
      double e = get(v).distance(B.get(v));
      if (e > d) d = e;
   }
   return d;
}

double IntervalBox::gap(const IntervalBox& B) const
{
   return gapOnScope(B, scop_);
}

double IntervalBox::gapOnScope(const IntervalBox& B, const Scope& scop) const
{
   ASSERT(scop_.contains(scop) && B.scop_.contains(scop),
          "Bad scopes used to calculate the gap between interval boxes");

   double gap = 0.0;
   for (const auto& v : scop)
   {
      double e = get(v).gap(B.get(v));
      if (e > gap) gap = e;
   }
   return gap;
}

void IntervalBox::inflateOnScope(const Scope& scop, double delta, double chi)
{
   ASSERT(delta > 1.0, "Bad parameter delta of inflation: " << delta);
   ASSERT(chi > 0.0, "Bad parameter chi of inflation: " << chi);

   for (const auto& v : scop)
      set(v, get(v).inflate(delta, chi));
}

double IntervalBox::perimeter() const
{
   return perimeterOnScope(scop_);
}

double IntervalBox::perimeterOnScope(const Scope& scop) const
{
   double p = 0.0;

   for (const auto& v : scop)
      p += get(v).width();

   return p;
}

double IntervalBox::gridPerimeter() const
{
   return gridPerimeterOnScope(scop_);
}

double IntervalBox::gridPerimeterOnScope(const Scope& scop) const
{
   double p = 0.0;

   LOG_FULL("Grid perimeter");

   for (const auto& v : scop)
   {
      Interval x = get(v);
      Tolerance tol = v.getTolerance();

      if (!tol.isTight(x))
      {
         double w = x.width();
         double z = w / tol.getAbsTol();
         
         LOG_FULL("   > " << v.getName() << " : " << w << " / "
                          << tol.getAbsTol() << " = " << z);
         
         p += z;
      }      
   }

   LOG_FULL("   > " << p);

   return p;
}

IntervalBox* IntervalBox::clone() const
{
   return new IntervalBox(*this);
}

double IntervalBox::gainRatio(const IntervalBox& B) const
{
   return gainRatioOnScope(B, scop_);
}

double IntervalBox::gainRatioOnScope(const IntervalBox& B,
                                     const Scope& scop) const
{
   ASSERT(scop_.contains(scop),
          "Bad scope used to calculate a gan ratio " << scop);
   ASSERT(B.scop_.contains(scop),
          "Bad scope used to calculate a gan ratio " << scop);
   ASSERT(!scop.isEmpty(),
          "Empty scope used to calculate a gan ratio");

   double s = 0.0;

   for (const auto& v : scop)
      s += (1.0 - get(v).width() / B.get(v).width());

   return s / scop.size();
}

void IntervalBox::print(std::ostream& os) const
{
   vecPrint(os);
}

void IntervalBox::listPrint(std::ostream& os) const
{
   size_t lmax = scop_.nameMaxLength();

   for (const auto& v : scop_)
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

      Variable v = scop_.var(i);
      os << v.getName() << " = " << get(v);
   }
   os << ')'; 
}

bool IntervalBox::isSplitable(const Variable& v) const
{
      return !v.getTolerance().isTight(get(v));
}

} // namespace
