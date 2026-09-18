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
 * @file   NodePropagatorList.cpp
 * @brief  Node propagator applying a sequence of propagators
 * @author Laurent Granvilliers
 * @date   13 Oct 2025
 */

#include "realpaver/NodePropagatorACID.hpp"
#include "realpaver/NodePropagatorAffine.hpp"
#include "realpaver/NodePropagatorHC4.hpp"
#include "realpaver/NodePropagatorList.hpp"
#include "realpaver/NodePropagatorNewton.hpp"
#include "realpaver/NodePropagatorPolytope.hpp"

namespace realpaver {

NodePropagatorList::NodePropagatorList()
    : NodePropagator()
    , v_()
{
}

void NodePropagatorList::pushBack(SharedNodePropagator op)
{
   v_.push_back(op);
}

void NodePropagatorList::pushBack(NodePropagAlgo alg, ContractorFactory &facto)
{
   SharedNodePropagator op;
   switch (alg)
   {
   case NodePropagAlgo::HC4:
      op = std::make_shared<NodePropagatorHC4>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::AFFINE:
      op = std::make_shared<NodePropagatorAffine>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::ACID:
      op = std::make_shared<NodePropagatorACID>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::Polytope:
      op = std::make_shared<NodePropagatorPolytope>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;

   case NodePropagAlgo::Newton:
      op = std::make_shared<NodePropagatorNewton>(facto);
      if (op != nullptr)
         v_.push_back(op);
      break;
   }
}

size_t NodePropagatorList::size() const
{
   return v_.size();
}

Proof NodePropagatorList::contractImpl(IntervalBox &B, const Bitset &active)
{
   Proof proof = Proof::Maybe;
   for (size_t i = 0; i < v_.size(); ++i)
   {
      proof = v_[i]->contractImpl(B, active);
      if (proof == Proof::Empty)
         return proof;
   }
   return proof;
}

} // namespace realpaver
