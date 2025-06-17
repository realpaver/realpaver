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
 * @file   SplitRR.hpp
 * @brief  Round-Robin splitting strategy
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#ifndef REALPAVER_SPLIT_RR_HPP
#define REALPAVER_SPLIT_RR_HPP

#include "realpaver/Split.hpp"

namespace realpaver {

/// Round-Robin splitting strategy
class SplitRR : public Split {
public:
   /// Constructor
   SplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~SplitRR() = default;

   /// No copy
   SplitRR(const SplitRR &) = delete;

   /// No assignment
   SplitRR &operator=(const SplitRR &) = delete;

   void applyImpl(SharedNode &node, NodeContext &context) override;

private:
   // variable selection method
   std::pair<bool, Variable> selectVar(SharedNode &node, NodeContext &context);
};

} // namespace realpaver

#endif
