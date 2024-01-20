///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/NcspSelector.hpp"
#include "realpaver/NcspSplit.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

NcspSplit::NcspSplit(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : scop_(scop),
        slicerMap_(smap.release()),
        infoMap_(nullptr),
        cont_(),
        nbs_(0),
        idx_(0)
{
   ASSERT(!scop.isEmpty(), "Creation of a split object with an empty scope");
   ASSERT(slicerMap_ != nullptr, "No domain slicer map in a split object");

   infoMap_ = new NcspNodeInfoMap();
}

NcspSplit::~NcspSplit()
{
   delete slicerMap_;
   delete infoMap_;
}

Scope NcspSplit::scope() const
{
   return scop_;
}

void NcspSplit::apply(SharedNcspNode& node)
{
   LOG_INTER("Split node " << node->index() << ": " << (*node->box()));

   cont_.clear();
   ++nbs_;
   applyImpl(node);

   LOG_INTER("  -> " << getNbNodes() << " sub-node(s)");
}

size_t NcspSplit::getNbNodes() const
{
   return cont_.size();
}

size_t NcspSplit::getNbSplits() const
{
   return nbs_;
}

void NcspSplit::removeInfo(int index)
{
   infoMap_->remove(index);
}

SharedNcspNode NcspSplit::cloneNode(const SharedNcspNode& node)
{
   SharedNcspNode aux = std::make_shared<NcspNode>(*node);
   aux->setIndex(++idx_);
   aux->setDepth(1+node->depth());
   return aux;
}

void NcspSplit::splitOne(SharedNcspNode& node, Variable v)
{
   DomainBox* B = node->box();
   DomainSlicer* slicer = slicerMap_->getSlicer(v);

   size_t n = slicer->apply(B->get(v));
   if (n < 2) return;

   auto it = slicer->begin();
   while (it != slicer->end())
   {
      std::unique_ptr<Domain> slice = slicer->next(it);
      SharedNcspNode aux = cloneNode(node);
      aux->box()->set(v, std::move(slice));
      cont_.push_back(aux);
   }
}

void NcspSplit::reset()
{
   nbs_ = idx_ = 0;
}

NcspSplit::iterator NcspSplit::begin()
{
   return cont_.begin();
}

NcspSplit::iterator NcspSplit::end()
{
   return cont_.end();
}

///////////////////////////////////////////////////////////////////////////////

NcspSplitRR::NcspSplitRR(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : NcspSplit(scop, std::move(smap))
{}

void NcspSplitRR::applyImpl(SharedNcspNode& node)
{
   // variable selection
   std::pair<bool, Variable> res = selectVar(node);
   if (!res.first) return;
   Variable v = res.second;

   // splits the variable domain
   splitOne(node, v);

   LOG_INTER("Round-Robin selects " << v.getName()
                                    << " in node " << node->index());

   if (getNbNodes() < 2) return;

   // assigns the split variable in the sub-nodes
   std::shared_ptr<NcspNodeInfoVar> p = std::make_shared<NcspNodeInfoVar>(v);

   for (SharedNcspNode& aux : cont_)
      infoMap_->insert(aux->index(), p);
}

std::pair<bool, Variable> NcspSplitRR::selectVar(SharedNcspNode& node)
{
   DomainBox* box = node->box();

   std::shared_ptr<NcspNodeInfo>
      info = infoMap_->getInfo(node->index(), NcspNodeInfoType::SplitVar);

   // assigns an iterator pointing to the next variable
   Scope::const_iterator it;
   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      NcspNodeInfoVar* infovar = static_cast<NcspNodeInfoVar*>(info.get());
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

///////////////////////////////////////////////////////////////////////////////

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

NcspSplitLF::NcspSplitLF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : NcspSplit(scop, std::move(smap))
{}

void NcspSplitLF::applyImpl(SharedNcspNode& node)
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

std::pair<bool, Variable> NcspSplitLF::selectVar(SharedNcspNode& node)
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

///////////////////////////////////////////////////////////////////////////////

NcspSplitSF::NcspSplitSF(Scope scop, std::unique_ptr<DomainSlicerMap> smap)
      : NcspSplit(scop, std::move(smap))
{}

void NcspSplitSF::applyImpl(SharedNcspNode& node)
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

std::pair<bool, Variable> NcspSplitSF::selectVar(SharedNcspNode& node)
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

///////////////////////////////////////////////////////////////////////////////

NcspSplitSLF::NcspSplitSLF(Scope scop,
                                     std::unique_ptr<DomainSlicerMap> smap)
      : NcspSplit(scop, std::move(smap))
{}

void NcspSplitSLF::applyImpl(SharedNcspNode& node)
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

std::pair<bool, Variable> NcspSplitSLF::selectVar(SharedNcspNode& node)
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

} // namespace
