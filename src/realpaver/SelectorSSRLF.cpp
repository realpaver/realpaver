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
 * @file   SelectorSSRLF.cpp
 * @brief  Variable selection strategy alternating SSR and LF
 * @author Laurent Granvilliers
 * @date   30 Apr 2025
 */

#include "realpaver/Param.hpp"
#include "realpaver/SelectorSSRLF.hpp"

namespace realpaver {

SelectorSSRLF::SelectorSSRLF(Scope scop, IntervalFunctionVector F)
    : Selector(scop)
    , ssr_(F)
    , lf_(scop)
    , nb_(0)
    , nbssr_(0)
    , f_(Params::GetDblParam("SPLIT_SSR_LF_FREQUENCY"))
{
}

double SelectorSSRLF::getFrequency() const
{
   return f_;
}

void SelectorSSRLF::setFrequency(double f)
{
   ASSERT(f >= 0.0 && f <= 1.0, "Bad frequency");

   f_ = f;
}

bool SelectorSSRLF::apply(const DomainBox &box)
{
   bool res = true;
   if (nbssr_ < f_ * (++nb_))
   {
      ++nbssr_;
      res = ssr_.apply(box);
      if (res)
         setSelectedVar(ssr_.getSelectedVar());
   }
   else
   {
      res = lf_.apply(box);
      if (res)
         setSelectedVar(lf_.getSelectedVar());
   }
   return res;
}

} // namespace realpaver
