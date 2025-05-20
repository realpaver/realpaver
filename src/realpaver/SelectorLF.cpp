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
 * @file   SelectorLF.cpp
 * @brief  Variable selection strategy Largest-First
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#include "realpaver/SelectorLF.hpp"

namespace realpaver {

SelectorLF::SelectorLF(Scope scop)
    : Selector(scop)
{
}

bool SelectorLF::apply(const DomainBox &box)
{
   double dmax;
   bool found = false;

   for (const auto &v : scop_)
   {
      if (box.isSplitable(v))
      {
         double d = box.get(v)->discreteSize(v.getTolerance().getAbsTol());
         if ((!found) || (d > dmax))
         {
            setSelectedVar(v);
            dmax = d;
            found = true;
         }
      }
   }

   return found;
}

} // namespace realpaver
