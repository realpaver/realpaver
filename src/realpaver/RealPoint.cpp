///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/RealPoint.hpp"

namespace realpaver {

RealPoint::RealPoint(const Scope& s, double a)
      : RealVector(s.size(), a),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create a real point");   
}

RealPoint::RealPoint(const Scope& s, const RealVector& X)
      : RealVector(X),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create a real point");      
   ASSERT(s.size() == X.size(), "Bad initialization of a real point");  
}

Scope RealPoint::scope() const
{
   return scope_;
}

double RealPoint::get(Variable v) const
{
   ASSERT(scope_.contains(v), "Bad access in a real point @ " << v.getName());

   return operator[](scope_.index(v));
}

void RealPoint::set(Variable v, double a)
{
   ASSERT(scope_.contains(v), "Bad access in a real point @ " << v.getName());

   operator[](scope_.index(v)) = a;
}

void RealPoint::setOnScope(const RealPoint& pt, const Scope& s)
{
   for (auto v : s) set(v, pt.get(v));
}

RealPoint RealPoint::subPoint(const Scope& s) const
{
   ASSERT(scope_.contains(s), "Bad scope used to create a sub-point " << s);

   RealPoint pt(s);
   for (auto v : s) pt.set(v, get(v));
   return pt;
}

RealPoint* RealPoint::clone() const
{
   return new RealPoint(*this);
}

} // namespace