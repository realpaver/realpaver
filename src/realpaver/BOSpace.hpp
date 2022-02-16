///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_SPACE_HPP
#define REALPAVER_BO_SPACE_HPP

#include <set>
#include <unordered_map>
#include "realpaver/BONode.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a search space used for solving Bound Optimization problems.
///
/// The nodes are selected according to the bounds of the objective function
/// and a frequency:
/// - if frequency = 1: allways the node with the lowest lower bound
/// - otherwise:        we count the number of extractions and the node with
///                     the lowest upper bound is selected if the counter is
///                     is a multiple of the frequency, otherwise the node
///                     with the lowest lower bound is selected.
///
/// It is assumed that the BO problem is a minimization problem. This
/// assumption is used when a space is simplified given an upper bound of
/// the optimal value, i.e. a minimum.
///////////////////////////////////////////////////////////////////////////////
class BOSpace {
public:
   /// Creates a space
   /// @param frequency frequency used for the selection of nodes
   BOSpace(size_t frequency = 2);

   /// No copy
   BOSpace(const BOSpace&) = delete;

   /// No assignment
   BOSpace& operator=(const BOSpace&) = delete;

   /// Default destructor
   ~BOSpace() = default;

   /// Inserts a node in this space
   /// @param node node inserted
   void insertNode(const SharedBONode& node);

   /// Extracts the first node from this
   /// @return the first node of this
   SharedBONode extractNode();

   /// @return true if this is empty
   bool isEmpty() const;

   /// @return the frequency
   size_t getFrequency() const;

   /// Sets the frequency
   /// @param frequency new value of the frequency
   void setFrequency(size_t frequency);

   /// @return the lowest lower bound
   double getLowestLowerBound() const;

   /// @return the lowest upper bound
   double getLowestUpperBound() const;

   /// Simplifies this given an upper bound of the minimum
   /// u upper bound of the optimal value (we seek for minimum)
   void simplify(const double& u);

   /// @return the number of nodes inserted in this
   int getNodeCount() const;

private:
   struct Item {
      SharedBONode node;
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

   SpaceLo lo_;         // set of nodes sorted wrt. the lower bounds
   SpaceUp up_;         // set of nodes sorted wrt. the upper bounds
   SpaceMap ma_;        // auxiliary map
   size_t id_;          // next id
   size_t ex_;          // counts the extractions
   size_t f_;           // frequency
   int nodecount_;      // counter of nodes inserted in this

   // extracts the i-th node from this space
   SharedBONode extractNode(size_t i);
};

} // namespace

#endif
