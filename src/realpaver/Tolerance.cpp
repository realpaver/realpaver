///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

Tolerance::Tolerance(double rtol, double atol)
      : rtol_(rtol),
        atol_(atol)
{
   ASSERT(rtol >= 0.0, "A relative tolerance must be positive");
   ASSERT(atol >= 0.0, "An absolute tolerance must be positive");
}

double Tolerance::getRelTol() const
{
   return rtol_;
}

double Tolerance::getAbsTol() const
{
   return atol_;
}

void Tolerance::setRelTol(double val)
{
   ASSERT(val >= 0.0, "A relative tolerance must be positive");

   rtol_ = val;
}

void Tolerance::setAbsTol(double val)
{
   ASSERT(val >= 0.0, "An absolute tolerance must be positive");

   atol_ = val;
}

bool Tolerance::isTight(const Interval& x) const
{
   if (x.isEmpty())
      return false;

   else if (x.isCanonical())
      return true;

   else
      return Double::isClose(x.left(), x.right(), rtol_, atol_);
}

bool Tolerance::isTight(const IntervalVector& X) const
{
   for (size_t i=0; i<X.size(); ++i)
      if (!isTight(X.get(i)))
         return false;

   return true;
}

bool Tolerance::isImproved(const Interval& old, const Interval& x) const
{
   if (old.isEmpty() || x.isEmpty())
      return false;

   ASSERT(old.contains(x), "Bad use of is Imporoved " << old << " " << x);

   return (1.0 - x.width() / old.width()) > rtol_;
}

/*
Interval Tolerance::maxIntervalDn(double ub) const
{
   if (Double::isInf(ub)) return Interval::universe();

   if (val_ == 0.0) return Interval(Double::prevDouble(ub), ub);

   if (isAbsolute())
   {
      Double::rndUp();
      return Interval(ub - val_, ub);
   }
   else if (ub > 1.0)
   {
      Interval v(val_), lb(ub*(1.0-v)/(1.0+v));
      return Interval(lb.right(), ub);
   }
   else if (ub <= -1.0)
   {
      if (val_ == 1.0)
      {
         return Interval(Double::lowest(), ub);
      }
      else
      {
         Interval v(val_), lb(ub*(1.0+v)/(1.0-v));
         return Interval(lb.right(), ub);
      }
   }
   else
   {
      Double::rndUp();
      double lb = ub - val_;

      if (lb >= -1.0)
      {
         return Interval(lb, ub);
      }
      else
      {
         // we have ub < 0.0 since val_ <= 1.0
         if (val_ == 1.0)
         {
            return Interval(Double::lowest(), ub);
         }
         else
         {
            Interval v(val_), lb(ub*(1.0+v)/(1.0-v));
            return Interval(lb.right(), ub);
         }
      }
   }
}

Interval Tolerance::maxIntervalUp(double lb) const
{
   return -maxIntervalDn(-lb); 
}
*/

double Tolerance::discreteSize(const Interval& x) const
{
   if (x.isEmpty())
      return 0.0;
   
   else if (x.isCanonical())
      return 1.0;
  
   else if (x.isInf())
      return Double::floor(Double::greatest());

   else
   {
      // absolute
      double a = x.width() / atol_,
             b = Double::floor(a);

      return (a == b) ? b : b+1.0;
   }
}

std::ostream& operator<<(std::ostream& os, const Tolerance& tol)
{
   os << "tol(" << tol.getRelTol() << ", "
      << tol.getAbsTol() << ")";
   return os;
}

} // namespace
