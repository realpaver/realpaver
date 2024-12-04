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
 * @file   Tolerance.cpp
 * @brief  Tolerances for numerical computations
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

Tolerance::Tolerance(double rtol, double atol)
      : rtol_(rtol),
        atol_(atol)
{
   ASSERT(rtol >= 0.0 && rtol <= 1.0, "A relative tolerance must be in [0, 1]");
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

   return (1.0 - x.width() / old.width()) > rtol_;
}

double maxIntervalDnAbs(double ub, double atol)
{
   Double::rndUp();
   return ub - atol;
}

double maxIntervalDnRel(double ub, double rtol)
{
   if (ub > 1.0)
   {
      Interval v(rtol), lb(ub*(1.0-v)/(1.0+v));
      return lb.right();
   }
   else if (ub <= -1.0)
   {
      if (rtol == 1.0)
      {
         return Double::lowest();
      }
      else
      {
         Interval v(rtol), lb(ub*(1.0+v)/(1.0-v));
         return lb.right();
      }
   }
   else
   {
      Double::rndUp();
      double lb = ub - rtol;

      if (lb >= -1.0)
      {
         return lb;
      }
      else
      {
         // we have ub < 0.0 since rtol <= 1.0
         if (rtol == 1.0)
         {
            return Double::lowest();
         }
         else
         {
            Interval v(rtol), lb(ub*(1.0+v)/(1.0-v));
            return lb.right();
         }
      }
   }
}

Interval Tolerance::maxIntervalDn(double ub) const
{
   if (Double::isNan(ub))
      return Interval::emptyset();

   if (Double::isInf(ub))
      return Interval::universe();

   else if (atol_ == 0.0 && rtol_ == 0.0)
      return Interval(Double::prevDouble(ub), ub);

   else if (rtol_ == 0.0)
   {
      double lb = maxIntervalDnAbs(ub, atol_);
      return Interval(lb, ub);
   }

   else if (atol_ == 0.0)
   {
      double lb = maxIntervalDnRel(ub, rtol_);
      return Interval(lb, ub);
   }

   else
   {
      double lb1 = maxIntervalDnAbs(ub, atol_),
             lb2 = maxIntervalDnRel(ub, rtol_);
      return Interval(Double::min(lb1, lb2), ub);
   }
}

Interval Tolerance::maxIntervalUp(double lb) const
{
   return -maxIntervalDn(-lb); 
}

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
