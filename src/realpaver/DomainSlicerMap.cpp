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
 * @file   DomainSlicerMap.cpp
 * @brief  Map of domain slicers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/DomainSlicerMap.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

DomainSlicerMap::DomainSlicerMap()
    : slc_()
{
}

DomainSlicerMap::~DomainSlicerMap()
{
   for (DomainSlicer *slc : slc_)
      if (slc != nullptr)
         delete slc;
}

void DomainSlicerMap::setSlicer(DomainType type, std::unique_ptr<DomainSlicer> slc)
{
   int i = static_cast<int>(type);

   if (i >= (int)slc_.size())
   {
      for (int k = slc_.size(); k <= i; ++k)
         slc_.push_back(nullptr);
   }

   if (slc_[i] != nullptr)
      delete slc_[i];
   slc_[i] = slc.release();
}

DomainSlicer *DomainSlicerMap::getSlicer(DomainType type) const
{
   int i = static_cast<int>(type);
   return (i < (int)slc_.size()) ? slc_[i] : nullptr;
}

bool DomainSlicerMap::hasSlicer(DomainType type) const
{
   return getSlicer(type) != nullptr;
}

} // namespace realpaver
