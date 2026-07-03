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

#include "AssertDebug.hpp"
#include "Common.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Prospector.hpp"

namespace realpaver {

Prospector::Prospector(std::unique_ptr<Prover> prover)
    : prover_(std::move(prover))
    , feasible_(nullptr)
    , use_prover_(true)
{
   REALPAVER_ASSERT(prover_ != nullptr, "No prover is given as input");
}

Prospector::~Prospector()
{
   if (feasible_ != nullptr)
      delete feasible_;
}

bool Prospector::explore(Node &node, NodeContext &ctx)
{
   RealPoint pt(node.scope());
   bool res = exploreImpl(node, pt);
   if (res)
   {
      if (feasible_ != nullptr)
         delete feasible_;
      feasible_ = new IntervalBox(pt);

      if (use_prover_)
      {
         Proof proof = prover_->certify(*feasible_, node.active());
         res = (proof == Proof::Feasible || proof == Proof::Inner);
      }
      else
      {
         LOG_INTER("PROVER NOT USED")
         res = true;
      }
   }
   LOG_INTER("PROSPECTOR END")
   return res;
}

const IntervalBox &Prospector::feasibleBox() const
{
   REALPAVER_ASSERT(feasible_ != nullptr, "No feasible point found");
   return *feasible_;
}

} // namespace realpaver
