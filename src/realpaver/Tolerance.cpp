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

Tolerance::Tolerance(double val, bool absolute) :
   val_(val), abs_(absolute)
{
   ASSERT(val >= 0.0 && (val <= 1.0 || absolute), "bad tolerance: " << val);
}

Tolerance::Tolerance(const std::string& str)
{
   if (str.size() < 2) THROW("Bad tolerance format: " + str);

   size_t k = str.size()-1;
   char c = str[k];

   if (c != 'A' && c != 'R') THROW("Bad tolerance format: " + str);

   Interval x(str.substr(0, k));
   if (x.isEmpty() || x.right() < 0.0) THROW("Bad tolerance format: " + str);

   abs_ = (c == 'A');
   val_ = x.right();
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

bool Tolerance::hasTolerance(const IntervalVector& X) const
{
   for (size_t i=0; i<X.size(); ++i)
      if (!hasTolerance(X.get(i)))
         return false;

   return true;
}

bool Tolerance::hasTolerance(double x, double y) const
{
   if (x == y) return true;

   return (x < y) ? hasTolerance(Interval(x, y)) :
                    hasTolerance(Interval(y, x));
}

bool Tolerance::haveDistTolerance(const Interval& x, const Interval& y) const
{
   if (x.isEmpty() || y.isEmpty() || x.isInf() || y.isInf())
      return false;

   double u = Double::abs(x.left() - y.left()),
          v = Double::abs(x.right() - y.right());

   return (u > v) ? hasTolerance(x.left(), y.left()) :
                    hasTolerance(x.right(), y.right());
}

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

double Tolerance::discreteSize(const Interval& x) const
{
   if (x.isEmpty())
      return 0.0;
   
   else if (x.isCanonical())
      return 1.0;
  
   else if (x.isInf())
      return Double::floor(Double::greatest());

   else if (Interval::minusOnePlusOne().contains(x))
   {
      // absolute
      double a = x.width() / val_,
             b = Double::floor(a);

      return (a == b) ? b : b+1.0;
   }

   else
   {
      // relative
      double a = x.relWidth() / val_,
             b = Double::floor(a);

      return (a == b) ? b : b+1.0;
   }
}

std::ostream& operator<<(std::ostream& os, const Tolerance& tol)
{
   os << tol.getVal()
      << (tol.isAbsolute() ? "A" : "R");
   return os;
}

} // namespace
