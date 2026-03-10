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
 * @file   SpaceOptimFD.cpp
 * @brief  Search space of branch-and-bound algorithm
 * @author Laurent Granvilliers
 * @date   17 Nov 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodeOptim.hpp"
#include "realpaver/SpaceOptimFD.hpp"

namespace realpaver {

SpaceOptimFD::SpaceOptimFD()
    : SpaceOptim()
    , nod_()
    , next_(nullptr)
    , last_(-1)
{
}

double SpaceOptimFD::getLowestLowerBound() const
{
   double l = Double::inf();
   auto it = nod_.begin();
   auto jt = sol_.begin();

   if (it != nod_.end())
      l = std::min(l, (*it)->getLower());

   if (next_ != nullptr)
      l = std::min(l, next_->getLower());

   if (jt != sol_.end())
      l = std::min(l, (*jt)->getLower());

   return l;
}

void SpaceOptimFD::simplifyImpl(double u)
{
   // counts the number of nodes to remove from the end (from the greatest lower bounds)
   int n = 0;
   auto it = nod_.rbegin();
   while (it != nod_.rend())
   {
      double l = (*it)->getLower();
      if (l > u)
      {
         ++n;
         it++;
      }
      else
         it = nod_.rend();
   }

   // removes the nodes
   auto jt = nod_.begin();
   std::advance(jt, nod_.size() - n);
   nod_.erase(jt, nod_.end());

   if (next_ != nullptr)
   {
      double l = next_->getLower();
      if (tol_.isTightOptimum(l, u))
      {
         next_ = nullptr;
      }
   }
}

void SpaceOptimFD::print(std::ostream &os) const
{
   os << "Optimization space (FD)" << std::endl;
   os << "Lowest LB: " << getLowestLowerBound() << std::endl;
   os << "Pending nodes (" << nbPendingNodes() << ")" << std::endl;
   for (const auto &it : nod_)
      os << "   " << (*it) << std::endl;
   if (next_ != nullptr)
      os << "Next node (DFS): " << (*next_) << std::endl;
   os << "Solution nodes (" << nbSolNodes() << ")" << std::endl;
   for (const auto &it : sol_)
      os << "   " << (*it) << std::endl;
}

size_t SpaceOptimFD::nbPendingNodes() const
{
   return (next_ == nullptr) ? nod_.size() : 1 + nod_.size();
}

SharedNodeOptim SpaceOptimFD::popPendingNode()
{
   SharedNodeOptim res;
   if (next_ != nullptr)
   {
      res = next_;
      next_ = nullptr;
   }
   else
   {
      auto it = nod_.begin();
      res = (*it);
      nod_.erase(it);
   }
   last_ = res->index();
   return res;
}

void SpaceOptimFD::pushPendingNode(const SharedNodeOptim &node)
{
   if (node->parent() != last_ || last_ == -1)
   {
      // Best-FS
      nod_.insert(node);
   }
   else
   {
      // DFS
      if (next_ == nullptr)
      {
         next_ = node;
      }
      else if (next_->getLower() > node->getLower())
      {
         nod_.insert(next_);
         next_ = node;
      }
      else
      {
         nod_.insert(node);
      }
   }
}

SharedNodeOptim SpaceOptimFD::getPendingNode(size_t i) const
{
   auto it = nod_.begin();
   std::advance(it, i);
   return *it;
}

} // namespace realpaver
