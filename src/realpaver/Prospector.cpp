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
 * @file   Prospector.cpp
 * @brief  Base class of prospectors
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Prospector.hpp"

namespace realpaver {

Prospector::Prospector(std::unique_ptr<ProverOptim> prover)
    : prover_(std::move(prover))
    , pt_(nullptr)
{
   REALPAVER_ASSERT(prover_ != nullptr, "No prover is given as input");
}

Prospector::~Prospector()
{
   if (pt_ != nullptr)
      delete pt_;
}

bool Prospector::explore(Node &node, NodeContext &ctx)
{
   if (pt_ != nullptr)
      delete pt_;
   pt_ = new RealPoint(node.scope());

   bool res = exploreImpl(node, *pt_);
   if (res)
   {
      IntervalBox P(*pt_);
      res = prover_->certify(P, node.active());
   }
   return res;
}

const RealPoint &Prospector::feasiblePoint() const
{
   return *pt_;
}

} // namespace realpaver
