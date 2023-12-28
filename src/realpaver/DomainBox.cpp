///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "DomainBox.hpp"

namespace realpaver {

DomainBox::DomainBox(Scope scop)
      : scop_(scop),
        doms_(scop.size())
{
   ASSERT(!scop_.isEmpty(), "Creation of a domain box with an empty scope");

   for (size_t i=0; i<size(); ++i)
      doms_[i] = scop_.var(i).getDomain()->clone();
}

DomainBox::DomainBox(const DomainBox& box)
      : scop_(box.scop_),
        doms_(box.scop_.size())
{
   for (size_t i=0; i<size(); ++i)
      doms_[i] = box.doms_[i]->clone();
}

DomainBox::~DomainBox()
{
   for (size_t i=0; i<size(); ++i)
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
   for (size_t i=0; i<size(); ++i)
      if (doms_[i]->isEmpty())
         return true;

   return false;
}

Domain* DomainBox::get(const Variable& v) const
{
   ASSERT(scop_.contains(v),
          "Bad access in a domain box to variable " << v.getName());

   return doms_[scop_.index(v)];
}

Domain* DomainBox::get(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return doms_[i];
}

Variable DomainBox::var(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return scop_.var(i);
}

void DomainBox::set(const Variable& v, std::unique_ptr<Domain> p)
{
   ASSERT(scop_.contains(v),
          "Bad access in a domain box to variable " << v.getName());

   ASSERT((p != nullptr) && (!p->isEmpty()), "Bad domain assignment in a box");

   doms_[scop_.index(v)] = p.release();
}

DomainBox* DomainBox::clone() const
{
   return new DomainBox(*this);
}

bool DomainBox::isSplitable(const Variable& v) const
{
   Domain* dom = get(v);

   if (dom->isReal())
      return !v.getTolerance().isTight(dom->intervalHull());

   else
      return !dom->isCanonical();
}

void DomainBox::listPrint(std::ostream& os) const
{
   size_t lmax = scop_.nameMaxLength();

   for (size_t i=0; i<size(); ++i)
   {
      Variable v = scop_.var(i);
      os << v.getName();

      size_t n = v.getName().length();
      for (size_t i=0; i<lmax-n; ++i) os << ' ';

      os << " = " << (*doms_[i]) << std::endl;
   }
}

void DomainBox::vecPrint(std::ostream& os) const
{
   os << '(';
   for (size_t i=0; i<size(); ++i)
   {
      if (i!=0) os << ", ";

      Variable v = scop_.var(i);
      os << v.getName() << " = " << (*doms_[i]);
   }
   os << ')'; 
}

std::ostream& operator<<(std::ostream& os, const DomainBox& box)
{
   box.vecPrint(os);
   return os;
}

double DomainBox::width() const
{
   double wid = 0.0, w;

   for (size_t i=0; i<size(); ++i)
   {
      w = doms_[i]->intervalHull().width();
      if (w > wid) wid = w;
   }

   return wid;
}

double DomainBox::gap(const DomainBox& box) const
{
   return gapOnScope(box, scop_);
}

double DomainBox::gapOnScope(const DomainBox& box, const Scope& scop) const
{
   ASSERT(scop_.contains(scop) && box.scop_.contains(scop),
          "Bad scopes used to calculate the gap between domain boxes");

   double gap = 0.0;
   for (const auto& v : scop)
   {
      double e = get(v)->intervalHull().gap(box.get(v)->intervalHull());
      if (e > gap) gap = e;
   }
   return gap;
}

void DomainBox::glue(const DomainBox& box)
{
   glueOnScope(box, scop_);
}

void DomainBox::glueOnScope(const DomainBox& box, const Scope& scop)
{
   for (const auto& v : scop)
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

double DomainBox::perimeterOnScope(const Scope& scop) const
{
   double p = 0.0;

   for (const auto& v : scop)
      p += get(v)->intervalHull().width();

   return p;
}

double DomainBox::gridPerimeter() const
{
   return gridPerimeterOnScope(scop_);
}

double DomainBox::gridPerimeterOnScope(const Scope& scop) const
{
   double p = 0.0;

   for (const auto& v : scop)
   {
      Interval x = get(v)->intervalHull();
      Tolerance tol = v.getTolerance();

      if (tol.isTight(x))
         p += x.width() / tol.getAbsTol();
   }

   return p;
}

} // namespace
