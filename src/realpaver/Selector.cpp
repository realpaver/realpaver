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

Selector::Selector(const Scope& s) : scope_(s)
{
   ASSERT(s.size() > 0, "Creation of a selector with an empty scope");
}

Selector::~Selector()
{}

Scope Selector::scope() const
{
   return scope_;
}

///////////////////////////////////////////////////////////////////////////////

SelectorMaxDom::SelectorMaxDom(const Scope& s) : Selector(s)
{
   ASSERT(!s.isEmpty(), "Empty scope in a selector");
}

std::pair<bool, Variable> SelectorMaxDom::selectVar(const IntervalVector& X)
{
   bool found = false;
   double wmax, w;
   Variable vmax;

   for (auto v : scope())
   {
      const Interval& I = X[v];

      if (!v.getTolerance().hasTolerance(I))
      {
         w = X[v].width();
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

SelectorMaxSmear::SelectorMaxSmear(IntervalFunction* f, const Scope& s)
      : Selector(s),
        f_(f)
{
   ASSERT(!s.isEmpty(), "Empty scope in a MaxSmear selector");
   ASSERT(f != nullptr, "Null function n a MaxSmear selector");
}

std::pair<bool, Variable> SelectorMaxSmear::selectVar(const IntervalVector& X)
{
   Scope S = scope();
   Scope fscope = f_->ifunScope();

   IntervalVector grad(fscope.size());
   f_->ifunDiff(X, grad);

   bool found = false;
   double smax, s, w;
   Variable vmax;

   for (auto v : S)
   {
      const Interval& I = X[v];

      if (!v.getTolerance().hasTolerance(I))
      {
         w = X[v].width();

         if (fscope.contains(v))
         {
            size_t pos = fscope.index(v);
            const Interval& G = grad[pos];
            s = G.isZero() ? w : w * G.mag();
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

} // namespace
