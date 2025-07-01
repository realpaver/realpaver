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
 * @file   Split.cpp
 * @brief  Base class of splitting strategies
 * @author Laurent Granvilliers
 * @date   10 June 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Split.hpp"

namespace realpaver {

Split::Split(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : scop_(scop)
    , slicerMap_(smap.release())
    , cont_()
    , nbs_(0)
    , idx_(0)
{
   ASSERT(!scop.isEmpty(), "Creation of a split object with an empty scope");
   ASSERT(slicerMap_ != nullptr, "No domain slicer map in a split object");
}

Split::~Split()
{
   delete slicerMap_;
}

Scope Split::scope() const
{
   return scop_;
}

void Split::apply(SharedNode &node, NodeContext &context)
{
   LOG_INTER("Split node " << node->index() << ": " << (*node->box()));

   cont_.clear();
   ++nbs_;

   applyImpl(node, context);

   LOG_INTER("  -> " << getNbNodes() << " sub-node(s)");
}

size_t Split::getNbNodes() const
{
   return cont_.size();
}

size_t Split::getNbSplits() const
{
   return nbs_;
}

SharedNode Split::cloneNode(const SharedNode &node)
{
   SharedNode aux = std::make_shared<Node>(*node);
   aux->setIndex(++idx_);
   aux->setDepth(1 + node->depth());
   aux->setParent(node->index());
   return aux;
}

void Split::splitOne(SharedNode &node, Variable v)
{
   Domain *dom = node->box()->get(v);
   DomainSlicer *slicer = slicerMap_->getSlicer(dom->type());

   size_t n = slicer->apply(dom);
   if (n < 2)
      return;

   auto it = slicer->begin();
   while (it != slicer->end())
   {
      std::unique_ptr<Domain> slice = slicer->next(it);
      SharedNode aux = cloneNode(node);
      aux->box()->set(v, std::move(slice));
      cont_.push_back(aux);
   }
}

void Split::reset()
{
   nbs_ = idx_ = 0;
}

Split::iterator Split::begin()
{
   return cont_.begin();
}

Split::iterator Split::end()
{
   return cont_.end();
}

DomainSlicerMap *Split::getSlicerMap() const
{
   return slicerMap_;
}

} // namespace realpaver
