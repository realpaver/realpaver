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
/// This registers slicers associated with variables from a given scope.
///////////////////////////////////////////////////////////////////////////////
class DomainSlicerMap {
public:
   /// Creates a map
   /// @param scop set of variables
   DomainSlicerMap(Scope scop);

   /// Destructor
   ~DomainSlicerMap();

   /// No copy
   DomainSlicerMap(const DomainSlicerMap&) = delete;

   /// No assignment
   DomainSlicerMap& operator=(const DomainSlicerMap&) = delete;

   /// Associates a slicer with a variable
   /// @param v a variable
   /// @param pslicer slicer associated with v in this
   void setSlicer(const Variable& v, std::unique_ptr<DomainSlicer> pslicer);

   /// Gets a slicer
   /// @param v a variable
   /// @return the slicer associated with v
   DomainSlicer* getSlicer(const Variable& v) const;

private:
   Scope scop_;                        // scope
   std::vector<DomainSlicer*> sli_;    // slicers
};

} // namespace

#endif
