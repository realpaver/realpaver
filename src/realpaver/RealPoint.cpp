/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   RealPoint.cpp
 * @brief  Classes of real points
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/RealPoint.hpp"
#include "realpaver/AssertDebug.hpp"

namespace realpaver {

RealPoint::RealPoint(Scope scop, double a)
    : RealVector(scop.size(), a)
    , scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create a real point");
}

RealPoint::RealPoint(Scope scop, const RealVector &X)
    : RealVector(X)
    , scop_(scop)
{
   ASSERT(!scop.isEmpty(), "Empty scope used to create a real point");
   ASSERT(scop.size() == X.size(), "Bad initialization of a real point");
}

RealPoint::RealPoint(const RealPoint &pt, Scope scop)
    : RealVector(scop.size())
    , scop_(scop)
{
   ASSERT(pt.scope().contains(scop), "Bad scope used to project a point in a sub-space");

   for (const auto &v : scop)
      set(v, pt.get(v));
}

Scope RealPoint::scope() const
{
   return scop_;
}

double RealPoint::get(const Variable &v) const
{
   ASSERT(scop_.contains(v), "Bad access in a real point @ " << v.getName());

   return operator[](scop_.index(v));
}

void RealPoint::set(const Variable &v, double a)
{
   ASSERT(scop_.contains(v), "Bad access in a real point @ " << v.getName());

   operator[](scop_.index(v)) = a;
}

void RealPoint::setOnScope(const RealPoint &pt, const Scope &scop)
{
   for (const auto &v : scop)
      set(v, pt.get(v));
}

RealPoint RealPoint::subPoint(const Scope &scop) const
{
   ASSERT(scop_.contains(scop), "Bad scope used to create a sub-point " << scop);

   RealPoint pt(scop);
   for (const auto &v : scop)
      pt.set(v, get(v));
   return pt;
}

RealPoint *RealPoint::clone() const
{
   return new RealPoint(*this);
}

bool RealPoint::isVectorizable() const
{
   return scop_.isIdentity();
}

} // namespace realpaver
