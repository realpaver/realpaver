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
 * @file   IntervalBoxPicker.hpp
 * @brief  Base class of pickers on interval boxes
 * @author Laurent Granvilliers
 * @date   13 May 2026
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalBoxPicker.hpp"

namespace realpaver {

void IntervalBoxPicker::apply(const IntervalBox &B, RealPoint &pt)
{
   applyOnScope(B, pt, pt.scope());
}

void IntervalBoxPicker::applyOnScope(const IntervalBox &B, RealPoint &pt,
                                     const Scope &scop)
{
   REALPAVER_ASSERT(pt.scope().contains(scop), "Bad scopes");
   REALPAVER_ASSERT(B.scope().contains(scop), "Bad scopes");
   REALPAVER_ASSERT(!B.isEmpty(), "Empty box");

   for (const auto &v : scop)
      pt.set(v, B.get(v).midpoint());
}

} // namespace realpaver
