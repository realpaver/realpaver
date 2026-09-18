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

#ifndef REALPAVER_NODE_PROPAGATOR_LOOP_HPP
#define REALPAVER_NODE_PROPAGATOR_LOOP_HPP

#include "realpaver/NodePropagator.hpp"
#include <vector>

namespace realpaver {

/// Propagator
class NodePropagatorLoop : public NodePropagator {
public:
   /// Constructor
   NodePropagatorLoop(SharedNodePropagator op);

   /// Default copy constructor
   NodePropagatorLoop(const NodePropagatorLoop &) = default;

   /// No assignment
   NodePropagatorLoop &operator=(const NodePropagatorLoop &) = delete;

   /// Default destructor
   ~NodePropagatorLoop() = default;

   /// Returns the tolerance used as stopping criterion
   double getTol() const;

   /// Sets the tolerance used as stopping criterion
   void setTol(double tol);

   Proof contractImpl(IntervalBox &B, const Bitset &active) override;

private:
   SharedNodePropagator propagator_; // operator
   double tol_;                      // tolerance used for iteration
};

} // namespace realpaver

#endif
