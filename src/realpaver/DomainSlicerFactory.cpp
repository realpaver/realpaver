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
 * @file   DomainSlicerFactory.cpp
 * @brief  Factory of domain slicer maps
 * @author Laurent Granvilliers
 * @date   2023-11-19
*/

#include "realpaver/DomainSlicerFactory.hpp"

namespace realpaver {

std::unique_ptr<DomainSlicerMap> DomainSlicerFactory::makeBisectionStrategy()
{
   std::unique_ptr<DomainSlicerMap> smap = std::make_unique<DomainSlicerMap>();
   std::unique_ptr<DomainSlicer> slc;

   // binary
   slc = std::make_unique<BinaryDomainSlicer>();
   smap->setSlicer(DomainType::Binary, std::move(slc));

   // interval
   slc = std::make_unique<IntervalDomainBisecter>();
   smap->setSlicer(DomainType::Interval, std::move(slc));

   // interval union
   slc = std::make_unique<IntervalUnionDomainBisecter>();
   smap->setSlicer(DomainType::IntervalUnion, std::move(slc));

   // range
   slc = std::make_unique<RangeDomainBisecter>();
   smap->setSlicer(DomainType::Range, std::move(slc));

   // range union
   slc = std::make_unique<RangeUnionDomainBisecter>();
   smap->setSlicer(DomainType::RangeUnion, std::move(slc));

   return smap;
}

} // namespace
