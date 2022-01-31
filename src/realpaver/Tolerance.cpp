///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/number.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

Tolerance::Tolerance(double val, bool absolute) :
   val_(val), abs_(absolute)
{
   ASSERT(val >= 0.0 && (val <= 1.0 || absolute), "bad tolerance: " << val);
}

double Tolerance::getVal() const
{
   return val_;
}

void Tolerance::setVal(double m)
{
   val_ = m;
}

bool Tolerance::isAbsolute() const
{
   return abs_;
}

bool Tolerance::isRelative() const
{
   return !abs_;
}

Tolerance Tolerance::makeAbs(double val)
{
   return Tolerance(val, true);
}

Tolerance Tolerance::makeRel(double val)
{
   return Tolerance(val, false);
}

double Tolerance::toleranceOf(const Interval& x)
{
   if (x.isEmpty())
      return -1.0;

   else if (x.isCanonical())
      return 0.0;
 
   else if (abs_ || Interval::minusOnePlusOne().contains(x))
      return x.width();

   else
      return x.relWidth();
}

bool Tolerance::hasTolerance(const Interval& x) const
{
   if (x.isEmpty() || x.isInf())
      return false;

   else if (x.isCanonical())
      return true;

   else
   {
      double px = (isAbsolute() || Interval::minusOnePlusOne().contains(x)) ?
                     x.width() : x.relWidth();

      return px <= val_;
   }
}

bool Tolerance::hasTolerance(double x, double y) const
{
   if (x == y) return true;

   return (x < y) ? hasTolerance(Interval(x, y)) :
                    hasTolerance(Interval(y, x));
}

bool Tolerance::hasTolerance(const Interval& x, const Interval& y) const
{
   if (x.isEmpty() || y.isEmpty() || x.isInf() || y.isInf())
      return false;

   double u = Double::abs(x.left() - y.left()),
          v = Double::abs(x.right() - y.right());

   return (u > v) ? hasTolerance(x.left(), y.left()) :
                    hasTolerance(x.right(), y.right());
}

std::ostream& operator<<(std::ostream& os, const Tolerance& tol)
{
   os << tol.getVal()
      << " "
      << (tol.isAbsolute() ? "(abs)" : "(rel)");
   return os;
}

} // namespace
