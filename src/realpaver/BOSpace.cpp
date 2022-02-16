///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/BOSpace.hpp"

namespace realpaver {

BOSpace::BOSpace(size_t frequency)
      : lo_(),
        up_(),
        ma_(),
        id_(0),
        ex_(0),
        nodecount_(0)
{
   setFrequency(frequency);
}

void BOSpace::setFrequency(size_t frequency)
{
   ASSERT(frequency >= 1, "Bad frequency");
   f_ = frequency;
}

bool BOSpace::isEmpty() const
{
   return lo_.empty();
}

size_t BOSpace::getFrequency() const
{
   return f_;
}

double BOSpace::getLowestLowerBound() const
{
   return lo_.begin()->node->getLower();
}

double BOSpace::getLowestUpperBound() const
{
   return up_.begin()->node->getUpper();
}

int BOSpace::getNodeCount() const
{
   return nodecount_;
}

void BOSpace::insertNode(const SharedBONode& node)
{
   // one mode node
   ++nodecount_;

   // new item
   size_t i = id_++;
   Item item = {node, i};

   // inserts the node in the sets
   auto itlo = lo_.insert(item);
   auto itup = up_.insert(item);

   // registers the couple of iterators in the map
   SpaceIter ita = {itlo, itup};
   ma_.insert(std::make_pair(i, ita));
}

SharedBONode BOSpace::extractNode()
{
   // one more extraction
   ++ex_;

   // chooses an item
   size_t i = (f_ == 1 || ex_ % f_ != 0) ? lo_.begin()->id :
                                           up_.begin()->id;

   return extractNode(i);
}

SharedBONode BOSpace::extractNode(size_t i)
{
   // finds the i-th node in the map
   auto itm = ma_.find(i);
   SharedBONode node = itm->second.itlo->node;

   // removes it
   lo_.erase(itm->second.itlo);
   up_.erase(itm->second.itup);
   ma_.erase(itm);

   return node;   
}

void BOSpace::simplify(const double& u)
{
   while (true)
   {
      auto it = lo_.rbegin();
      if (it == lo_.rend())
         return;
      
      if (it->node->getLower() > u)
         extractNode(it->id);
      
      else
         return;
   }
}

} // namespace
