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
   /**
    * @brief Bisection strategy.
    *
    * Returns a domain slicer map such that every doman is split in two parts.
    * - range: bisection
    * - interval: [a, b] -> a + sip*(b-a)
    * - interval union: split along with a hole if any, otherwise same as
    *   interval
    * - range union split along with a hole if any, otherwise same as
    *   range
    * - binary: trivial
    */
   static std::unique_ptr<DomainSlicerMap> makeBiStrategy(double sip);
};

} // namespace realpaver

#endif
