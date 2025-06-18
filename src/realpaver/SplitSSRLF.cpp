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
 * @file   SplitSSRLF.cpp
 * @brief  Splitting strategy hybridizing SSR and LF
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#include "realpaver/Logger.hpp"
#include "realpaver/SplitSSRLF.hpp"

namespace realpaver {

SplitSSRLF::SplitSSRLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                       IntervalFunctionVector F)
    : Split(scop, std::move(smap))
    , sel_(scop, F)
{
}

double SplitSSRLF::getFrequency() const
{
   return sel_.getFrequency();
}

void SplitSSRLF::setFrequency(double f)
{
   sel_.setFrequency(f);
}

void SplitSSRLF::applyImpl(SharedNode &node, NodeContext &context)
{
   if (sel_.apply(*node->box()))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("HybridSSR selects " << v.getName() << " in node " << node->index());
   }
}

} // namespace realpaver
