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
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/CSPSplit.hpp"

namespace realpaver {

CSPSplit::CSPSplit(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : scop_(scop),
        slicerMap_(smap.release()),
        cont_(),
        nbs_(0),
        idx_(0)
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

void CSPSplit::apply(SharedCSPNode& node, CSPContext& context)
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

SharedCSPNode CSPSplit::cloneNode(const SharedCSPNode& node)
{
   SharedCSPNode aux = std::make_shared<CSPNode>(*node);
   aux->setIndex(++idx_);
   aux->setDepth(1+node->depth());
   aux->setParent(node->index());
   return aux;
}

void CSPSplit::splitOne(SharedCSPNode& node, Variable v)
{
   Domain* dom = node->box()->get(v);
   DomainSlicer* slicer = slicerMap_->getSlicer(dom->type());

   size_t n = slicer->apply(dom);
   if (n < 2) return;

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

DomainSlicerMap* CSPSplit::getSlicerMap() const
{
   return slicerMap_;
}

/*----------------------------------------------------------------------------*/

CSPSplitRR::CSPSplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : CSPSplit(scop, std::move(smap))
{}

void CSPSplitRR::applyImpl(SharedCSPNode& node, CSPContext& context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node, context);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Round-Robin selects " << v.getName()
                                    << " in node " << node->index());

   if (getNbNodes() < 2) return;

   // assigns the split variable in the sub-nodes
   std::shared_ptr<CSPNodeInfoVar> info = std::make_shared<CSPNodeInfoVar>(v);

   for (SharedCSPNode& aux : cont_)
      context.insert(aux->index(), info);
}

std::pair<bool, Variable> CSPSplitRR::selectVar(SharedCSPNode& node,
                                                 CSPContext& context)
{
   DomainBox* box = node->box();

   std::shared_ptr<CSPNodeInfo>
      info = context.getInfo(node->index(), CSPNodeInfoType::SplitVar);

   // assigns an iterator pointing to the next variable
   Scope::const_iterator it;
   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      CSPNodeInfoVar* infovar = static_cast<CSPNodeInfoVar*>(info.get());
      it = scop_.find(infovar->getVar());
      ++it;
      if (it == scop_.end()) it = scop_.begin();
   }

   bool found = false;
   size_t nb = 0;

   while (!found && nb<scop_.size())
   {
      if (box->isSplitable(*it))
      {
         found = true;
      }
      else
      {
         ++nb;
         ++it;
         if (it == scop_.end()) it = scop_.begin();
      }
   }

   return std::make_pair(found, *it);
}

/*----------------------------------------------------------------------------*/

double domainSize(const Variable& v, Domain* dom)
{
   double d;

   if (v.isReal())
   {
      Tolerance tol = v.getTolerance();
      d = tol.discreteSize(dom->intervalHull());
   }
   else
      d = dom->size();

   return d;
}

CSPSplitLF::CSPSplitLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : CSPSplit(scop, std::move(smap))
{}

void CSPSplitLF::applyImpl(SharedCSPNode& node, CSPContext& context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Largest-First selects " << v.getName()
                                      << " in node " << node->index());
}

std::pair<bool, Variable> CSPSplitLF::selectVar(SharedCSPNode& node)
{
   return selectVar(scop_, *node->box());
}

std::pair<bool, Variable> CSPSplitLF::selectVar(const Scope& scop,
                                                 const DomainBox& box)
{
   Variable vres;
   double dres;
   bool found = false;

   for (const auto& v : scop)
   {
      if (box.isSplitable(v))
      {
         double d = domainSize(v, box.get(v));

         if ((!found) || (d > dres))
         {
            vres = v;
            dres = d;
            found = true;
         }
      }
   }

   return std::make_pair(found, vres);
}

/*----------------------------------------------------------------------------*/

CSPSplitSF::CSPSplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : CSPSplit(scop, std::move(smap))
{}

void CSPSplitSF::applyImpl(SharedCSPNode& node, CSPContext& context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Smallest-First selects " << v.getName()
                                       << " in node " << node->index());
}

std::pair<bool, Variable> CSPSplitSF::selectVar(SharedCSPNode& node)
{
   DomainBox* box = node->box();
   Variable vres;
   double dres;
   bool found = false;

   for (const auto& v : scop_)
   {
      if (box->isSplitable(v))
      {
         double d = domainSize(v, box->get(v));

         if ((!found) || (d < dres))
         {
            vres = v;
            dres = d;
            found = true;
         }
      }
   }

   return std::make_pair(found, vres);
}

/*----------------------------------------------------------------------------*/

CSPSplitSLF::CSPSplitSLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : CSPSplit(scop, std::move(smap))
{}

void CSPSplitSLF::applyImpl(SharedCSPNode& node, CSPContext& context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Smallest-Largest-First selects " << v.getName()
                                               << " in node " << node->index());
}

std::pair<bool, Variable> CSPSplitSLF::selectVar(SharedCSPNode& node)
{
   DomainBox* box = node->box();
   Variable ivmin, rvmax;
   double idmin, rdmax;
   bool ifound, rfound;

   ifound = rfound = false;

   for (const auto& v : scop_)
   {
      if (box->isSplitable(v))
      {
         double d = domainSize(v, box->get(v));

         if (v.isReal())
         {
            if ((!rfound) || (d > rdmax))
            {
               rvmax = v;
               rdmax = d;
               rfound = true;

            }
         }
         else
         {
            if ((!ifound) || (d < idmin))
            {
               ivmin = v;
               idmin = d;
               ifound = true;
            }
         }
      }
   }

   return (ifound) ? std::make_pair(ifound, ivmin) :
                     std::make_pair(rfound, rvmax);
}

/*----------------------------------------------------------------------------*/

CSPSplitSSR::CSPSplitSSR(std::shared_ptr<IntervalSmearSumRel> ssr,
                           std::unique_ptr<DomainSlicerMap> smap)
      : CSPSplit(ssr->scope(), std::move(smap)),
        ssr_(ssr)
{
   ASSERT(ssr != nullptr,
          "No interval smear sum rel object in a splitting object");
}

void CSPSplitSSR::applyImpl(SharedCSPNode& node, CSPContext& context)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Smear-Sum-Rel selects " << v.getName()
                                      << " in node " << node->index());
}

std::pair<bool, Variable> CSPSplitSSR::selectVar(SharedCSPNode& node)
{
   IntervalBox B(*node->box());
   ssr_->calculate(B);

   Variable v = ssr_->getMaxVar();
   if (node->box()->isSplitable(v))
      return std::make_pair(true, v);

   for (size_t i=0; i<ssr_->nbVars(); ++i)
   {
      v = ssr_->getVar(i);
      if (node->box()->isSplitable(v))
         return std::make_pair(true, v);
   }

   return std::make_pair(false, v);
}

} // namespace
