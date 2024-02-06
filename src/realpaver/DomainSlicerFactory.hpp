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

#include "realpaver/DomainSlicerMap.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This class implements strategies for creating domain slicer maps.
///////////////////////////////////////////////////////////////////////////////
class DomainSlicerFactory {
public:
   /// Bisection strategy
   /// @param scop set of variables
   /// @return the domain slicer map
   static std::unique_ptr<DomainSlicerMap> makeBisectionStrategy(Scope scop);
};

} // namespace

#endif
