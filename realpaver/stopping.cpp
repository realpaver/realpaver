// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/stopping.hpp"

namespace realpaver {

Improvement::Improvement(const double& f)
{
   setVal(f);
}

void Improvement::setVal(const double& f)
{
   ASSERT(f >= 0.0 && f <= 100.0,
          "an improvement factor must belong to [0,100]");

   f_ = f;
}

bool Improvement::test(const Interval& x, const Interval& old) const
{
   if (x.isEmpty() || old.isEmpty() || old.isSetEq(x) || (!old.contains(x)))
      return false;

   if (old.isInfLeft() && (!x.isInfLeft()))
      return true;

   if (old.isInfRight() && (!x.isInfRight()))
      return true;

   double wx = x.width(),
          wo = old.width();

   if (f_ == 100.0)
      return wx < wo;

   else
   {
      Interval y( 100.0 * Interval(wx) ),
               z( f_ * Interval(wo) );

      return y.isCertainlyLt(z);
   }
}

Improvement Improvement::calculate(const Interval& x, const Interval& old)
{
   if (x.isEmpty() || old.isEmpty() || old.isSetEq(x) || (!old.contains(x)))
      return Improvement(0.0);

   if (old.isInfLeft() && (!x.isInfLeft()))
      return Improvement(100.0);

   if (old.isInfRight() && (!x.isInfRight()))
      return Improvement(100.0);

   double wx = x.width(),
          wo = old.width();

   return Improvement(100.0*(1.0 - wx/wo));
}

std::ostream& operator<<(std::ostream& os, const Improvement& imp)
{
   return os << imp.val();
}

///////////////////////////////////////////////////////////////////////////////
Precision::Precision(const double& val, bool absolute) :
   val_(val), abs_(absolute)
{
   ASSERT(val >= 0.0 && (val <= 1.0 || absolute), "bad precision: " << val);
}

bool Precision::test(const Interval& x) const
{
   if (x.isEmpty())
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

double Precision::precisionOf(const Interval& x)
{
   if (x.isEmpty())
      return -1.0;

   else if (x.isCanonical())
      return 0.0;
 
   else if (Interval::minusOnePlusOne().contains(x))
      return x.width();

   else
      return x.relWidth();
}

std::ostream& operator<<(std::ostream& os, const Precision& p)
{
   os << p.val();

   if (p.isAbsolute())
      os << " abs";
   else
      os << " ada";

   return os;
}

} // namespace
