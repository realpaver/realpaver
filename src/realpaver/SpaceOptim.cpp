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
 * @file   SpaceOptim.cpp
 * @brief  Base class of search spaces of branch-and-bound algorithms
 * @author Laurent Granvilliers
 * @date   1 July 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NodeOptim.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/SpaceOptim.hpp"
#include <iterator>

namespace realpaver {

SpaceOptim::SpaceOptim()
    : Space()
    , sol_()
    , tol_(Tolerance(Params::GetDblParam("OBJ_REL_TOL"),
                     Params::GetDblParam("OBJ_ABS_TOL")))
{
}

SpaceOptim::~SpaceOptim()
{
}

Tolerance SpaceOptim::getTol() const
{
   return tol_;
}

void SpaceOptim::setTol(const Tolerance &tol)
{
   tol_ = tol;
}

void SpaceOptim::simplify(double u)
{
   // counts the number of nodes to remove from the end (from the greatest lower bounds)
   int n = 0;
   auto it = sol_.rbegin();
   while (it != sol_.rend())
   {
      double l = (*it)->getLower();
      if (l > u)
      {
         ++n;
         it++;
      }
      else
         it = sol_.rend();
   }

   // removes the nodes
   auto jt = sol_.begin();
   std::advance(jt, sol_.size() - n);
   sol_.erase(jt, sol_.end());

   // other nodes
   simplifyImpl(u);
}

std::ostream &operator<<(std::ostream &os, const SpaceOptim &space)
{
   space.print(os);
   return os;
}

size_t SpaceOptim::nbSolNodes() const
{
   return sol_.size();
}

void SpaceOptim::pushSolNode(const SharedNodeOptim &node)
{
   sol_.insert(node);
}

SharedNodeOptim SpaceOptim::popSolNode()
{
   auto it = sol_.begin();
   SharedNodeOptim node = *it;
   sol_.erase(it);
   return node;
}

SharedNodeOptim SpaceOptim::getSolNode(size_t i) const
{
   REALPAVER_ASSERT(i < sol_.size(), "Bad access to a solution node in a space @ " << i);
   auto it = sol_.begin();
   std::advance(it, i);
   return *it;
}

bool SpaceOptim::hasFeasibleSolNode() const
{
   for (auto node : sol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

} // namespace realpaver
