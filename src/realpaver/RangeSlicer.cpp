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
 * @file   RangeSlicer.cpp
 * @brief  Classes of range slicers
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/RangeSlicer.hpp"

namespace realpaver {

RangeSlicer::~RangeSlicer()
{}

size_t RangeSlicer::apply(const Range& x)
{
   ASSERT(x.nbElems() > 0, "Slicing of a degenerated range: " << x);

   cont_.clear();
   applyImpl(x);
   return cont_.size();
}

void RangeSlicer::push(const Range& x)
{
   if (!x.isEmpty())
      cont_.push_back(x);
}

void RangeSlicer::clear()
{
   cont_.clear();
}

RangeSlicer::iterator RangeSlicer::begin()
{
   return cont_.begin();
}

RangeSlicer::iterator RangeSlicer::end()
{
   return cont_.end();
}

size_t RangeSlicer::nbSlices() const
{
   return cont_.size();
}

/*----------------------------------------------------------------------------*/

void RangeBisecter::applyImpl(const Range& x)
{
   Integer m = x.midpoint();
   if (m == x.left())
   {
      push(Range(m,m));
      push(Range(m+1,x.right()));         
   }
   else if (m == x.right())
   {
      push(Range(x.left(),m-1));
      push(Range(m,m));
   }
   else
   {
      push(Range(x.left(),m));
      push(Range(m+1,x.right()));
   }
}

/*----------------------------------------------------------------------------*/

void RangePeeler::applyImpl(const Range& x)
{
   Integer a = x.left()+1,
           b = x.right()-1;

   push(Range(x.left(),x.left()));
   push(Range(x.right(),x.right()));

   if (b >= a)
      push(Range(a,b));
}

/*----------------------------------------------------------------------------*/

void RangeLeftFixer::applyImpl(const Range& x)
{
   push(Range(x.left(),x.left()));
   push(Range(x.left()+1,x.right()));
}

/*----------------------------------------------------------------------------*/

void RangeRightFixer::applyImpl(const Range& x)
{
   push(Range(x.left(),x.right()-1));
   push(Range(x.right(),x.right()));
}

/*----------------------------------------------------------------------------*/

void RangeSprayer::applyImpl(const Range& x)
{
   for (auto a = x.left(); a <= x.right(); a += 1)
      push(Range(a));
}

} // namespace
