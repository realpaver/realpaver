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
 * @file   DomainSlicerMap.hpp
 * @brief  Map of domain slicers
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DOMAIN_SLICER_MAP_HPP
#define REALPAVER_DOMAIN_SLICER_MAP_HPP

#include <memory>
#include <vector>
#include "realpaver/DomainSlicer.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

/// Maps domain types to slicers
class DomainSlicerMap {
public:
   /// Constructor
   DomainSlicerMap();

   /// Destructor
   ~DomainSlicerMap();

   /// No copy
   DomainSlicerMap(const DomainSlicerMap&) = delete;

   /// No assignment
   DomainSlicerMap& operator=(const DomainSlicerMap&) = delete;

   /// Associates a slicer with a domain type
   void setSlicer(DomainType type, std::unique_ptr<DomainSlicer> slc);

   /// Gets the slicer associated with a domain type
   DomainSlicer* getSlicer(DomainType type) const;

   /// Tests if a slicer is already associated with a domain type
   bool hasSlicer(DomainType type) const;

private:
   std::vector<DomainSlicer*> slc_;
};

} // namespace

#endif
