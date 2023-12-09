///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSplit.hpp"

namespace realpaver {

NcspSplit::NcspSplit(std::unique_ptr<VariableSelector> selector,
                     std::unique_ptr<IntervalSlicer> slicer)
      : SplitStrategy<SharedNcspNode>(),
        selector_(std::move(selector)),
        slicer_(std::move(slicer))
{
   ASSERT(selector_ != nullptr, "No selector in a split object");
   ASSERT(slicer_ != nullptr, "No slicer in a split object");
}

bool NcspSplit::applyImpl(SharedNcspNode node)
{
   IntervalBox* B = node->region();

   std::pair<bool, Variable> p = selector_->selectVar(*node);
   if (!p.first) return false;

   Variable v = p.second;

   size_t n = slicer_->apply(B->get(v));
   if (n < 2) return false;

   // reuses the input node
   auto it = slicer_->begin();
   B->set(v, *it);
   push(node);

   // generates the other nodes
   while (++it != slicer_->end())
   {
      SharedNcspNode aux = std::make_shared<NcspNode>(*node);
      aux->region()->set(v, *it);
      push(aux);
   }

   return true;
}

} // namespace
