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
 * @file   SelectorSLF.cpp
 * @brief  Variable selection strategy for mixed problems
 * @author Laurent Granvilliers
 * @date   25 Apr 2025
 */

#include "realpaver/SelectorSLF.hpp"

namespace realpaver {

SelectorSLF::SelectorSLF(Scope scop)
    : Selector(scop)
{
}

bool SelectorSLF::apply(const DomainBox &box)
{
   Variable ivmin, rvmax;
   double idmin, rdmax;
   bool ifound, rfound;

   ifound = rfound = false;

   for (const auto &v : scop_)
   {
      if (box.isSplitable(v))
      {
         double d = box.get(v)->size();

         if (v.isReal())
         {
            if ((!rfound) || (d > rdmax))
            {
               rvmax = v;
               rdmax = d;
               rfound = true;
            }
         }
         else
         {
            if ((!ifound) || (d < idmin))
            {
               ivmin = v;
               idmin = d;
               ifound = true;
            }
         }
      }
   }

   if (ifound)
      setSelectedVar(ivmin);
   else if (rfound)
      setSelectedVar(rvmax);

   return ifound || rfound;
}

} // namespace realpaver
