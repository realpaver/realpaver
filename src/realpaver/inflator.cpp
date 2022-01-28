// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/inflator.hpp"
#include "realpaver/param.hpp"

namespace realpaver {

Inflator::Inflator()
{
   setDelta(Param::DefInflatorDelta());
   setChi(Param::DefInflatorChi());
}

Inflator::Inflator(const double& delta, const double& chi)
{
   setDelta(delta);
   setChi(chi);
}

Interval Inflator::inflate(const Interval& x) const
{
   return x.inflate(delta_, chi_);
}

Box Inflator::inflate(const Box& B) const
{
   Box aux(B.size());

   for (size_t i=0; i<aux.size(); ++i)
      aux.set(i, B[i].inflate(delta_, chi_));

   return aux;
}

} // namespace
