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
#include "realpaver/Logger.hpp"

namespace realpaver {


NcspSplit::NcspSplit(std::unique_ptr<NcspSelector> selector,
                     std::unique_ptr<DomainSlicerMap> smap)
      : SplitStrategy<SharedNcspNode>(),
        selector_(selector.release()),
        smap_(smap.release())
{
   ASSERT(selector_ != nullptr, "No selector in a split object");
   ASSERT(smap_ != nullptr, "No slicer map in a split object");
}

NcspSplit::~NcspSplit()
{
   delete selector_;
   delete smap_;
}

bool NcspSplit::applyImpl(SharedNcspNode node)
{
   DomainBox* B = node->box();

   LOG_INTER("Split node " << node->index() << ": " << (*B));

   std::pair<bool, Variable> p = selector_->selectVar(*node);

   if (!p.first) return false;

   Variable v = p.second;
   DomainSlicer* slicer = smap_->getSlicer(v);

   size_t n = slicer->apply(B->get(v));
   if (n < 2) return false;

   // reuses the input node with the first slice
   auto it = slicer->begin();
   std::unique_ptr<Domain> slice = slicer->next(it);
   B->set(v, std::move(slice));
   push(node);

   // new nodes for the other slices
   while (it != slicer->end())
   {
      slice = slicer->next(it);
      SharedNcspNode aux = std::make_shared<NcspNode>(*node);
      aux->box()->set(v, std::move(slice));
      push(aux);      
   }

   return true;
}

} // namespace
