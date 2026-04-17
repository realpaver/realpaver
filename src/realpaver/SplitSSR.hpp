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
 * @file   SplitSSR.hpp
 * @brief  Smear-Sum-Relative splitting strategy
 * @author Laurent Granvilliers
 * @date   10 June 2025
 */

#ifndef REALPAVER_SPLIT_SSR_HPP
#define REALPAVER_SPLIT_SSR_HPP

#include "realpaver/SelectorSSR.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

/// Splitting strategy based on Smear Sum Relative values
class SplitSSR : public Split {
public:
   /// Constructor
   SplitSSR(Scope scop, std::unique_ptr<DomainSlicerMap> smap, IntervalFunctionVector F);

   /// Default destructor
   ~SplitSSR() = default;

   /// No copy
   SplitSSR(const SplitSSR &) = delete;

   /// No assignment
   SplitSSR &operator=(const SplitSSR &) = delete;

   void applyImpl(SharedNode node, NodeContext &context) override;

private:
   SelectorSSR ssr_; // main selector based on SSR values
};

} // namespace realpaver

#endif
