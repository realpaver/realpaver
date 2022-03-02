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

BOSplit::BOSplit(Selector* selector, IntervalSlicer* slicer)
      : SplitStrategy<SharedBONode>(),
        selector_(selector),
        slicer_(slicer)
{
   ASSERT(selector != nullptr, "No selector in a split object");
   ASSERT(slicer != nullptr, "No slicer in a split object");
}

BOSplit::~BOSplit()
{
   if (selector_ != nullptr) delete selector_;
   if (slicer_ != nullptr) delete slicer_;
}

bool BOSplit::applyImpl(SharedBONode node)
{
   IntervalVector* X = node->getRegion();

   std::pair<bool, Variable> p = selector_->selectVar(*X);
   if (!p.first) return false;

   size_t id = p.second.getId();

   size_t n = slicer_->apply(X->at(id));
   if (n < 2) return false;

   // reuses the input node
   auto it = slicer_->begin();
   X->set(id, *it);
   push(node);

   // generates the other nodes
   while (++it != slicer_->end())
   {
      SharedBONode aux = std::make_shared<BONode>(*node);
      aux->getRegion()->set(id, *it);
      push(aux);
   }

   return true;
}

} // namespace
