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
 * @file   SplitLF.hpp
 * @brief  Largest-First splitting strategy
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#ifndef REALPAVER_SPLIT_LF_HPP
#define REALPAVER_SPLIT_LF_HPP

#include "realpaver/SelectorLF.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

/// Largest-First splitting strategy
class SplitLF : public Split {
public:
   /// Constructor
   SplitLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap);

   /// Default destructor
   ~SplitLF() = default;

   /// No copy
   SplitLF(const SplitLF &) = delete;

   /// No assignment
   SplitLF &operator=(const SplitLF &) = delete;

   void applyImpl(SharedNode &node, NodeContext &context) override;

private:
   SelectorLF sel_;
};

} // namespace realpaver

#endif
