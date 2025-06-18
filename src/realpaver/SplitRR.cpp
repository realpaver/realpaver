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
 * @file   SplitRR.cpp
 * @brief  Round-Robin splitting strategy
 * @author Laurent Granvilliers
 * @date   10 june 2025
 */

#include "realpaver/Logger.hpp"
#include "realpaver/SplitRR.hpp"

namespace realpaver {

SplitRR::SplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : Split(scop, std::move(smap))
{
}

void SplitRR::applyImpl(SharedNode &node, NodeContext &context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node, context);
   if (!res.first)
      return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Round-Robin selects " << v.getName() << " in node " << node->index());

   if (getNbNodes() < 2)
      return;

   // assigns the split variable in the sub-nodes
   std::shared_ptr<NodeInfoVar> info = std::make_shared<NodeInfoVar>(v);

   for (SharedNode &aux : cont_)
      context.insert(aux->index(), info);
}

std::pair<bool, Variable> SplitRR::selectVar(SharedNode &node, NodeContext &context)
{
   DomainBox *box = node->box();

   std::shared_ptr<NodeInfo> info =
       context.getInfo(node->index(), NodeInfoType::SplitVar);

   // assigns an iterator pointing to the next variable
   Scope::const_iterator it;
   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      NodeInfoVar *infovar = static_cast<NodeInfoVar *>(info.get());
      it = scop_.find(infovar->getVar());
      ++it;
      if (it == scop_.end())
         it = scop_.begin();
   }

   bool found = false;
   size_t nb = 0;

   while (!found && nb < scop_.size())
   {
      if (box->isSplitable(*it))
      {
         found = true;
      }
      else
      {
         ++nb;
         ++it;
         if (it == scop_.end())
            it = scop_.begin();
      }
   }

   return std::make_pair(found, *it);
}

} // namespace realpaver
