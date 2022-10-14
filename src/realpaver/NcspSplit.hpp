///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NCSP_SPLIT_HPP
#define REALPAVER_NCSP_SPLIT_HPP

#include <list>
#include "realpaver/NcspNode.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/SplitStrategy.hpp"
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This implements a splitting strategy for NCSP nodes.
///////////////////////////////////////////////////////////////////////////////
class NcspSplit : public SplitStrategy<SharedNcspNode> {
public:
   /// Type of nodes
   typedef SharedNcspNode NodeType;

   /// Creates a splitting object
   NcspSplit(std::unique_ptr<VariableSelector> selector,
             std::unique_ptr<IntervalSlicer> slicer);

   /// Default destructor
   ~NcspSplit() = default;

   /// No copy
   NcspSplit(const NcspSplit&) = delete;

   /// No assignment
   NcspSplit& operator=(const NcspSplit&) = delete;

   /// Overrides
   bool applyImpl(SharedNcspNode node);

private:
   std::unique_ptr<VariableSelector> selector_;
   std::unique_ptr<IntervalSlicer> slicer_;
};

} // namespace

#endif
