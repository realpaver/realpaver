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
 * @file   Inlator.cpp
 * @brief  Inflator of intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Inflator.hpp"

namespace realpaver {

Inflator::Inflator() : delta_(1.125), chi_(1.0e-12)
{}

Inflator::Inflator(double delta, double chi)
{
   setDelta(delta);
   setChi(chi);
}

Interval Inflator::inflate(const Interval& x) const
{
   return x.inflate(delta_, chi_);
}

void Inflator::inflate(IntervalVector& X) const
{
   for (size_t i=0; i<X.size(); ++i)
      X.set(i, X[i].inflate(delta_, chi_));
}

double Inflator::getDelta() const
{
   return delta_;
}

double Inflator::getChi() const
{
   return chi_;
}

void Inflator::setDelta(double delta)
{
   ASSERT(delta > 1.0, "bad inflation factor delta " << delta);

   delta_ = delta;
}

void Inflator::setChi(double chi)
{
   ASSERT(chi > 0.0, "bad inflation factor chi " << chi);

   chi_ = chi;
}

} // namespace
