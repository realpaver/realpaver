///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DOMAIN_SLICER_FACTORY_HPP
#define REALPAVER_DOMAIN_SLICER_FACTORY_HPP

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
   /// @param sco set of variables
   DomainSlicerMap(Scope sco);

   /// Destructor
   ~DomainSlicerMap();

   /// No copy
   DomainSlicerMap(const DomainSlicerMap&) = delete;

   /// No assignment
   DomainSlicerMap& operator=(const DomainSlicerMap&) = delete;

   /// Associates a slicer with a variable
   /// @param v a variable
   /// @param pslicer slicer associated with v in this
   void setSlicer(Variable v, std::unique_ptr<DomainSlicer> pslicer);

   /// Gets a slicer
   /// @param v a variable
   /// @return the slicer associated with v
   DomainSlicer* getSlicer(const Variable& v) const;

private:
   Scope sco_;                         // scope
   std::vector<DomainSlicer*> sli_;    // slicers
};

///////////////////////////////////////////////////////////////////////////////
/// This class implements strategies for creating domain slicer maps.
///////////////////////////////////////////////////////////////////////////////
class DomainSlicerFactory {
public:
   /// Bisection strategy
   /// @param sco set of variables
   /// @return the domain slicer map
   static std::unique_ptr<DomainSlicerMap> makeBisectionStrategy(Scope sco);
};

} // namespace

#endif
