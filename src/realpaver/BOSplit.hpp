///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_BO_SPLIT_HPP
#define REALPAVER_BO_SPLIT_HPP

#include <list>
#include "realpaver/BONode.hpp"
#include "realpaver/SplitStrategy.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a base class for Bound Optimization splitting strategies.
///
/// An object of this class divides the region of a given node. The sub-nodes
/// are inserted in a container. Iterators on this container are provided.
///////////////////////////////////////////////////////////////////////////////
class BOSplit : public SplitStrategy<SharedBONode> {
public:
   /// Type of nodes
   typedef SharedBONode NodeType;

   /// Creates a splitting object
   BOSplit();

   /// Destructor
   ~BOSplit();

   /// No copy
   BOSplit(const BOSplit&) = delete;

   /// No assignment
   BOSplit& operator=(const BOSplit&) = delete;

   /// Overrides
   bool applyImpl(SharedBONode node);
};

} // namespace

#endif
