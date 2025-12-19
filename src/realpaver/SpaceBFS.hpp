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
 * @file   SpaceBFS.hpp
 * @brief  Search space for a Breadth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_SPACE_BFS_HPP
#define REALPAVER_SPACE_BFS_HPP

#include "realpaver/Node.hpp"
#include "realpaver/Space.hpp"
#include <list>

namespace realpaver {

/**
 * @brief Breadth-First-Search strategy.
 *
 * The pending nodes are stored in a FIFO queue in order to implement a BFS
 * strategy.
 */
class SpaceBFS : public Space<Node> {
public:
   /// Constructor
   SpaceBFS();

   /// Default destructor
   ~SpaceBFS() = default;

   /// No assignment
   SpaceBFS &operator=(const SpaceBFS &) = delete;

   /// Default copy constructor
   SpaceBFS(const SpaceBFS &) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNode &node) override;
   SharedNode popSolNode() override;
   SharedNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedNode popPendingNode() override;
   void pushPendingNode(const SharedNode &node) override;
   SharedNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedNode> vsol_; // vector of solution nodes
   std::list<SharedNode> lnode_;  // list of pending nodes
};

} // namespace realpaver

#endif
