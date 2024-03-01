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

DomainSlicerMap::DomainSlicerMap()
      : slc_()
{}

DomainSlicerMap::~DomainSlicerMap()
{
   for (DomainSlicer* slc : slc_)
      if (slc != nullptr)
         delete slc;
}

void DomainSlicerMap::setSlicer(DomainType type,
                                std::unique_ptr<DomainSlicer> slc)
{
   int i = static_cast<int>(type);

   if (i >= slc_.size())
   {
      for (int k=slc_.size(); k<=i; ++k)
         slc_.push_back(nullptr);
   }

   if (slc_[i] != nullptr) delete slc_[i];
   slc_[i] = slc.release();
}

DomainSlicer* DomainSlicerMap::getSlicer(DomainType type) const
{
   int i = static_cast<int>(type);
   return (i < slc_.size()) ? slc_[i] : nullptr;
}

bool DomainSlicerMap::hasSlicer(DomainType type) const
{
   return getSlicer(type) != nullptr;
}

} // namespace
