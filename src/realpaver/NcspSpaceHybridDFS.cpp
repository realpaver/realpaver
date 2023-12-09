///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSpaceHybridDFS.hpp"

namespace realpaver {

std::ostream& operator<<(std::ostream& os, const HybridDFSStyle& style)
{
   switch(style)
   {
      case HybridDFSStyle::Depth:         return os << "depth style";
      case HybridDFSStyle::Perimeter:     return os << "perimeter style";
      case HybridDFSStyle::GridPerimeter: return os << "grid perimeter style";
      default:                            os.setstate(std::ios::failbit);
   }
   return os;
}

///////////////////////////////////////////////////////////////////////////////

HybridNcspNodeSet::~HybridNcspNodeSet()
{}

///////////////////////////////////////////////////////////////////////////////

bool DepthNcspNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t DepthNcspNodeSet::size() const
{
   return set_.size();
}

void DepthNcspNodeSet::insert(const SharedNcspNode& node)
{
   Elem e = { node, node->depth() };
   set_.insert(e);
}

SharedNcspNode DepthNcspNodeSet::extract()
{
   auto it = set_.begin();
   SharedNcspNode node = it->node;
   set_.erase(it);
   return node;
}

SharedNcspNode DepthNcspNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

///////////////////////////////////////////////////////////////////////////////

bool PerimeterNcspNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t PerimeterNcspNodeSet::size() const
{
   return set_.size();
}

void PerimeterNcspNodeSet::insert(const SharedNcspNode& node)
{
   Elem e = { node, node->region()->perimeter() };
   set_.insert(e);
}

SharedNcspNode PerimeterNcspNodeSet::extract()
{
   auto it = set_.begin();
   SharedNcspNode node = it->node;
   set_.erase(it);
   return node;
}

SharedNcspNode PerimeterNcspNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

///////////////////////////////////////////////////////////////////////////////

bool GridPerimeterNcspNodeSet::isEmpty() const
{
   return set_.empty();
}

size_t GridPerimeterNcspNodeSet::size() const
{
   return set_.size();
}

void GridPerimeterNcspNodeSet::insert(const SharedNcspNode& node)
{
   Elem e = { node, node->region()->gridPerimeter() };
   set_.insert(e);
}

SharedNcspNode GridPerimeterNcspNodeSet::extract()
{
   auto it = set_.begin();
   SharedNcspNode node = it->node;
   set_.erase(it);
   return node;
}

SharedNcspNode GridPerimeterNcspNodeSet::getNode(size_t i) const
{
   auto it = set_.begin();
   std::advance(it, i);
   return it->node;
}

///////////////////////////////////////////////////////////////////////////////

NcspSpaceHybridDFS::NcspSpaceHybridDFS(HybridDFSStyle style)
   : sta_(),
     set_(nullptr),
     vsol_(),
     stotal_(0),
     leftRight_(true)
{
   if (style == HybridDFSStyle::Depth)
      set_ = new DepthNcspNodeSet();

   else if (style == HybridDFSStyle::Perimeter)
      set_ = new PerimeterNcspNodeSet();

   else if (style == HybridDFSStyle::GridPerimeter)
      set_ = new GridPerimeterNcspNodeSet();

   else
      THROW("NcspSpaceHybridDFS, style not yet supported");
}

NcspSpaceHybridDFS::~NcspSpaceHybridDFS()
{
   if (set_ != nullptr) delete set_;
}

size_t NcspSpaceHybridDFS::nbSolNodes() const
{
   return vsol_.size();
}

void NcspSpaceHybridDFS::pushSolNode(const SharedNcspNode& node)
{
   stotal_++;
   vsol_.push_back(node);

   // changes the ordering for the next DFS stage
   leftRight_ = !leftRight_;

   // moves the nodes from the stack to the set
   for (auto node : sta_)
      set_->insert(node);

   sta_.clear();
}

SharedNcspNode NcspSpaceHybridDFS::popSolNode()
{
   stotal_--;
   SharedNcspNode node = vsol_.back();
   vsol_.pop_back();
   return node;
}

SharedNcspNode NcspSpaceHybridDFS::getSolNode(size_t i) const
{
   ASSERT(i < vsol_.size(), "Bad access to a solution node in a CSP space");

   return vsol_[i];
}

bool NcspSpaceHybridDFS::hasFeasibleSolNode() const
{
   for (auto node : vsol_)
   {
      Proof p = node->getProof();
      if (p == Proof::Feasible || p == Proof::Inner)
         return true;
   }
   return false;
}

void NcspSpaceHybridDFS::makeSolClusters(double gap)
{
   // no clustering if the gap is negative
   if (gap < 0.0) return;

   // moves the solution nodes in a list
   std::list<SharedNcspNode> lnode;
   for (auto node : vsol_) lnode.push_back(node);
   vsol_.clear();

   while (!lnode.empty())
   {
      // extracts the first node
      SharedNcspNode node = lnode.front();
      IntervalBox* regnode = node->region();
      lnode.pop_front();

      // finds another node that is close enough
      bool found = false;
      auto it = lnode.begin();
      while (!found && it != lnode.end())
      {
         SharedNcspNode bis = *it;
         IntervalBox* regbis = bis->region();

         if (regnode->gap(*regbis) < gap)
         {
            // merges node in bisnode and iterates
            regbis->hullAssignOnScope(*regnode, node->scope());
            found = true;
         }
         else ++it;
      }

      // this is a solution node and no other solution is close enough
      if (!found) vsol_.push_back(node);
   }
}

size_t NcspSpaceHybridDFS::nbTotalSolNodes() const
{
   return stotal_;
}

size_t NcspSpaceHybridDFS::nbPendingNodes() const
{
   return sta_.size() + set_->size();
}

SharedNcspNode NcspSpaceHybridDFS::nextPendingNode()
{
   // gets the top of the stack if it is not empty
   if (sta_.empty())
   {
      SharedNcspNode node = set_->extract();
      return node;
   }
   // the first element of the set otherwise
   else
   {
      SharedNcspNode node = sta_.back();
      sta_.pop_back();
      return node;
   }
}

void NcspSpaceHybridDFS::insertPendingNode(const SharedNcspNode& node)
{
   // inserts a node in the stack during a DFS stage
   sta_.push_back(node);
}

SharedNcspNode NcspSpaceHybridDFS::getPendingNode(size_t i) const
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

void NcspSpaceHybridDFS::insertPendingNodes(NcspSplit::iterator first,
                                            NcspSplit::iterator last)
{
   // the nodes between first and last are ordered from left to right
   // if the DFS ordering is left to right then it is necessary to inverse
   // the range of iterators

   if (leftRight_)
   {
      std::list<SharedNcspNode> aux;
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
                                       
} // namespace
