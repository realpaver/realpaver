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
 * @file   NcspSpaceBFS.hpp
 * @brief  Breadth-First-Search strategy
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_NCSP_SPACE_BFS_HPP
#define REALPAVER_NCSP_SPACE_BFS_HPP

#include <list>
#include "realpaver/NcspSpace.hpp"

namespace realpaver {

/**
 * @brief Breadth-First-Search strategy.
 * 
 * The pending nodes are stored in a FIFO queue in order to implement a BFS
 * strategy.
 */
class NcspSpaceBFS : public NcspSpace {
public:
   /// Constructor
   NcspSpaceBFS();

   /// Default destructor
   ~NcspSpaceBFS() = default;

   /// No assignment
   NcspSpaceBFS& operator=(const NcspSpaceBFS&) = delete;

   /// Default copy constructor
   NcspSpaceBFS(const NcspSpaceBFS&) = default;

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
   std::list<SharedNcspNode> lnode_;     // list of pending nodes
};

} // namespace

#endif
