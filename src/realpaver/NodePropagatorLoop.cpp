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
 * @file   NodePropagatorLoop.hpp
 * @brief  Fixed-point node propagator
 * @author Laurent Granvilliers
 * @date   11 June Oct 2026
 */

#include "AssertDebug.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodePropagatorLoop.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

NodePropagatorLoop::NodePropagatorLoop(SharedNodePropagator op)
    : NodePropagator()
    , propagator_(op)
    , tol_(Params::GetDblParam("PROPAGATION_TOL"))

{
   REALPAVER_ASSERT(op != nullptr, "Null pointer");
}

double NodePropagatorLoop::getTol() const
{
   return tol_;
}

void NodePropagatorLoop::setTol(double tol)
{
   REALPAVER_ASSERT(tol >= 0.0 && tol <= 1.0,
                    "A relative tolerance must belong to [0, 1]");
   tol_ = tol;
}

Proof NodePropagatorLoop::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   bool iter = true;
   do
   {
      IntervalBox save(B);
      proof = propagator_->contractImpl(B, active);
      iter = (proof != Proof::Empty) && B.improves(save, tol_);
   } while (iter);

   return proof;
}

} // namespace realpaver
