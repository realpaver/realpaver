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
 * @file   LinearizerRLT.cpp
 * @brief  Linearization of nonlinear problems based on RLT
 * @author Laurent Granvilliers
 * @date   5 May 2025
 */

#include "realpaver/LinearizerRLT.hpp"
#include "realpaver/Logger.hpp"
#include <cstddef>

namespace realpaver {

LinearizerRLT::LinearizerRLT(SharedDag dag)
    : Linearizer(dag)
    , mnv_(nullptr)
{
   mnv_ = std::make_shared<MapType>();
}

bool LinearizerRLT::makeLPImpl(LPModel &lpm, const IntervalBox &B, const Bitset &active)
{
   if (!dag_->iEvalNodes(B))
      return false;

   mnv_->clear();

   if (active.areAllOnes())
   {
      for (size_t i = 0; i < dag_->nbNodes(); ++i)
      {
         DagNode *node = dag_->node(i);

         // creates the linear variable
         createLinVar(lpm, node);

         // constrains this variable
         RLTCreator vis(&lpm, mnv_);
         node->acceptVisitor(vis);
         if (vis.fails())
            return false;
      }
      LOG_LOW("RLT relaxation for the DAG");
   }
   else
   {
      for (size_t i = 0; i < dag_->nbFuns(); ++i)
      {
         DagFun *f = dag_->fun(i);
         if (f->isActive(active))
         {
            for (size_t j = 0; j < f->nbNodes(); ++j)
            {
               DagNode *node = f->node(j);
               auto it = mnv_->find(node->index());

               if (it == mnv_->end())
               {
                  // creates the linear variable
                  createLinVar(lpm, node);

                  // constrains this variable
                  RLTCreator vis(&lpm, mnv_);
                  node->acceptVisitor(vis);
                  if (vis.fails())
                     return false;
               }
            }
            LOG_LOW("RLT relaxation for the constraint @ " << f->ctrId());
         }
      }

      // manages the variables that are not involved in active constraints
      // simply creates a linear variable for each of these variables
      for (size_t i = 0; i < dag_->nbVars(); ++i)
      {
         DagVar *node = dag_->varNode(i);
         auto it = mnv_->find(node->index());
         if (it == mnv_->end())
            createLinVar(lpm, node);
      }
   }

   // takes into account the root nodes
   for (size_t i = 0; i < dag_->nbFuns(); ++i)
   {
      DagFun *f = dag_->fun(i);

      if (!f->isActive(active))
         continue;

      DagNode *node = f->rootNode();
      auto it = mnv_->find(node->index());
      LinVar v = lpm.getLinVar(it->second);

      Interval img = f->getImage();
      if (img.isSingleton())
         img += Interval(-tol_, tol_);

      Interval x = v.getDomain() & img;
      if (x.isEmpty())
         return false;
      v.setDomain(x);
   }

   return true;
}

size_t LinearizerRLT::nodeToLinVar(DagNode *node) const
{
   auto it = mnv_->find(node->index());
   return it->second;
}

void LinearizerRLT::createLinVar(LPModel &lpm, DagNode *node)
{
   Interval val = node->ival();

   // creates a linear variable for this node
   LinVar v = lpm.makeVar(val.left(), val.right());

   // inserts the couple (node index, lin var index)
   mnv_->insert(std::make_pair(node->index(), v.getIndex()));

   // for a variable node, associates (var index, lin var index)
   DagVar *vnode = dynamic_cast<DagVar *>(node);
   if (vnode != nullptr)
      setLinVarIndex(vnode->getVar().id(), v.getIndex());
}

} // namespace realpaver
