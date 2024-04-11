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
 * @file   DomainSlicerFactory.hpp
 * @brief  Factory of domain slicer maps
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DOMAIN_SLICER_FACTORY_HPP
#define REALPAVER_DOMAIN_SLICER_FACTORY_HPP

#include "realpaver/DomainSlicerMap.hpp"

namespace realpaver {

/**
 * @brief Factory of domain slicer maps.
 * 
 * A factory implements various strategies for creating domain slicer maps.
 */
class DomainSlicerFactory {
public:
   /// Returns the domain slicer map implementing bisection
   static std::unique_ptr<DomainSlicerMap> makeBisectionStrategy();
};

} // namespace

#endif
