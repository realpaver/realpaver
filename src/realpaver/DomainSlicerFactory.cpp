///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
