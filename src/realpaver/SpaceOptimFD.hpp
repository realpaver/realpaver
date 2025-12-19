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
 * @file   SpaceOptimFD.hpp
 * @brief  Search space of branch-and-bound algorithm
 * @author Laurent Granvilliers
 * @date   17 Nov 2025
 */

#ifndef REALPAVER_SPACE_OPTIM_FD_HPP
#define REALPAVER_SPACE_OPTIM_FD_HPP

#include "realpaver/SpaceOptim.hpp"
#include <set>

namespace realpaver {

/*
 * @brief Search space of branch-and-bound algorithm.
 *
 * The selection strategy is called feasible diving. It alternates a best-first searh
 * strategy that selects the node with the slowest lower bound and a depth-first search
 * strategy that seeks for feasible points. See: B. Neveu, G. Trombettoni, I. Araya. Node
 * Selection Strategies in Interval Branch and Bound Algorithms. Journal of Global
 * Optimization, 64 (2), 289-304, 2016.
 *
 * It is assumed that the problem is a minimization problem. This assumption is used
 * when a space is simplified given an upper bound of the optimal value, i.e. a minimum.
 */
class SpaceOptimFD : public SpaceOptim {
public:
   /// Creates an empty space
   SpaceOptimFD();

   /// No copy
   SpaceOptimFD(const SpaceOptimFD &) = delete;

   /// No assignment
   SpaceOptimFD &operator=(const SpaceOptimFD &) = delete;

   /// Default destructor
   ~SpaceOptimFD() = default;

   double getLowestLowerBound() const override;
   void simplifyImpl(double u) override;
   void print(std::ostream &os) const override;

   size_t nbPendingNodes() const override;
   SharedNodeOptim popPendingNode() override;
   void pushPendingNode(const SharedNodeOptim &node) override;
   SharedNodeOptim getPendingNode(size_t i) const override;

private:
   // Comparator used to sort a set of nodes by increasing ordering of the lower bounds
   // and, when the bounds are equal, by increasing ordering of the depths
   struct NodeCmpLo {
      bool operator()(const SharedNodeOptim &x, const SharedNodeOptim &y) const
      {
         return (x->getLower() < y->getLower()) ||
                ((x->getLower() == y->getLower()) && (x->depth() < y->depth()));
      }
   };

   std::set<SharedNodeOptim, NodeCmpLo> nod_; // set of open nodes
   SharedNodeOptim next_;                     // next node in the DFS search
   int last_;                                 // index of the last extracted node
};

} // namespace realpaver

#endif
