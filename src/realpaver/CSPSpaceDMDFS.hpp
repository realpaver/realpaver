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
 * @file   CSPSpaceDMDFS.hpp
 * @brief  Distant-Most Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CSP_SPACE_DMDFS_HPP
#define REALPAVER_CSP_SPACE_DMDFS_HPP

#include "realpaver/CSPSpace.hpp"

namespace realpaver {

/// Base class of objects that calculate the distance between boxes
class DistCalculator {
public:
   /// Virtual destructor
   virtual ~DistCalculator();

   /// Calculates the distance between boxes
   virtual double distance(const DomainBox& db1, const DomainBox& db2) = 0;
};

/*----------------------------------------------------------------------------*/

/// Calculates the Hausdorff distance between boxes
class HausdorffDistCalculator : public DistCalculator {
public:
   double distance(const DomainBox& db1, const DomainBox& db2) override;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Distant-Most Depth-First-Search strategy.
 *
 * Space generated by a branch-and-prune algorithm for solving CSPs that
 * implements a Distant-Most Depth-First-Search strategy. A DFS strategy is used
 * to find the next solution. When a solution is found, the list of pending
 * nodes is sorted according to a decreasing ordering of the distance of each
 * node to its closest solution. This is a way to seek diverse solutions at
 * any time of the search process.
 *
 * The distance used can be parameterized through a distance calculator.
 * The default one is the Hausdorff distance.
 */
class CSPSpaceDMDFS : public CSPSpace {
public:
   /// Constructor
   CSPSpaceDMDFS();

   /// Destructor
   ~CSPSpaceDMDFS();

   /// No assignment
   CSPSpaceDMDFS& operator=(const CSPSpaceDMDFS&) = delete;

   /// Default copy constructor
   CSPSpaceDMDFS(const CSPSpaceDMDFS&) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedCSPNode& node) override;
   SharedCSPNode popSolNode() override;
   SharedCSPNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   void makeSolClusters(double gap) override;
   size_t nbPendingNodes() const override;
   SharedCSPNode nextPendingNode() override;
   void insertPendingNode(const SharedCSPNode& node) override;
   SharedCSPNode getPendingNode(size_t i) const override;

   /// Assigns the distance calculator in this
   void setDistCalculator(std::unique_ptr<DistCalculator> dcalc);

private:
   struct Elem {
      SharedCSPNode node;    // node
      double mindist;         // distance to the closest solution
   };

   struct Comp {
      bool operator()(const Elem& x, const Elem& y)
      {
         return (x.mindist < y.mindist) ||
                 (x.mindist == y.mindist && x.node->index() < y.node->index());
      }
   } comparator;

   std::vector<Elem> vnode_;           // vector of pending nodes
   std::vector<SharedCSPNode> vsol_;  // vector of solution nodes

   DistCalculator* dcalc_;             // object used to calculate the distance
                                       // between boxes
};

} // namespace

#endif
