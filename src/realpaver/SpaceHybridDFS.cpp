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
 * @file   SpaceHybridDFS.cpp
 * @brief  Search space for hybrid strategies
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/SpaceHybridDFS.hpp"
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

HybridNodeSet::~HybridNodeSet()
{
}

/*----------------------------------------------------------------------------*/

bool DepthNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t DepthNodeSet::size() const
{
   return set_.size();
}

void DepthNodeSet::insert(const SharedNode &node)
{
   Elem e = {node, node->depth()};
   set_.insert(e);
}

SharedNode DepthNodeSet::extract()
{
   auto it = set_.begin();
   SharedNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / depth : " << it->depth);

   set_.erase(it);
   return node;
}

SharedNode DepthNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

bool PerimeterNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t PerimeterNodeSet::size() const
{
   return set_.size();
}

void PerimeterNodeSet::insert(const SharedNode &node)
{
   double p = node->box()->perimeter();

   LOG_INTER("Insert node " << node->index() << " / perimeter : " << p);

   Elem e = {node, p};
   set_.insert(e);
}

SharedNode PerimeterNodeSet::extract()
{
   auto it = set_.begin();
   SharedNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / perimeter : " << it->peri);

   set_.erase(it);
   return node;
}

SharedNode PerimeterNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

bool GridPerimeterNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t GridPerimeterNodeSet::size() const
{
   return set_.size();
}

void GridPerimeterNodeSet::insert(const SharedNode &node)
{
   double p = node->box()->gridPerimeter();

   LOG_INTER("Insert node " << node->index() << " / grid perimeter : " << p);

   Elem e = {node, p};
   set_.insert(e);
}

SharedNode GridPerimeterNodeSet::extract()
{
   auto it = set_.begin();
   SharedNode node = it->node;

   LOG_INTER("Extract node " << node->index() << " / grid perimeter : " << it->peri);

   set_.erase(it);
   return node;
}

SharedNode GridPerimeterNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

/*----------------------------------------------------------------------------*/

SpaceHybridDFS::SpaceHybridDFS(HybridDFSStyle style)
    : Space()
    , sta_()
    , set_(nullptr)
    , vsol_()
    , leftRight_(true)
{
   if (style == HybridDFSStyle::Depth)
      set_ = new DepthNodeSet();

   else if (style == HybridDFSStyle::Perimeter)
      set_ = new PerimeterNodeSet();

   else if (style == HybridDFSStyle::GridPerimeter)
      set_ = new GridPerimeterNodeSet();

   else
      THROW("SpaceHybridDFS, style not yet supported");
}

SpaceHybridDFS::~SpaceHybridDFS()
{
   if (set_ != nullptr)
      delete set_;
}

size_t SpaceHybridDFS::nbSolNodes() const
{
   return vsol_.size();
}

void SpaceHybridDFS::pushSolNode(const SharedNode &node)
{
   vsol_.push_back(node);

   // changes the ordering for the next DFS stage
   leftRight_ = !leftRight_;

   // moves the nodes from the stack to the set
   for (auto node : sta_)
      set_->insert(node);

   sta_.clear();
}

SharedNode SpaceHybridDFS::popSolNode()
{
   SharedNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNode SpaceHybridDFS::getSolNode(size_t i) const
{
   REALPAVER_ASSERT(i < vsol_.size(), "Bad access to a solution node in a space");

   return vsol_[i];
}

bool SpaceHybridDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

size_t SpaceHybridDFS::nbPendingNodes() const
{
   return sta_.size() + set_->size();
}

SharedNode SpaceHybridDFS::popPendingNode()
{
   // gets the top of the stack if it is not empty
   if (sta_.empty())
   {
      SharedNode node = set_->extract();
      return node;
   }
   // the first element of the set otherwise
   else
   {
      SharedNode node = sta_.back();
      sta_.pop_back();
      return node;
   }
}

void SpaceHybridDFS::pushPendingNode(const SharedNode &node)
{
   // inserts a node in the stack during a DFS stage
   sta_.push_back(node);
}

SharedNode SpaceHybridDFS::getPendingNode(size_t i) const
{
   REALPAVER_ASSERT(i < nbPendingNodes(), "Bad access to a pending node in a space");

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

} // namespace realpaver
