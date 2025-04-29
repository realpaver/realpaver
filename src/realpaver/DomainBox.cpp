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
 * @file   DomainBox.cpp
 * @brief  Box of domains
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "DomainBox.hpp"
#include "AssertDebug.hpp"
#include "IntervalBox.hpp"
#include "Logger.hpp"

namespace realpaver {

DomainBox::DomainBox(Scope scop)
    : scop_(scop)
    , doms_(scop.size())
{
   ASSERT(!scop_.isEmpty(), "Creation of a domain box with an empty scope");

   for (size_t i = 0; i < size(); ++i)
      doms_[i] = scop_.var(i).getDomain()->clone();
}

DomainBox::DomainBox(const DomainBox &box)
    : scop_(box.scop_)
    , doms_(box.scop_.size())
{
   for (size_t i = 0; i < size(); ++i)
      doms_[i] = box.doms_[i]->clone();
}

DomainBox::DomainBox(const IntervalBox &B)
    : scop_(B.scope())
    , doms_(B.scope().size())
{
   for (size_t i = 0; i < size(); ++i)
      doms_[i] = new IntervalDomain(B.get(scop_.var(i)));
}

DomainBox::~DomainBox()
{
   for (size_t i = 0; i < size(); ++i)
      delete doms_[i];
}

Scope DomainBox::scope() const
{
   return scop_;
}

size_t DomainBox::size() const
{
   return scop_.size();
}

bool DomainBox::isEmpty() const
{
   for (size_t i = 0; i < size(); ++i)
      if (doms_[i]->isEmpty())
         return true;

   return false;
}

Domain *DomainBox::get(const Variable &v) const
{
   ASSERT(scop_.contains(v), "Bad access in a domain box to variable " << v.getName());

   return doms_[scop_.index(v)];
}

Domain *DomainBox::get(size_t i) const
{
   ASSERT(i >= 0 && i < size(), "Bad access in a domain box at index " << i);

   return doms_[i];
}

Variable DomainBox::var(size_t i) const
{
   ASSERT(i >= 0 && i < size(), "Bad access in a domain box at index " << i);

   return scop_.var(i);
}

void DomainBox::set(const Variable &v, std::unique_ptr<Domain> p)
{
   ASSERT(scop_.contains(v), "Bad access in a domain box to variable " << v.getName());

   ASSERT((p != nullptr) && (!p->isEmpty()), "Bad domain assignment in a box");

   size_t i = scop_.index(v);
   delete doms_[i];
   doms_[i] = p.release();
}

DomainBox *DomainBox::clone() const
{
   return new DomainBox(*this);
}

bool DomainBox::isSplitable(const Variable &v) const
{
   Domain *dom = get(v);

   if (dom->isReal())
      return !v.getTolerance().isTight(dom->intervalHull());

   else
      return !dom->isCanonical();
}

void DomainBox::print(std::ostream &os) const
{
   os << '(';
   for (size_t i = 0; i < size(); ++i)
   {
      if (i != 0)
         os << ", ";

      Variable v = scop_.var(i);
      os << v.getName() << " = " << (*doms_[i]);
   }
   os << ')';
}

std::ostream &operator<<(std::ostream &os, const DomainBox &box)
{
   box.print(os);
   return os;
}

double DomainBox::width() const
{
   double wid = 0.0, w;

   for (size_t i = 0; i < size(); ++i)
   {
      w = doms_[i]->intervalHull().width();
      if (w > wid)
         wid = w;
   }

   return wid;
}

double DomainBox::gap(const DomainBox &box) const
{
   return gapOnScope(box, scop_);
}

double DomainBox::gapOnScope(const DomainBox &box, const Scope &scop) const
{
   ASSERT(scop_.contains(scop) && box.scop_.contains(scop),
          "Bad scopes used to calculate the gap between domain boxes");

   double gap = 0.0;
   for (const auto &v : scop)
   {
      double e = get(v)->intervalHull().gap(box.get(v)->intervalHull());
      if (e > gap)
         gap = e;
   }
   return gap;
}

void DomainBox::glue(const DomainBox &box)
{
   glueOnScope(box, scop_);
}

void DomainBox::glueOnScope(const DomainBox &box, const Scope &scop)
{
   for (const auto &v : scop)
   {
      Interval x = get(v)->intervalHull() | box.get(v)->intervalHull();
      std::unique_ptr<IntervalDomain> dom(new IntervalDomain(x));
      set(v, std::move(dom));
   }
}

double DomainBox::perimeter() const
{
   return perimeterOnScope(scop_);
}

double DomainBox::perimeterOnScope(const Scope &scop) const
{
   double p = 0.0;

   for (const auto &v : scop)
      p += get(v)->intervalHull().width();

   return p;
}

double DomainBox::gridPerimeter() const
{
   return gridPerimeterOnScope(scop_);
}

double DomainBox::gridPerimeterOnScope(const Scope &scop) const
{
   double p = 0.0;

   LOG_FULL("Grid perimeter");

   for (const auto &v : scop)
   {
      Interval x = get(v)->intervalHull();
      Tolerance tol = v.getTolerance();

      if (!tol.isTight(x))
      {
         double w = x.width();
         double z = w / tol.getAbsTol();

         LOG_FULL("   > " << v.getName() << " : " << w << " / " << tol.getAbsTol()
                          << " = " << z);

         p += z;
      }
   }

   LOG_FULL("   > " << p);

   return p;
}

bool DomainBox::equals(const DomainBox &box) const
{
   if (scop_ != box.scop_)
      return false;

   for (size_t i = 0; i < doms_.size(); ++i)
      if (!doms_[i]->equals(*box.doms_[i]))
         return false;

   return true;
}

} // namespace realpaver
