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
 * @file   CSPSpaceHybridDFS.cpp
 * @brief  Hybrid strategies
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/CSPSpaceHybridDFS.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include <algorithm>
#include <list>

namespace realpaver {

std::ostream &operator<<(std::ostream &os, const HybridDFSStyle &style)
{
   switch (style)
   {
   case HybridDFSStyle::Depth:
      return os << "depth style";
   case HybridDFSStyle::Perimeter:
      return os << "perimeter style";
   case HybridDFSStyle::GridPerimeter:
      return os << "grid perimeter style";
   default:
      os.setstate(std::ios::failbit);
   }
   return os;
}

/*----------------------------------------------------------------------------*/

HybridCSPNodeSet::~HybridCSPNodeSet()
{
}

/*----------------------------------------------------------------------------*/

bool DepthCSPNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t DepthCSPNodeSet::size() const
{
   return set_.size();
}

void DepthCSPNodeSet::insert(const SharedCSPNode &node)
{
   Elem e = {node, node->depth()};
   set_.insert(e);
}

SharedCSPNode DepthCSPNodeSet::extract()
{
   auto it = set_.begin();
   SharedCSPNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / depth : " << it->depth);

   set_.erase(it);
   return node;
}

SharedCSPNode DepthCSPNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

bool PerimeterCSPNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t PerimeterCSPNodeSet::size() const
{
   return set_.size();
}

void PerimeterCSPNodeSet::insert(const SharedCSPNode &node)
{
   double p = node->box()->perimeter();

   LOG_INTER("Insert node " << node->index() << " / perimeter : " << p);

   Elem e = {node, p};
   set_.insert(e);
}

SharedCSPNode PerimeterCSPNodeSet::extract()
{
   auto it = set_.begin();
   SharedCSPNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / perimeter : " << it->peri);

   set_.erase(it);
   return node;
}

SharedCSPNode PerimeterCSPNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

bool GridPerimeterCSPNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t GridPerimeterCSPNodeSet::size() const
{
   return set_.size();
}

void GridPerimeterCSPNodeSet::insert(const SharedCSPNode &node)
{
   double p = node->box()->gridPerimeter();

   LOG_INTER("Insert node " << node->index() << " / grid perimeter : " << p);

   Elem e = {node, p};
   set_.insert(e);
}

SharedCSPNode GridPerimeterCSPNodeSet::extract()
{
   auto it = set_.begin();
   SharedCSPNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / grid perimeter : " << it->peri);

   set_.erase(it);
   return node;
}

SharedCSPNode GridPerimeterCSPNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

CSPSpaceHybridDFS::CSPSpaceHybridDFS(HybridDFSStyle style)
    : sta_()
    , set_(nullptr)
    , vsol_()
    , leftRight_(true)
{
   if (style == HybridDFSStyle::Depth)
      set_ = new DepthCSPNodeSet();

   else if (style == HybridDFSStyle::Perimeter)
      set_ = new PerimeterCSPNodeSet();

   else if (style == HybridDFSStyle::GridPerimeter)
      set_ = new GridPerimeterCSPNodeSet();

   else
      THROW("CSPSpaceHybridDFS, style not yet supported");
}

CSPSpaceHybridDFS::~CSPSpaceHybridDFS()
{
   if (set_ != nullptr)
      delete set_;
}

size_t CSPSpaceHybridDFS::nbSolNodes() const
{
   return vsol_.size();
}

void CSPSpaceHybridDFS::pushSolNode(const SharedCSPNode &node)
{
   vsol_.push_back(node);

   // changes the ordering for the next DFS stage
   leftRight_ = !leftRight_;

   // moves the nodes from the stack to the set
   for (auto node : sta_)
      set_->insert(node);

   sta_.clear();
}

SharedCSPNode CSPSpaceHybridDFS::popSolNode()
{
   SharedCSPNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedCSPNode CSPSpaceHybridDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

   return vsol_[i];
}

bool CSPSpaceHybridDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t CSPSpaceHybridDFS::nbPendingNodes() const
{
   return sta_.size() + set_->size();
}

SharedCSPNode CSPSpaceHybridDFS::nextPendingNode()
{
   // gets the top of the stack if it is not empty
   if (sta_.empty())
   {
      SharedCSPNode node = set_->extract();
      return node;
   }
   // the first element of the set otherwise
   else
   {
      SharedCSPNode node = sta_.back();
      sta_.pop_back();
      return node;
   }
}

void CSPSpaceHybridDFS::insertPendingNode(const SharedCSPNode &node)
{
   // inserts a node in the stack during a DFS stage
   sta_.push_back(node);
}

SharedCSPNode CSPSpaceHybridDFS::getPendingNode(size_t i) const
{
   ASSERT(i < nbPendingNodes(), "Bad access to a pending node in a CSP space");

   if (i < sta_.size())
   {
      // gets the i-th node from the stack
      return sta_[i];
   }
   else
   {
      // gets the j-th node from the set
      size_t j = i - sta_.size();
      return set_->getNode(j);
   }
}

void CSPSpaceHybridDFS::insertPendingNodes(CSPSplit::iterator first,
                                           CSPSplit::iterator last)
{
   // the nodes between first and last are ordered from left to right
   // if the DFS ordering is left to right then it is necessary to inverse
   // the range of iterators

   if (leftRight_)
   {
      std::list<SharedCSPNode> aux;
      for (auto it = first; it != last; ++it)
         aux.push_front(*it);

      for (auto it = aux.begin(); it != aux.end(); ++it)
         insertPendingNode(*it);
   }
   else
   {
      for (auto it = first; it != last; ++it)
         insertPendingNode(*it);
   }
}

} // namespace realpaver
