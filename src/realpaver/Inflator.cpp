///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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

void Inflator::inflate(Box& B) const
{
   for (size_t i=0; i<B.size(); ++i)
      B.set(i, B[i].inflate(delta_, chi_));
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
