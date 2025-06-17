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
 * @file   SplitSF.hpp
 * @brief  Smallest-First splitting strategy
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#ifndef REALPAVER_SPLIT_SF_HPP
#define REALPAVER_SPLIT_SF_HPP

#include "realpaver/SelectorSF.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

/// Smallest-First splitting strategy
class SplitSF : public Split {
public:
   /// Constructor
   SplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~SplitSF() = default;

   /// No copy
   SplitSF(const SplitSF &) = delete;

   /// No assignment
   SplitSF &operator=(const SplitSF &) = delete;

   void applyImpl(SharedNode &node, NodeContext &context) override;

private:
   SelectorSF sel_;
};

} // namespace realpaver

#endif
