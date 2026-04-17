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
 * @file   SplitSF.cpp
 * @brief  Smallest-First splitting strategy
 * @author Laurent Granvilliers
 * @date   10 June 2025
 */

#include "realpaver/Logger.hpp"
#include "realpaver/SplitSF.hpp"

namespace realpaver {

SplitSF::SplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : Split(scop, std::move(smap))
    , sel_()
{
}

void SplitSF::applyImpl(SharedNode node, NodeContext &context)
{
   if (sel_.apply(node, context))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("SF selects " << v.getName() << " in node " << node->index());
   }
}

} // namespace realpaver
