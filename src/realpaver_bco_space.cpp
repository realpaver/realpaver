// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_bco_space.hpp"

namespace realpaver {

BcoSpace::BcoSpace(size_t frequency) :
   lo_(), up_(), ma_(), id_(0), ex_(0)
{
   setFrequency(frequency);
}

void BcoSpace::setFrequency(size_t frequency)
{
   ASSERT(frequency >= 1, "Bad frequency");
   f_ = frequency;
}

void BcoSpace::insertNode(const SharedBcoNode& node)
{
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

SharedBcoNode BcoSpace::extractNode()
{
   // one more extraction
   ++ex_;

   // chooses an item
   size_t i = (f_ == 1 || ex_ % f_ != 0) ? lo_.begin()->id :
                                           up_.begin()->id;

   return extractNode(i);
}

SharedBcoNode BcoSpace::extractNode(size_t i)
{
   // finds the i-th node in the map
   auto itm = ma_.find(i);
   SharedBcoNode node = itm->second.itlo->node;

   // removes it
   lo_.erase(itm->second.itlo);
   up_.erase(itm->second.itup);
   ma_.erase(itm);

   return node;   
}

double BcoSpace::getLowestLowerBound() const
{
   return lo_.begin()->node->getLower();
}

double BcoSpace::getLowestUpperBound() const
{
   return up_.begin()->node->getUpper();
}

void BcoSpace::simplify(const double& u)
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
