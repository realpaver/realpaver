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

std::unique_ptr<DomainSlicerMap>
   DomainSlicerFactory::makeBisectionStrategy(Scope scop)
{
   std::unique_ptr<DomainSlicerMap>
      smap = std::make_unique<DomainSlicerMap>(scop);

   for (const auto& v : scop)
   {
      Domain* dom = v.getDomain();
      std::unique_ptr<DomainSlicer> pslicer;

      switch(dom->type())
      {
         case DomainType::Binary:
            pslicer = std::make_unique<BinaryDomainSlicer>();
            break;
         
         case DomainType::Interval:
            pslicer = std::make_unique<IntervalDomainBisecter>();
            break;

         case DomainType::IntervalUnion:
            pslicer = std::make_unique<IntervalUnionDomainBisecter>();
            break;

         case DomainType::Range:
            pslicer = std::make_unique<RangeDomainBisecter>();
            break;

         case DomainType::RangeUnion:
            pslicer = std::make_unique<RangeUnionDomainBisecter>();
            break;
      }
      
      smap->setSlicer(v, std::move(pslicer));
   }

   return smap;
}

} // namespace
