///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Selector.hpp"

namespace realpaver {

Selector::Selector(Scope s) : scope_(s)
{
   ASSERT(s.size() > 0, "Creation of a selector with an empty scope");
}

Selector::~Selector()
{}

Scope Selector::scope() const
{
   return scope_;
}

std::pair<bool, Variable> Selector::selectVar(const SearchNode& node)
{
   return selectVar(*node.region());
}

std::pair<bool, Variable> Selector::selectVar(const IntervalRegion& reg)
{
   return std::make_pair(false, Variable());
}

///////////////////////////////////////////////////////////////////////////////

SelectorMaxDom::SelectorMaxDom(Scope s) : Selector(s)
{
   ASSERT(!s.isEmpty(), "Empty scope in a selector");
}

std::pair<bool, Variable> SelectorMaxDom::selectVar(const IntervalRegion& reg)
{
   bool found = false;
   double wmax, w;
   Variable vmax;

   for (auto v : scope_)
   {
      const Interval& I = reg.get(v);

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

///////////////////////////////////////////////////////////////////////////////

SelectorMaxSmear::SelectorMaxSmear(DiffIntervalFunction* f, Scope s)
      : Selector(s),
        f_(f)
{
   ASSERT(!s.isEmpty(), "Empty scope in a MaxSmear selector");
   ASSERT(f != nullptr, "Null function n a MaxSmear selector");
}

std::pair<bool, Variable>
SelectorMaxSmear::selectVar(const IntervalRegion& reg)
{
   Scope fscope = f_->funScope();

   IntervalVector grad(fscope.size());
   f_->intervalDiff(reg, grad);

   bool found = false;
   double smax, s, w;
   Variable vmax;

   for (auto v : scope_)
   {
      Interval I = reg.get(v);;

      if (!v.getTolerance().hasTolerance(I))
      {
         w = I.width();

         if (fscope.contains(v))
         {
            size_t pos = fscope.index(v);
            Interval dv = grad[pos];
            s = dv.isZero() ? w : w * dv.mag();
         }
         else
         {
            s = w;
         }
         
         if (found)
         {
            if (s > smax)
            {
               smax = s;
               vmax = v;
            }
         }
         else
         {
            found = true;
            smax = s;
            vmax = v;
         }
      }
   }

   return std::make_pair(found, vmax);
}

///////////////////////////////////////////////////////////////////////////////

SelectorRoundRobin::SelectorRoundRobin(Scope s) : Selector(s)
{}

std::pair<bool, Variable> SelectorRoundRobin::selectVar(const SearchNode& node)
{
   IntervalRegion* reg = node.region();
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

      if (!v.getTolerance().hasTolerance(reg->get(v)))
      {
         found = true;
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
