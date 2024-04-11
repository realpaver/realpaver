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
 * @file   ZeroOne.cpp
 * @brief  Subset of {0, 1}
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/Range.hpp"
#include "realpaver/ZeroOne.hpp"

namespace realpaver {

ZeroOne::ZeroOne()
      : zro_(true), one_(true)
{}

ZeroOne::ZeroOne(bool zro, bool one)
      : zro_(zro), one_(one)
{}

ZeroOne ZeroOne::universe()
{
   static ZeroOne zo(true, true);
   return zo;
}

ZeroOne ZeroOne::zero()
{
   static ZeroOne zo(true, false);
   return zo;   
}

ZeroOne ZeroOne::one()
{
   static ZeroOne zo(false, true);
   return zo;
}

ZeroOne ZeroOne::emptyset()
{
   static ZeroOne zo(false, false);
   return zo;
}

bool ZeroOne::isEmpty() const
{
   return !(zro_ || one_);
}

bool ZeroOne::isZero() const
{
   return zro_ && (!one_);
}

bool ZeroOne::isOne() const
{
   return (!zro_) && one_;
}

bool ZeroOne::isUniverse() const
{
   return zro_ && one_;
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
 
std::ostream& operator<<(std::ostream& os, const ZeroOne& zo)
{
   if (zo.hasZero())

      if (zo.hasOne())
         os << "{0, 1}";
      else
         os << "0";

   else if (zo.hasOne())
      os << "1";

   else
      os << "empty";
   
   return os;
}
 
} // namespace
