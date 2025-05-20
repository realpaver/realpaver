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
 * @file   SelectorSF.cpp
 * @brief  Selector of variables using the Smallest-First strategy
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#include "realpaver/SelectorSF.hpp"

namespace realpaver {

SelectorSF::SelectorSF(Scope scop)
    : Selector(scop)
{
}

bool SelectorSF::apply(const DomainBox &box)
{
   double dmin;
   bool found = false;

   for (const auto &v : scop_)
   {
      if (box.isSplitable(v))
      {
         double d = box.get(v)->discreteSize(v.getTolerance().getAbsTol());
         if ((!found) || (d < dmin))
         {
            setSelectedVar(v);
            dmin = d;
            found = true;
         }
      }
   }

   return found;
}

} // namespace realpaver
