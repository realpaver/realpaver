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

Domain* DomainBox::getDomain(const Variable& v) const
{
   ASSERT(sco_.contains(v),
          "Bad access in a domain box to variable " << v.getName());

   return doms_[sco_.index(v)];
}

Domain* DomainBox::getDomain(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return doms_[i];
}

Variable DomainBox::getVar(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a domain box at index " << i);

   return sco_.var(i);
}

DomainBox* DomainBox::clone() const
{
   return new DomainBox(*this);
}

bool DomainBox::isSplitable(const Variable& v) const
{
   Domain* dom = getDomain(v);

   if (dom->isReal())
      return !v.getTolerance().hasTolerance(dom->intervalHull());

   else
      return !dom->isCanonical();
}

void DomainBox::print(std::ostream& os) const
{
   for (size_t i=0; i<size(); ++i)
   {
      Variable v = sco_.var(i);
      os << v.getName() << " = " << (*doms_[i]) << std::endl;
   }
}

std::ostream& operator<<(std::ostream& os, const DomainBox& box)
{
   box.print(os);
   return os;
}

} // namespace
