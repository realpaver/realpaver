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
 * @file   CSPSpaceDMDFS.cpp
 * @brief  Distant-Most Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/CSPSpaceDMDFS.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include <algorithm>

namespace realpaver {

DistCalculator::~DistCalculator()
{
}

/*----------------------------------------------------------------------------*/

double HausdorffDistCalculator::distance(const DomainBox &db1, const DomainBox &db2)
{
   ASSERT(db1.scope() == db2.scope(), "The scopes of the two boxes must be equal");

   double d = 0.0;
   for (const auto &v : db1.scope())
   {
      Interval x = db1.get(v)->intervalHull(), y = db2.get(v)->intervalHull();
      double e = x.distance(y);

      if (e > d)
         d = e;
   }
   return d;
}

/*----------------------------------------------------------------------------*/

CSPSpaceDMDFS::CSPSpaceDMDFS()
    : vnode_()
    , vsol_()
    , dcalc_(nullptr)
{
   dcalc_ = new HausdorffDistCalculator();
}

CSPSpaceDMDFS::~CSPSpaceDMDFS()
{
   if (dcalc_ != nullptr)
      delete dcalc_;
}

void CSPSpaceDMDFS::setDistCalculator(std::unique_ptr<DistCalculator> dcalc)
{
   if (dcalc_ != nullptr)
      delete dcalc_;

   dcalc_ = dcalc.release();
}

size_t CSPSpaceDMDFS::nbSolNodes() const
{
   return vsol_.size();
}

void CSPSpaceDMDFS::pushSolNode(const SharedCSPNode &node)
{
   vsol_.push_back(node);

   // update the distances
   for (auto &elem : vnode_)
   {
      double d = dcalc_->distance(*node->box(), *elem.node->box());

      if (d < elem.mindist)
         elem.mindist = d;
   }

   // ascending ordering of the node distances
   std::sort(vnode_.begin(), vnode_.end(), comparator);
}

SharedCSPNode CSPSpaceDMDFS::popSolNode()
{
   SharedCSPNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedCSPNode CSPSpaceDMDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

   return vsol_[i];
}

bool CSPSpaceDMDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

void CSPSpaceDMDFS::makeSolClusters(double gap)
{
   // no clustering if the gap is negative
   if (gap < 0.0)
      return;

   // clustering from the super class
   CSPSpace::makeSolClusters(gap);

   // it is necessary to update the distance between each pending node and its
   // closest solution.
   for (auto &elem : vnode_)
   {
      DomainBox *reg = elem.node->box();
      elem.mindist = Double::inf();

      for (auto &sol : vsol_)
      {
         DomainBox *regsol = sol->box();
         double d = dcalc_->distance(*reg, *regsol);

         if (d < elem.mindist)
            elem.mindist = d;
      }
   }

   // ascending ordering of the node distances
   std::sort(vnode_.begin(), vnode_.end(), comparator);
}

size_t CSPSpaceDMDFS::nbPendingNodes() const
{
   return vnode_.size();
}

SharedCSPNode CSPSpaceDMDFS::nextPendingNode()
{
   SharedCSPNode node = vnode_.back().node;
   vnode_.pop_back();
   return node;
}

void CSPSpaceDMDFS::insertPendingNode(const SharedCSPNode &node)
{
   // Finds the distance to the closest solution
   double d = Double::inf();
   for (auto &sol : vsol_)
   {
      DomainBox *reg = sol->box();
      double e = dcalc_->distance(*reg, *node->box());

      if (e < d)
         d = e;
   }

   LOG_INTER("Insert node " << node->index()
                            << " / distance to the closest solution : " << d);

   Elem elem = {node, d};
   vnode_.push_back(elem);
}

SharedCSPNode CSPSpaceDMDFS::getPendingNode(size_t i) const
{
   ASSERT(i < vnode_.size(), "Bad access to a pending node in a CSP space");

   return vnode_[i].node;
}

} // namespace realpaver
