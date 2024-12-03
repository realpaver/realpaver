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
 * @file   CSPSpaceDFS.hpp
 * @brief  Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CSP_SPACE_DFS_HPP
#define REALPAVER_CSP_SPACE_DFS_HPP

#include "realpaver/CSPSpace.hpp"

namespace realpaver {

/**
 * @brief Depth-First-Search strategy.
 *
 * The pending nodes are stored in a stack in order to implement a DFS strategy.
 */
class CSPSpaceDFS : public CSPSpace {
public:
   /// Constructor
   CSPSpaceDFS();

   /// Default destructor
   ~CSPSpaceDFS() = default;

   /// No assignment
   CSPSpaceDFS& operator=(const CSPSpaceDFS&) = delete;

   /// Default copy constructor
   CSPSpaceDFS(const CSPSpaceDFS&) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedCSPNode& node) override;
   SharedCSPNode popSolNode() override;
   SharedCSPNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedCSPNode nextPendingNode() override;
   void insertPendingNode(const SharedCSPNode& node) override;
   SharedCSPNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedCSPNode> vsol_;    // vector of solution nodes
   std::vector<SharedCSPNode> vnode_;   // vector of pending nodes
};

} // namespace

#endif
