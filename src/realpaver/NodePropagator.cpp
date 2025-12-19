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
 * @file   NodePropagator.cpp
 * @brief  Base class of node propagators
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/NodePropagator.hpp"

namespace realpaver {

NodePropagator::NodePropagator()
{
}

NodePropagator::~NodePropagator()
{
}

Proof NodePropagator::contractBox(const IntervalBox &B, DomainBox &box)
{
   for (const auto &v : box.scope())
   {
      Domain *dom = box.get(v);
      dom->contract(B.get(v));

      if (dom->isEmpty())
         return Proof::Empty;
   }
   return Proof::Maybe;
}

Proof NodePropagator::contract(Node &node, NodeContext &ctx)
{
   // creates an interval box from the domain box in the given node
   IntervalBox B(*node.box());

   // contractor of sub-class
   Proof proof = contractImpl(B, node.active());

   // contracts the domains using a reduced interval box from contractImpl
   // if any
   if (proof != Proof::Empty)
   {
      DomainBox *box = node.box();
      auto it = box->scope().begin();

      while ((proof != Proof::Empty) && (it != box->scope().end()))
      {
         Variable v = *it;
         Domain *dom = box->get(v);
         dom->contract(B.get(v));

         if (dom->isEmpty())
            proof = Proof::Empty;

         else
            ++it;
      }
   }

   return proof;
}

} // namespace realpaver
