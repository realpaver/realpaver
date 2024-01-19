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

namespace realpaver {

NcspSelector::NcspSelector(Scope scop)
      : scop_(scop),
        split_(nullptr)
{
   ASSERT(scop.size() > 0, "Creation of a selector with an empty scope");
}

NcspSelector::~NcspSelector()
{}

Scope NcspSelector::scope() const
{
   return scop_;
}

NcspSplit* NcspSelector::getSplit() const
{
   return split_;
}

void NcspSelector::setSplit(NcspSplit* split)
{
   split_ = split;
}

///////////////////////////////////////////////////////////////////////////////

NcspSelectorRR::NcspSelectorRR(Scope scop)
      : NcspSelector(scop)
{}

std::pair<bool, Variable> NcspSelectorRR::selectVar(NcspNode& node)
{
   ASSERT(getSplit() != nullptr, "No splitting object in a RR selector");

   DomainBox* box = node.box();
   //Variable v = node.splitVariable(); TODO
   //auto it = scop_.begin();

   Scope::const_iterator it;

// ----
// TODO

/*
   NcspNodeInfoMap* imap = getSplit()->getInfoMap();
   int index = node.index();
   std::shared_ptr<NcspNodeInfo>
      info = imap->getInfo(index, NcspNodeInfoType::SplitVar);

   if (info == nullptr)
   {
      it = scop_.begin();
   }
   else
   {
      NcspNodeInfoVar* infovar =
         static_cast<NcspNodeInfoVar*>(info.get());

      it = scop_.find(infovar->getVar());
      ++it;
      if (it == scop_.end()) it = scop_.begin();
   }
*/
// ----

/*
   if (!v.hasNullPointer())
   {
      it = scop_.find(v);
      ++it;
      if (it == scop_.end()) it = scop_.begin();
   }
*/

   bool found = false;
   size_t nb = 0;

   while (!found && nb<scop_.size())
   {
      Variable v = *it;

      if (box->isSplitable(v))
      {
         found = true;
         //node.setSplitVariable(v);  TODO
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

double domSize(const Variable& v, Domain* dom)
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

NcspSelectorLF::NcspSelectorLF(Scope s) : NcspSelector(s)
{}

std::pair<bool, Variable> NcspSelectorLF::selectVar(NcspNode& node)
{
   DomainBox* box = node.box();
   Variable vres;
   double dres;
   bool found = false;

   for (const auto& v : scop_)
   {
      if (box->isSplitable(v))
      {
         double d = domSize(v, box->get(v));

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

NcspSelectorSF::NcspSelectorSF(Scope s) : NcspSelector(s)
{}

std::pair<bool, Variable> NcspSelectorSF::selectVar(NcspNode& node)
{
   DomainBox* box = node.box();
   Variable vres;
   double dres;
   bool found = false;

   for (const auto& v : scop_)
   {
      if (box->isSplitable(v))
      {
         double d = domSize(v, box->get(v));

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

NcspSelectorMixedSLF::NcspSelectorMixedSLF(Scope s) : NcspSelector(s)
{}

std::pair<bool, Variable> NcspSelectorMixedSLF::selectVar(NcspNode& node)
{
   DomainBox* box = node.box();
   Variable ivmin, rvmax;
   double idmin, rdmax;
   bool ifound, rfound;

   ifound = rfound = false;

   for (const auto& v : scop_)
   {
      if (box->isSplitable(v))
      {
         double d = domSize(v, box->get(v));

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

///////////////////////////////////////////////////////////////////////////////

NcspSelectorSSR::NcspSelectorSSR(IntervalFunctionVector F)
      : NcspSelector(F.scope()),
        F_(F),
        ssr_(F.nbVars())
{}

void NcspSelectorSSR::calculateSSR(const IntervalBox& B)
{
   IntervalMatrix jac(F_.nbFuns(), F_.nbVars());
   RealMatrix S(F_.nbFuns(), F_.nbVars(), 0.0);

   // calculates the partial derivatives
   F_.diff(B, jac);

   // calculates the relative smear values
   for (size_t i=0; i<F_.nbFuns(); ++i)
   {
      double sum = 0.0;
      for (size_t j=0; j<F_.nbVars(); ++j)
      {
         const auto& v = scop_.var(j);
         double smear = jac.get(i, j).mag() * B.get(v).width();
         S.set(i, j, smear);
         sum += smear;
      }
      if (sum != 0.0)
      {
         for (size_t j=0; j<F_.nbVars(); ++j)
            S.set(i, j, S.get(i, j) / sum);
      }
   }

   // calculates the smearRelSum values
   for (size_t j=0; j<F_.nbVars(); ++j)
   {
      ssr_[j] = 0.0;
      for (size_t i=0; i<F_.nbFuns(); ++i)
         ssr_[j] += S.get(i, j);
   }
}

std::pair<bool, Variable> NcspSelectorSSR::selectVar(const IntervalBox& B)
{
   // calculates the smearSumRel values
   calculateSSR(B);

   // selects a variable
   bool found = false;
   double smax;
   Variable vmax;

   for (const auto& v : scop_)
   {
      if (B.isSplitable(v))
      {
         double s = getSSR(v);

         if ((!found) || (s > smax))
         {
            vmax = v;
            smax = s;
            found = true;
         }
      }
   }

   return std::make_pair(found, vmax);
}

std::pair<bool, Variable> NcspSelectorSSR::selectVar(NcspNode& node)
{
   IntervalBox B(*node.box());
   return selectVar(B);
}

double NcspSelectorSSR::getSSR(const Variable& v) const
{
   return ssr_[scop_.index(v)];
}

} // namespace
