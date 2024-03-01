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

RealPoint::RealPoint(Scope scop, double a)
      : RealVector(scop.size(), a),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create a real point");   
}

RealPoint::RealPoint(Scope scop, const RealVector& X)
      : RealVector(X),
        scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create a real point");      
   ASSERT(scop.size() == X.size(), "Bad initialization of a real point");  
}

RealPoint::RealPoint(const RealPoint& pt, Scope scop)
      : RealVector(scop.size()),
        scop_(scop)
{
   ASSERT(pt.scope().contains(scop),
          "Bad scope used to project a point in a sub-space");

   for (const auto& v : scop)
      set(v, pt.get(v));
}

Scope RealPoint::scope() const
{
   return scop_;
}

double RealPoint::get(const Variable& v) const
{
   ASSERT(scop_.contains(v), "Bad access in a real point @ " << v.getName());

   return operator[](scop_.index(v));
}

void RealPoint::set(const Variable& v, double a)
{
   ASSERT(scop_.contains(v), "Bad access in a real point @ " << v.getName());

   operator[](scop_.index(v)) = a;
}

void RealPoint::setOnScope(const RealPoint& pt, const Scope& scop)
{
   for (const auto& v : scop)
      set(v, pt.get(v));
}

RealPoint RealPoint::subPoint(const Scope& scop) const
{
   ASSERT(scop_.contains(scop),
          "Bad scope used to create a sub-point " << scop);

   RealPoint pt(scop);
   for (const auto& v : scop) pt.set(v, get(v));
   return pt;
}

RealPoint* RealPoint::clone() const
{
   return new RealPoint(*this);
}

} // namespace
