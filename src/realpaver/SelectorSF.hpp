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
 * @file   SelectorSF.hpp
 * @brief  Variable selection strategy Smallest-First
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_SF_HPP
#define REALPAVER_SELECTOR_SF_HPP

#include "realpaver/Selector.hpp"

namespace realpaver {

/**
 * @brief Variable selection strategy Smallest-First.
 *
 * It selects the variable having the smallest domain. Domains are compared as follows.
 * The size of a discrete domain is its number of values. The size of a real domain
 * is the width of its interval hull divided by the variable tolerance, i.e. the domain
 * is discretized to measure its size.
 */
class SelectorSF : public Selector {
public:
   /// Creates a selector on a scope
   SelectorSF(Scope scop);

   /// Default destructor
   ~SelectorSF() = default;

   /// Default copy constructor
   SelectorSF(const SelectorSF &) = default;

   /// No assignment
   SelectorSF &operator=(const SelectorSF &) = delete;

   bool apply(const DomainBox &box) override;
};

} // namespace realpaver

#endif
