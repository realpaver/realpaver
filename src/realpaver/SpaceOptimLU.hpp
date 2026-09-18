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
 * @file   SpaceOptimLU.hpp
 * @brief  Search space of branch-and-bound algorithm
 * @author Laurent Granvilliers
 * @date   5 Nov 2025
 */

#ifndef REALPAVER_SPACE_OPTIM_LU_HPP
#define REALPAVER_SPACE_OPTIM_LU_HPP

#include "realpaver/DoubleRandom.hpp"
#include "realpaver/SpaceOptim.hpp"
#include <unordered_map>

namespace realpaver {

/*
 * @brief Search space of branch-and-bound algorithm.
 *
 * The nodes are selected according to the bounds of the objective function, either
 * the lowest Lower bound or the lowest Upper bound. The choice (L or U) is
 * controlled by a ratio R in [0, 1]. In fact, U is used with probability R, e.g.
 * R=0 means that L is used, R=1 means that U is used.
 *
 * It is assumed that the problem is a minimization problem. This assumption is used
 * when a space is simplified given an upper bound of the optimal value, i.e. a minimum.
 */
class SpaceOptimLU : public SpaceOptim {
public:
   /// Creates an empty space
   SpaceOptimLU();

   /// No copy
   SpaceOptimLU(const SpaceOptimLU &) = delete;

   /// No assignment
   SpaceOptimLU &operator=(const SpaceOptimLU &) = delete;

   /// Default destructor
   ~SpaceOptimLU() = default;

   /// Returns the ratio
   double getRatio() const;

   /// Assigns the ratio
   void setRatio(double ratio);

   double getLowestLowerBound() const override;
   void simplifyImpl(double u) override;
   void print(std::ostream &os) const override;

   size_t nbPendingNodes() const override;
   SharedNodeOptim popPendingNode() override;
   void pushPendingNode(const SharedNodeOptim &node) override;
   SharedNodeOptim getPendingNode(size_t i) const override;

private:
   struct Item {
      SharedNodeOptim node;
      int id;
   };

   struct ItemCmpLo {
      bool operator()(const Item &x, const Item &y) const
      {
         return (x.node->getLower() < y.node->getLower()) ||
                ((x.node->getLower() == y.node->getLower()) &&
                 (x.node->depth() < y.node->depth()));
      }
   };

   struct ItemCmpUp {
      bool operator()(const Item &x, const Item &y) const
      {
         return (x.node->getUpper() < y.node->getUpper()) ||
                ((x.node->getUpper() == y.node->getUpper()) &&
                 (x.node->depth() < y.node->depth()));
      }
   };

   typedef std::multiset<Item, ItemCmpLo> SpaceLo;
   typedef std::multiset<Item, ItemCmpUp> SpaceUp;

   struct SpaceIter {
      SpaceLo::iterator itlo;
      SpaceUp::iterator itup;
   };

   typedef std::unordered_map<size_t, SpaceIter> SpaceMap;

   SpaceLo lo_;        // set of nodes sorted wrt. the lower bounds
   SpaceUp up_;        // set of nodes sorted wrt. the upper bounds
   SpaceMap ma_;       // auxiliary map
   double r_;          // ratio
   DoubleRandom rand_; // generator of pseudo-random numbers

   // extracts the i-th node from this space
   SharedNodeOptim extractNode(int i);

   // Inserts a node in this space
   void insertNode(SharedNodeOptim node);

   // Extracts the first node from this
   SharedNodeOptim extractNode();
};

} // namespace realpaver

#endif
