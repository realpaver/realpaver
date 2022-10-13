///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPACE_DFS_HPP
#define REALPAVER_NCSP_SPACE_DFS_HPP

#include "realpaver/NcspSpace.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a space generated by a branch-and-prune algorithm for solving CSPs
/// that implements a Depth-First-Search strategy.
///////////////////////////////////////////////////////////////////////////////
class NcspSpaceDFS : public NcspSpace {
public:
   /// Default constructor
   NcspSpaceDFS() = default;

   /// Default destructor
   ~NcspSpaceDFS() = default;

   /// No assignment
   NcspSpaceDFS& operator=(const NcspSpaceDFS&) = delete;

   /// Default copy constructor
   NcspSpaceDFS(const NcspSpaceDFS&) = default;

   ///@{
   /// Overrides
   size_t nbPendingNodes() const;
   SharedNcspNode extractPendingNode();
   void insertPendingNode(const SharedNcspNode& node);
   ///@}

private:
   std::vector<SharedNcspNode> vnode_;   // vector of pending nodes
};

} // namespace

#endif
