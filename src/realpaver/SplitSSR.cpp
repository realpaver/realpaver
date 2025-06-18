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
 * @file   SplitSSR.cpp
 * @brief  Smear-Sum-Relative splitting strategy
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#include "realpaver/Logger.hpp"
#include "realpaver/SelectorLF.hpp"
#include "realpaver/SplitSSR.hpp"

namespace realpaver {

SplitSSR::SplitSSR(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                   IntervalFunctionVector F)
    : Split(scop, std::move(smap))
    , ssr_(F)
    , sbis_(scop.setminus(F.scope()))
{
   ASSERT(scop.contains(F.scope()), "Bad scopes");
}

void SplitSSR::applyImpl(SharedNode &node, NodeContext &context)
{
   const DomainBox &box = *node->box();

   if (ssr_.apply(box))
   {
      Variable v = ssr_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("SSR selects " << v.getName() << " in node " << node->index());
      return;
   }

   if (!sbis_.isEmpty())
   {
      SelectorLF sel(sbis_);
      if (sel.apply(box))
      {
         Variable v = sel.getSelectedVar();
         splitOne(node, v);
         LOG_INTER("LF selects " << v.getName() << " in node " << node->index());
      }
   }
}

} // namespace realpaver
