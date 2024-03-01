///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DOMAIN_SLICER_MAP_HPP
#define REALPAVER_DOMAIN_SLICER_MAP_HPP

#include <memory>
#include <vector>
#include "realpaver/DomainSlicer.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This registers slicers associated with domain types.
///////////////////////////////////////////////////////////////////////////////
class DomainSlicerMap {
public:
   /// Creates a map
   DomainSlicerMap();

   /// Destructor
   ~DomainSlicerMap();

   /// No copy
   DomainSlicerMap(const DomainSlicerMap&) = delete;

   /// No assignment
   DomainSlicerMap& operator=(const DomainSlicerMap&) = delete;

   /// Associates a slicer with a domain type
   /// @param type a domain type
   /// @param slc a slicer
   void setSlicer(DomainType type, std::unique_ptr<DomainSlicer> slc);

   /// Gets a slicer
   /// @param type a domain type
   /// @return the slicer associated with this type
   DomainSlicer* getSlicer(DomainType type) const;

   /// Tests if a slicer is already associated with a domain type
   /// @param type a domain type
   /// @return true if a slicer is associated with this type
   bool hasSlicer(DomainType type) const;

private:
   std::vector<DomainSlicer*> slc_;
};

} // namespace

#endif
