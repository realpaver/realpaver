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
 * @file   BoxPicker.cpp
 * @brief  Base class of pickers
 * @author Laurent Granvilliers
 * @date   14 Oct 2025
 */

#include "realpaver/BoxPicker.hpp"
#include "realpaver/DomainPicker.hpp"

namespace realpaver {

BoxPicker::~BoxPicker()
{
}

void BoxPicker::apply(const DomainBox &box, RealPoint &pt)
{
   REALPAVER_ASSERT(box.scope() == pt.scope(), "The scopes are different");
   REALPAVER_ASSERT(!box.isEmpty(), "Empty box");

   for (const auto &v : box.scope())
   {
      Domain *dom = box.get(v);
      DomainPicker vis;
      dom->acceptVisitor(vis);
      pt.set(v, vis.getValue());
   }
}

} // namespace realpaver
