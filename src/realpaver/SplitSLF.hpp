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
 * @file   SplitSLF.hpp
 * @brief  Splitting strategy for mixed problems
 * @author Laurent Granvilliers
 * @date   10 June 2025
 */

#ifndef REALPAVER_SPLIT_SLF_HPP
#define REALPAVER_SPLIT_SLF_HPP

#include "realpaver/SelectorSLF.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

/**
 * @brief Splitting strategy for mixed problems.
 *
 * If there is an integer variable whose domain is splitable, then it selects
 * the integer variable having the smallest domain. Otherwise, it selects the
 * real variable having the largest domain.
 */
class SplitSLF : public Split {
public:
   /// Constructor
   SplitSLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~SplitSLF() = default;

   /// No copy
   SplitSLF(const SplitSLF &) = delete;

   /// No assignment
   SplitSLF &operator=(const SplitSLF &) = delete;

   void applyImpl(SharedNode &node, NodeContext &context) override;

private:
   SelectorSLF sel_;
};

} // namespace realpaver

#endif
