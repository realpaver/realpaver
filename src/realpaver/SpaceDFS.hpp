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
 * @file   SpaceDFS.hpp
 * @brief  Search space for a Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_SPACE_DFS_HPP
#define REALPAVER_SPACE_DFS_HPP

#include "realpaver/Space.hpp"

namespace realpaver {

/**
 * @brief Depth-First-Search strategy.
 *
 * The pending nodes are stored in a stack in order to implement a DFS strategy.
 */
class SpaceDFS : public Space {
public:
   /// Constructor
   SpaceDFS();

   /// Default destructor
   ~SpaceDFS() = default;

   /// No assignment
   SpaceDFS &operator=(const SpaceDFS &) = delete;

   /// Default copy constructor
   SpaceDFS(const SpaceDFS &) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNode &node) override;
   SharedNode popSolNode() override;
   SharedNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedNode nextPendingNode() override;
   void insertPendingNode(const SharedNode &node) override;
   SharedNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedNode> vsol_;  // vector of solution nodes
   std::vector<SharedNode> vnode_; // vector of pending nodes
};

} // namespace realpaver

#endif
