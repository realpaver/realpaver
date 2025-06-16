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
 * @file   SelectorSLF.hpp
 * @brief  Variable selection strategy for mixed problems
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#ifndef REALPAVER_SELECTOR_SLF_HPP
#define REALPAVER_SELECTOR_SLF_HPP

#include "realpaver/Selector.hpp"

namespace realpaver {

/**
 * @brief Variable selection strategy for mixed problems.
 *
 * If there is an integer variable whose domain is splitable, then it selects
 * the integer variable having the smallest domain. Otherwise, it selects the
 * real variable having the largest domain.
 */
class SelectorSLF : public Selector {
public:
   /// Creates a selector on a scope
   SelectorSLF(Scope scop);

   /// Default destructor
   ~SelectorSLF() = default;

   /// Default copy constructor
   SelectorSLF(const SelectorSLF &) = default;

   /// No assignment
   SelectorSLF &operator=(const SelectorSLF &) = delete;

   bool apply(const DomainBox &box) override;
};

} // namespace realpaver

#endif
