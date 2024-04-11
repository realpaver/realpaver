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
 * @file   NcspSpaceDFS.hpp
 * @brief  Depth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_NCSP_SPACE_DFS_HPP
#define REALPAVER_NCSP_SPACE_DFS_HPP

#include "realpaver/NcspSpace.hpp"

namespace realpaver {

/**
 * @brief Depth-First-Search strategy.
 * 
 * The pending nodes are stored in a stack in order to implement a DFS strategy.
 */
class NcspSpaceDFS : public NcspSpace {
public:
   /// Constructor
   NcspSpaceDFS();

   /// Default destructor
   ~NcspSpaceDFS() = default;

   /// No assignment
   NcspSpaceDFS& operator=(const NcspSpaceDFS&) = delete;

   /// Default copy constructor
   NcspSpaceDFS(const NcspSpaceDFS&) = default;

   size_t nbSolNodes() const override;
   void pushSolNode(const SharedNcspNode& node) override;
   SharedNcspNode popSolNode() override;
   SharedNcspNode getSolNode(size_t i) const override;
   bool hasFeasibleSolNode() const override;
   size_t nbPendingNodes() const override;
   SharedNcspNode nextPendingNode() override;
   void insertPendingNode(const SharedNcspNode& node) override;
   SharedNcspNode getPendingNode(size_t i) const override;

private:
   std::vector<SharedNcspNode> vsol_;    // vector of solution nodes
   std::vector<SharedNcspNode> vnode_;   // vector of pending nodes
};

} // namespace

#endif
