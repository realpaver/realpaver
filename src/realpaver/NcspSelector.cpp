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

namespace realpaver {

NcspSelector::NcspSelector(Scope s) : scope_(s)
{
   ASSERT(s.size() > 0, "Creation of a selector with an empty scope");
}

NcspSelector::~NcspSelector()
{}

Scope NcspSelector::scope() const
{
   return scope_;
}

///////////////////////////////////////////////////////////////////////////////

/*
MaxSmearSelector::MaxSmearSelector(IntervalFunctionVector F, Scope s)
      : NcspSelector(s),
        F_(F)
{
   ASSERT(!s.isEmpty(), "Empty scope in a MaxSmear selector");
}

std::pair<bool, Variable> MaxSmearSelector::selectVar(const IntervalBox& box)
{
   Scope fscope = F_.scope();
   IntervalMatrix jac(F_.nbFuns(), F_.nbVars());

   // calculates the partial derivatives
   F_.diff(box, jac);

   bool found = false;
   double maxsmear = 0.0;
   Variable maxvar;

   for (auto v : scope_)
   {
      Interval I = box.get(v);
      if (v.getTolerance().hasTolerance(I)) continue;

      size_t j = F_.scope().index(v);

      // finds the maximum magnitude of the dFi / dxj
      double mag = 0.0;
      for (size_t i=0; i<F_.nbFuns(); ++i)
      {
         double m = jac.get(i, j).mag();
         if (m > mag) mag = m;
      }

      double smear = mag*I.width();
      if (found)
      {
         if (smear > maxsmear)
         {
            maxsmear = smear;
            maxvar = v;
         }
      }
      else
      {
         maxsmear = smear;
         maxvar = v;
         found = true;
      }
   }

   return std::make_pair(found, maxvar);
}

std::pair<bool, Variable> MaxSmearSelector::selectVar(SearchNode& node)
{
   return selectVar(*node.region());
}
*/

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

   for (const auto& v : scope_)
   {
      if (box->isSplitable(v))
      {
         double d = domSize(v, box->get(v));

         if ((!found) || (d > dres))
         {
            vres = v;
            dres = d;
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

   for (const auto& v : scope_)
   {
      if (box->isSplitable(v))
      {
         double d = domSize(v, box->get(v));

         if ((!found) || (d < dres))
         {
            vres = v;
            dres = d;
         }
      }
   }

   return std::make_pair(found, vres);
}

///////////////////////////////////////////////////////////////////////////////

NcspSelectorRR::NcspSelectorRR(Scope s) : NcspSelector(s)
{}

std::pair<bool, Variable> NcspSelectorRR::selectVar(NcspNode& node)
{
   DomainBox* box = node.box();
   Variable v = node.splitVariable();
   auto it = scope_.begin();

   if (!v.hasNullPointer())
   {
      it = scope_.find(v);
      ++it;
      if (it == scope_.end()) it = scope_.begin();
   }

   bool found = false;
   size_t nb = 0;

   while (!found && nb<scope_.size())
   {
      v = *it;

      if (box->isSplitable(v))
      {
         found = true;
         node.setSplitVariable(v);
      }
      else
      {
         ++nb;
         ++it;
         if (it == scope_.end()) it = scope_.begin();
      }
   }

   return std::make_pair(found, v);
}

} // namespace
