///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

void RangePeeler::applyImpl(const Range& x)
{
   Integer a = x.left()+1,
           b = x.right()-1;

   push(Range(x.left(),x.left()));
   push(Range(x.right(),x.right()));

   if (b >= a)
      push(Range(a,b));
}

///////////////////////////////////////////////////////////////////////////////

void RangeLeftFixer::applyImpl(const Range& x)
{
   push(Range(x.left(),x.left()));
   push(Range(x.left()+1,x.right()));
}

///////////////////////////////////////////////////////////////////////////////

void RangeRightFixer::applyImpl(const Range& x)
{
   push(Range(x.left(),x.right()-1));
   push(Range(x.right(),x.right()));
}

///////////////////////////////////////////////////////////////////////////////

void RangeSprayer::applyImpl(const Range& x)
{
   for (auto a = x.left(); a <= x.right(); a += 1)
      push(Range(a));
}

} // namespace
