///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RealPoint.hpp"

namespace realpaver {

RealPoint::RealPoint(Scope s, double a)
      : RealVector(s.size(), a),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create a real point");   
}

RealPoint::RealPoint(Scope s, const RealVector& X)
      : RealVector(X),
        scope_(s)
{
   ASSERT(!s.isEmpty(), "Empty scope used to create a real point");      
   ASSERT(s.size() == X.size(), "Bad initialization of a real point");  
}

RealPoint::RealPoint(const RealPoint& pt, Scope s)
      : RealVector(s.size()),
        scope_(s)
{
   ASSERT(pt.scope().contains(s), "Bad scope used to project a point in a sub-space");

   for (Variable v : s)
      set(v, pt.get(v));
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

void RealPoint::setOnScope(const RealPoint& pt, Scope s)
{
   for (auto v : s) set(v, pt.get(v));
}

RealPoint RealPoint::subPoint(Scope s) const
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
