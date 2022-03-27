///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOSplit.hpp"

namespace realpaver {

BOSplit::BOSplit(std::unique_ptr<Selector> selector,
                 std::unique_ptr<IntervalSlicer> slicer)
      : SplitStrategy<SharedBONode>(),
        selector_(std::move(selector)),
        slicer_(std::move(slicer))
{
   ASSERT(selector_ != nullptr, "No selector in a split object");
   ASSERT(slicer_ != nullptr, "No slicer in a split object");
}

bool BOSplit::applyImpl(SharedBONode node)
{
   IntervalRegion* reg = node->region();

   std::pair<bool, Variable> p = selector_->selectVar(*node);
   if (!p.first) return false;

   Variable v = p.second;

   size_t n = slicer_->apply(reg->get(v));
   if (n < 2) return false;

   // reuses the input node
   auto it = slicer_->begin();
   reg->set(v, *it);
   node->setSplitVariable(v);
   push(node);

   // generates the other nodes
   while (++it != slicer_->end())
   {
      SharedBONode aux = std::make_shared<BONode>(*node);
      aux->region()->set(v, *it);
      aux->setSplitVariable(v);
      push(aux);
   }

   return true;
}

} // namespace
