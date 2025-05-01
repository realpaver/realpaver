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
 * @file   CSPSplit.cpp
 * @brief  Splitting strategies of CSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */
#include "AssertDebug.hpp"
#include "IntervalFunctionVector.hpp"
#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/CSPSplit.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

CSPSplit::CSPSplit(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : scop_(scop)
    , slicerMap_(smap.release())
    , cont_()
    , nbs_(0)
    , idx_(0)
{
   ASSERT(!scop.isEmpty(), "Creation of a split object with an empty scope");
   ASSERT(slicerMap_ != nullptr, "No domain slicer map in a split object");
}

CSPSplit::~CSPSplit()
{
   delete slicerMap_;
}

Scope CSPSplit::scope() const
{
   return scop_;
}

void CSPSplit::apply(SharedCSPNode &node, CSPContext &context)
{
   LOG_INTER("Split node " << node->index() << ": " << (*node->box()));

   cont_.clear();
   ++nbs_;

   applyImpl(node, context);

   LOG_INTER("  -> " << getNbNodes() << " sub-node(s)");
}

size_t CSPSplit::getNbNodes() const
{
   return cont_.size();
}

size_t CSPSplit::getNbSplits() const
{
   return nbs_;
}

SharedCSPNode CSPSplit::cloneNode(const SharedCSPNode &node)
{
   SharedCSPNode aux = std::make_shared<CSPNode>(*node);
   aux->setIndex(++idx_);
   aux->setDepth(1 + node->depth());
   aux->setParent(node->index());
   return aux;
}

void CSPSplit::splitOne(SharedCSPNode &node, Variable v)
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
      SharedCSPNode aux = cloneNode(node);
      aux->box()->set(v, std::move(slice));
      cont_.push_back(aux);
   }
}

void CSPSplit::reset()
{
   nbs_ = idx_ = 0;
}

CSPSplit::iterator CSPSplit::begin()
{
   return cont_.begin();
}

CSPSplit::iterator CSPSplit::end()
{
   return cont_.end();
}

DomainSlicerMap *CSPSplit::getSlicerMap() const
{
   return slicerMap_;
}

/*----------------------------------------------------------------------------*/

CSPSplitRR::CSPSplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : CSPSplit(scop, std::move(smap))
{
}

void CSPSplitRR::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node, context);
   if (!res.first)
      return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Round-Robin selects " << v.getName() << " in node " << node->index());

   if (getNbNodes() < 2)
      return;

   // assigns the split variable in the sub-nodes
   std::shared_ptr<CSPNodeInfoVar> info = std::make_shared<CSPNodeInfoVar>(v);

   for (SharedCSPNode &aux : cont_)
      context.insert(aux->index(), info);
}

std::pair<bool, Variable> CSPSplitRR::selectVar(SharedCSPNode &node, CSPContext &context)
{
   DomainBox *box = node->box();

   std::shared_ptr<CSPNodeInfo> info =
       context.getInfo(node->index(), CSPNodeInfoType::SplitVar);

   // assigns an iterator pointing to the next variable
   Scope::const_iterator it;
   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      CSPNodeInfoVar *infovar = static_cast<CSPNodeInfoVar *>(info.get());
      it = scop_.find(infovar->getVar());
      ++it;
      if (it == scop_.end())
         it = scop_.begin();
   }

   bool found = false;
   size_t nb = 0;

   while (!found && nb < scop_.size())
   {
      if (box->isSplitable(*it))
      {
         found = true;
      }
      else
      {
         ++nb;
         ++it;
         if (it == scop_.end())
            it = scop_.begin();
      }
   }

   return std::make_pair(found, *it);
}

/*----------------------------------------------------------------------------*/

CSPSplitLF::CSPSplitLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : CSPSplit(scop, std::move(smap))
    , sel_(scop)
{
}

void CSPSplitLF::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   if (sel_.apply(*node->box()))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("LF selects " << v.getName() << " in node " << node->index());
   }
}

/*----------------------------------------------------------------------------*/

CSPSplitSF::CSPSplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : CSPSplit(scop, std::move(smap))
    , sel_(scop)
{
}

void CSPSplitSF::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   if (sel_.apply(*node->box()))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("SF selects " << v.getName() << " in node " << node->index());
   }
}

/*----------------------------------------------------------------------------*/

CSPSplitSLF::CSPSplitSLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
    : CSPSplit(scop, std::move(smap))
    , sel_(scop)
{
}

void CSPSplitSLF::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   if (sel_.apply(*node->box()))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("SLF selects " << v.getName() << " in node " << node->index());
   }
}

/*----------------------------------------------------------------------------*/

CSPSplitSSR::CSPSplitSSR(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                         IntervalFunctionVector F)
    : CSPSplit(scop, std::move(smap))
    , ssr_(F)
    , sbis_(scop.setminus(F.scope()))
{
   ASSERT(scop.contains(F.scope()), "Bad scopes");
}

void CSPSplitSSR::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   const DomainBox &box = *node->box();

   if (ssr_.apply(box))
   {
      Variable v = ssr_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("SSR selects " << v.getName() << " in node " << node->index());
      return;
   }

   if (!sbis_.isEmpty())
   {
      SelectorLF sel(sbis_);
      if (sel.apply(box))
      {
         Variable v = sel.getSelectedVar();
         splitOne(node, v);
         LOG_INTER("LF selects " << v.getName() << " in node " << node->index());
      }
   }
}

/*----------------------------------------------------------------------------*/

CSPSplitASR::CSPSplitASR(Scope scop, std::unique_ptr<DomainSlicerMap> smap, SharedDag dag)
    : CSPSplit(scop, std::move(smap))
    , asr_(dag)
    , sbis_(scop.setminus(dag->scope()))
{
   ASSERT(dag != nullptr, "Null pointer");
}

void CSPSplitASR::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   const DomainBox &box = *node->box();

   if (asr_.apply(box))
   {
      Variable v = asr_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("ASR selects " << v.getName() << " in node " << node->index());
      return;
   }

   if (!sbis_.isEmpty())
   {
      SelectorLF sel(sbis_);
      if (sel.apply(box))
      {
         Variable v = sel.getSelectedVar();
         splitOne(node, v);
         LOG_INTER("LF selects " << v.getName() << " in node " << node->index());
      }
   }
}

/*----------------------------------------------------------------------------*/

CSPSplitHybridSSR::CSPSplitHybridSSR(Scope scop, std::unique_ptr<DomainSlicerMap> smap,
                                     IntervalFunctionVector F)
    : CSPSplit(scop, std::move(smap))
    , sel_(scop, F)
{
}

double CSPSplitHybridSSR::getFrequency() const
{
   return sel_.getFrequency();
}

void CSPSplitHybridSSR::setFrequency(double f)
{
   sel_.setFrequency(f);
}

void CSPSplitHybridSSR::applyImpl(SharedCSPNode &node, CSPContext &context)
{
   if (sel_.apply(*node->box()))
   {
      Variable v = sel_.getSelectedVar();
      splitOne(node, v);
      LOG_INTER("HybridSSR selects " << v.getName() << " in node " << node->index());
   }
}

} // namespace realpaver
