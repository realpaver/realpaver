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

DomainBox::DomainBox(Scope sco)
      : sco_(sco),
        doms_(sco.size())
{
   ASSERT(!sco_.isEmpty(), "Creation of a domain box with an empty scope");

   for (size_t i=0; i<size(); ++i)
      doms_[i] = sco_.var(i).getDomain()->clone();
}

DomainBox::DomainBox(const DomainBox& box)
      : sco_(box.sco_),
        doms_(box.sco_.size())
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
   return sco_;
}

size_t DomainBox::size() const
{
   return sco_.size();
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
   ASSERT(sco_.contains(v),
          "Bad access in a domain box to variable " << v.getName());

   return doms_[sco_.index(v)];
}

Domain* DomainBox::get(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return doms_[i];
}

Variable DomainBox::var(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return sco_.var(i);
}

void DomainBox::set(const Variable& v, std::unique_ptr<Domain> p)
{
   ASSERT(sco_.contains(v),
          "Bad access in a domain box to variable " << v.getName());

   ASSERT((p != nullptr) && (!p->isEmpty()), "Bad domain assignment in a box");

   doms_[sco_.index(v)] = p.release();
}

DomainBox* DomainBox::clone() const
{
   return new DomainBox(*this);
}

bool DomainBox::isSplitable(const Variable& v) const
{
   Domain* dom = get(v);

   if (dom->isReal())
      return !v.getTolerance().hasTolerance(dom->intervalHull());

   else
      return !dom->isCanonical();
}

void DomainBox::listPrint(std::ostream& os) const
{
   size_t lmax = sco_.nameMaxLength();

   for (size_t i=0; i<size(); ++i)
   {
      Variable v = sco_.var(i);
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

      Variable v = sco_.var(i);
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

double DomainBox::gap(const DomainBox& B) const
{
   return gapOnScope(B, sco_);
}

double DomainBox::gapOnScope(const DomainBox& B, const Scope& sco) const
{
   ASSERT(sco_.contains(sco) && B.sco_.contains(sco),
          "Bad scopes used to calculate the gap between domain boxes");

   double gap = 0.0;
   for (const auto& v : sco)
   {
      double e = get(v)->intervalHull().gap(B.get(v)->intervalHull());
      if (e > gap) gap = e;
   }
   return gap;
}

void DomainBox::glue(const DomainBox& B)
{
   glueOnScope(B, sco_);
}

void DomainBox::glueOnScope(const DomainBox& B, const Scope& sco)
{
   for (const auto& v : sco)
   {
      Interval x = get(v)->intervalHull() | B.get(v)->intervalHull();
      std::unique_ptr<IntervalDomain> dom(new IntervalDomain(x));
      set(v, std::move(dom));
   }
}

double DomainBox::perimeter() const
{
   return perimeterOnScope(sco_);
}

double DomainBox::perimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : sco)
      p += get(v)->intervalHull().width();

   return p;
}

double DomainBox::gridPerimeter() const
{
   return gridPerimeterOnScope(sco_);
}

double DomainBox::gridPerimeterOnScope(const Scope& sco) const
{
   double p = 0.0;

   for (const auto& v : sco)
   {
      Interval x = get(v)->intervalHull();
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

} // namespace
