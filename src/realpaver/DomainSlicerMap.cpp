///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/DomainSlicerMap.hpp"

namespace realpaver {

DomainSlicerMap::DomainSlicerMap(Scope sco)
      : sco_(sco),
        sli_(sco.size(), nullptr)
{
   ASSERT(!sco.isEmpty(),
          "Creation of a domain slicer map with an empty scope");
}

DomainSlicerMap::~DomainSlicerMap()
{
   for (DomainSlicer* slicer : sli_)
      if (slicer != nullptr)
         delete slicer;
}

void DomainSlicerMap::setSlicer(Variable v,
                                std::unique_ptr<DomainSlicer> pslicer)
{
   ASSERT(sco_.contains(v), "Bad assignment in a domain slicer map");

   size_t i = sco_.index(v);
   if (sli_[i] != nullptr) delete sli_[i];

   sli_[i] = pslicer.release();
}

DomainSlicer* DomainSlicerMap::getSlicer(const Variable& v) const
{
   ASSERT(sco_.contains(v),
          "Variable " << v.getName() << " not handled by the domain slicer map");

   return sli_[sco_.index(v)];
}

} // namespace
