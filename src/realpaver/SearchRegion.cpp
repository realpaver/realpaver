///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "AssertDebug.hpp"
#include "SearchRegion.hpp"

namespace realpaver {

SearchRegion::SearchRegion(Scope sco)
      : sco_(sco),
        doms_(sco.size())
{
   ASSERT(!sco_.isEmpty(), "Creation of a search region with an empty scope");

   for (size_t i=0; i<size(); ++i)
      doms_[i] = sco_.var(i).getDomain()->clone();
}

SearchRegion::SearchRegion(const SearchRegion& reg)
      : sco_(reg.sco_),
        doms_(reg.sco_.size())
{
   for (size_t i=0; i<size(); ++i)
      doms_[i] = reg.doms_[i]->clone();
}

SearchRegion::~SearchRegion()
{
   for (size_t i=0; i<size(); ++i)
      delete doms_[i];
}

Scope SearchRegion::scope() const
{
   return sco_;
}

size_t SearchRegion::size() const
{
   return sco_.size();
}

bool SearchRegion::isEmpty() const
{
   for (size_t i=0; i<size(); ++i)
      if (doms_[i]->isEmpty())
         return true;

   return false;
}

Domain* SearchRegion::getDomain(const Variable& v) const
{
   ASSERT(sco_.contains(v),
          "Bad access in a region to variable " << v.getName());

   return doms_[sco_.index(v)];
}

Domain* SearchRegion::getDomain(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a region at index " << i);

   return doms_[i];
}

Variable SearchRegion::getVar(size_t i) const
{
   ASSERT(i>=0 && i<size(), "Bad access in a region at index " << i);

   return sco_.var(i);
}

SearchRegion* SearchRegion::clone() const
{
   return new SearchRegion(*this);
}

void SearchRegion::print(std::ostream& os) const
{
   for (size_t i=0; i<size(); ++i)
   {
      Variable v = sco_.var(i);
      os << v.getName() << " = " << (*doms_[i]) << std::endl;
   }
}

std::ostream& operator<<(std::ostream& os, const SearchRegion& reg)
{
   reg.print(os);
   return os;
}

} // namespace
