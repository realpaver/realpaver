// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_BCO_SPACE_HPP
#define REALPAVER_BCO_SPACE_HPP

#include <set>
#include <unordered_map>
#include "realpaver/bco_node.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of search spaces for bound-constrained optimization.
 * 
 * The nodes are selected according to the bounds of the objective function
 * and a frequeny of choice :
 *   - if frequency = 1: allways the node with the lowest lower bound
 *   - otherwise:        we count the number of extractions
 *                       each time count % frequency = 0 the node with the
 *                       lowest upper bound is selected, otherwise the node
 *                       with the lowest lower bound is selected
 *****************************************************************************/
class BcoSpace {
public:
   // constructor
   // 
   BcoSpace(size_t frequency = 2);

   // copy protection
   BcoSpace(const BcoSpace&) = delete;
   BcoSpace& operator=(const BcoSpace&) = delete;

   // inserts a new node in this space
   void insertNode(const SharedBcoNode& node);

   // extracts the next node from this space
   SharedBcoNode extractNode();

   // returns true if this space is empty
   bool isEmpty() const;

   // management of the frequency
   size_t getFrequency() const;
   void setFrequency(size_t frequency);

   // returns the lowest lower bound
   double getLowestLowerBound() const;

   // returns the lowest upper bound
   double getLowestUpperBound() const;

   // simplifies this space given an upper bound of the minimum
   void simplify(const double& u);

private:
   struct Item {
      SharedBcoNode node;
      size_t id;
   };

   struct ItemCmpLo {
      bool operator()(const Item& x, const Item& y) {
         return x.node->getLower() < y.node->getLower();
      }
   };

   struct ItemCmpUp {
      bool operator()(const Item& x, const Item& y) {
         return x.node->getUpper() < y.node->getUpper();
      }
   };

   typedef std::multiset<Item, ItemCmpLo> SpaceLo;
   typedef std::multiset<Item, ItemCmpUp> SpaceUp;

   struct SpaceIter {
      SpaceLo::iterator itlo;
      SpaceUp::iterator itup;
   };

   typedef std::unordered_map<size_t, SpaceIter> SpaceMap;

   SpaceLo lo_;   // set of nodes sorted wrt. the lower bounds
   SpaceUp up_;   // set of nodes sorted wrt. the upper bounds
   SpaceMap ma_;  // auxiliary map
   size_t id_;    // next id
   size_t ex_;    // counts the extractions
   size_t f_;     // frequency

   // extracts the i-th node from this space
   SharedBcoNode extractNode(size_t i);
};

inline bool BcoSpace::isEmpty() const
{
   return lo_.empty();
}

inline size_t BcoSpace::getFrequency() const
{
   return f_;
}

inline double BcoSpace::getLowestLowerBound() const
{
   return lo_.begin()->node->getLower();
}

inline double BcoSpace::getLowestUpperBound() const
{
   return up_.begin()->node->getUpper();
}

} // namespace

#endif
