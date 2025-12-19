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
 * @file   LinearizerAffineTaylor.cpp
 * @brief  Linearization of nonlinear problems based on affine and Taylor forms
 * @author Laurent Granvilliers
 * @date   11 Feb 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/LinearizerAffineTaylor.hpp"

namespace realpaver {

LinearizerAffineTaylor::LinearizerAffineTaylor(std::unique_ptr<LinearizerAffine> affine,
                                               std::unique_ptr<LinearizerTaylor> taylor)
    : Linearizer(affine->dag())
    , affine_(std::move(affine))
    , taylor_(std::move(taylor))
{
   ASSERT(affine_ != nullptr, "Null pointer (affine linearizer)");
   ASSERT(taylor_ != nullptr, "Null pointer (taylor linearizer)");
}

bool LinearizerAffineTaylor::makeLPImpl(LPModel &lpm, const IntervalBox &B,
                                        const Bitset &active)
{
   bool b = affine_->makeLP(lpm, B, active);
   if (!b)
      return b;

   for (const auto &v : scop_)
   {
      // copy the association variable / linear variable from the affine-based
      // linearizer in this and in the taylor-based linearizer
      setLinVarIndex(v.id(), affine_->linVarIndex(v.id()));
      taylor_->setLinVarIndex(v.id(), affine_->linVarIndex(v.id()));
   }

   return taylor_->makeCtrs(lpm, B, active);
}

} // namespace realpaver
