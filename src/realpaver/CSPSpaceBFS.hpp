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
 * @file   CSPSpaceBFS.hpp
 * @brief  Breadth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CSP_SPACE_BFS_HPP
#define REALPAVER_CSP_SPACE_BFS_HPP

#include "realpaver/CSPSpace.hpp"
#include <list>

namespace realpaver {

/**
 * @brief Breadth-First-Search strategy.
 *
 * The pending nodes are stored in a FIFO queue in order to implement a BFS
 * strategy.
 */
class CSPSpaceBFS : public CSPSpace {
public:
   /// Constructor
   CSPSpaceBFS();

   /// Default destructor
   ~CSPSpaceBFS() = default;

   /// No assignment
   CSPSpaceBFS &operator=(const CSPSpaceBFS &) = delete;

   /// Default copy constructor
   CSPSpaceBFS(const CSPSpaceBFS &) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedCSPNode &node) override;
   SharedCSPNode popSolNode() override;
   SharedCSPNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedCSPNode nextPendingNode() override;
   void insertPendingNode(const SharedCSPNode &node) override;
   SharedCSPNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedCSPNode> vsol_; // vector of solution nodes
   std::list<SharedCSPNode> lnode_;  // list of pending nodes
};

} // namespace realpaver

#endif
