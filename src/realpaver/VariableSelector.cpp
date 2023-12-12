///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

VariableSelector::VariableSelector(Scope s) : scope_(s)
{
   ASSERT(s.size() > 0, "Creation of a selector with an empty scope");
}

VariableSelector::~VariableSelector()
{}

Scope VariableSelector::scope() const
{
   return scope_;
}

///////////////////////////////////////////////////////////////////////////////

/*
MaxDomSelector::MaxDomSelector(Scope s) : VariableSelector(s)
{
   ASSERT(!s.isEmpty(), "Empty scope in a selector");
}

std::pair<bool, Variable>
MaxDomSelector::selectVar(const IntervalBox& box)
{
   bool found = false;
   double wmax, w;
   Variable vmax;

   for (auto v : scope_)
   {
      const Interval& I = box.get(v);

      if (!v.getTolerance().hasTolerance(I))
      {
         w = I.width();
         if (found)
         {
            if (w > wmax)
            {
               wmax = w;
               vmax = v;
            }
         }
         else
         {
            found = true;
            wmax = w;
            vmax = v;
         }
      }
   }

   return std::make_pair(found, vmax);   
}

std::pair<bool, Variable> MaxDomSelector::selectVar(SearchNode& node)
{
   return selectVar(*node.region());
}
*/

///////////////////////////////////////////////////////////////////////////////

/*
MaxSmearSelector::MaxSmearSelector(IntervalFunctionVector F, Scope s)
      : VariableSelector(s),
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

RoundRobinSelector::RoundRobinSelector(Scope s) : VariableSelector(s)
{}

std::pair<bool, Variable> RoundRobinSelector::selectVar(SearchNode& node)
{
   IntervalBox* box = node.region();
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

      if (!v.getTolerance().hasTolerance(box->get(v)))
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

///////////////////////////////////////////////////////////////////////////////
/*
HybridDomRobinSelector::HybridDomRobinSelector(Scope s, int f)
      : VariableSelector(s),
        f_(f)
{
   ASSERT(f>=1, "Bad factor of an hybrid max/robin selector");
}

std::pair<bool, Variable>
HybridDomRobinSelector::selectVar(const IntervalBox& box)
{
   MaxDomSelector selector(scope_);
   return selector.selectVar(box);
}

std::pair<bool, Variable> HybridDomRobinSelector::selectVar(SearchNode& node)
{
   if (node.depth() % f_ == 0)
   {
      RoundRobinSelector selector(scope_);
      return selector.selectVar(node);
   }
   else
   {
      MaxDomSelector selector(scope_);
      return selector.selectVar(node);
   }
}
*/

} // namespace
