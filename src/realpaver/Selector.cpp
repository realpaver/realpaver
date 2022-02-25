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
{}

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

} // namespace
