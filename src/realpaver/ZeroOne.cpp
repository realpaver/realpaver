///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Range.hpp"
#include "realpaver/ZeroOne.hpp"

namespace realpaver {

ZeroOne::ZeroOne()
   : zro_(true), one_(true)
{}

bool ZeroOne::isEmpty() const
{
   return !(zro_ || one_);
}

bool ZeroOne::hasZero() const
{
   return zro_;
}

bool ZeroOne::hasOne() const
{
   return one_;
}

ZeroOne& ZeroOne::setZero(bool b)
{
   zro_ = b;
   return *this;
}

ZeroOne& ZeroOne::setOne(bool b)
{
   one_ = b;
   return *this;
}

Range ZeroOne::toRange() const
{
   if (hasZero())

      if (hasOne())
         return Range(0, 1);
      else
         return Range(0, 0);

   else if (hasOne())
      return Range(1, 1);

   else
      return Range::emptyset();
}
   
} // namespace
