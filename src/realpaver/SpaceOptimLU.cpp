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
 * @file   SpaceOptimLU.cpp
 * @brief  Search space of branch-and-bound algorithm
 * @author Laurent Granvilliers
 * @date   5 Nov 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodeOptim.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/SpaceOptimLU.hpp"
#include <iterator>

namespace realpaver {

SpaceOptimLU::SpaceOptimLU()
    : SpaceOptim()
    , lo_()
    , up_()
    , ma_()
    , r_(Params::GetDblParam("BB_LU_RATIO"))
    , rand_(0.0, 1.0)
{
}

void SpaceOptimLU::setRatio(double ratio)
{
   REALPAVER_ASSERT(ratio >= 0.0 && ratio <= 1.0, "Bad ratio");
   r_ = ratio;
}

double SpaceOptimLU::getRatio() const
{
   return r_;
}

double SpaceOptimLU::getLowestLowerBound() const
{
   auto it = lo_.begin();
   auto jt = sol_.begin();

   if (it == lo_.end() && jt == sol_.end())
      return Double::inf();

   if (jt == sol_.end())
      return it->node->getLower();

   if (it == lo_.end())
      return (*jt)->getLower();

   return std::min(it->node->getLower(), (*jt)->getLower());
}

void SpaceOptimLU::insertNode(SharedNodeOptim node)
{
   // new item
   Item item = {node, node->index()};

   // inserts the node in the sets
   auto itlo = lo_.insert(item);
   auto itup = up_.insert(item);

   // registers the couple of iterators in the map
   SpaceIter ita = {itlo, itup};
   ma_.insert(std::make_pair(node->index(), ita));
}

SharedNodeOptim SpaceOptimLU::extractNode()
{
   double p = rand_.next();
   int i = (r_ < p) ? lo_.begin()->id : up_.begin()->id;
   return extractNode(i);
}

SharedNodeOptim SpaceOptimLU::extractNode(int i)
{
   // finds the i-th node in the map
   auto itm = ma_.find(i);
   SharedNodeOptim node = itm->second.itlo->node;

   // removes it
   lo_.erase(itm->second.itlo);
   up_.erase(itm->second.itup);
   ma_.erase(itm);

   return node;
}

void SpaceOptimLU::simplifyImpl(double u)
{
   // pending nodes
   SpaceMap::iterator itm = ma_.begin();
   while (itm != ma_.end())
   {
      double l = itm->second.itlo->node->getLower();
      if (tol_.isTightOptimum(l, u))
      {
         lo_.erase(itm->second.itlo);
         up_.erase(itm->second.itup);
         itm = ma_.erase(itm);
      }
      else
      {
         ++itm;
      }
   }
}

void SpaceOptimLU::print(std::ostream &os) const
{
   os << "Optimization space" << std::endl;
   os << "Lowest LB: " << getLowestLowerBound() << std::endl;
   os << "Pending nodes (" << nbPendingNodes() << ")" << std::endl;
   for (const auto &it : lo_)
      os << "   " << (*it.node) << std::endl;
   os << "Solution nodes (" << nbSolNodes() << ")" << std::endl;
   for (const auto &it : sol_)
      os << "   " << (*it) << std::endl;
}

size_t SpaceOptimLU::nbPendingNodes() const
{
   return lo_.size();
}

SharedNodeOptim SpaceOptimLU::popPendingNode()
{
   return extractNode();
}

void SpaceOptimLU::pushPendingNode(const SharedNodeOptim &node)
{
   insertNode(node);
}

SharedNodeOptim SpaceOptimLU::getPendingNode(size_t i) const
{
   auto it = lo_.begin();
   std::advance(it, i);
   return it->node;
}

} // namespace realpaver
