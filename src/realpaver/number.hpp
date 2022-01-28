// This file is part of realpaver. License: see COPYING file.

#ifndef REALPAVER_NUMBER_HPP
#define REALPAVER_NUMBER_HPP

#include <cfenv>
#include <cmath>
#include <limits>

namespace realpaver {

/*****************************************************************************
 * Floating-point numbers in double precision.
 *****************************************************************************/
struct Double {
   typedef double type;

   static type inf()      { return std::numeric_limits<double>::infinity(); }
   static type neginf()   { return -std::numeric_limits<double>::infinity(); }
   static type nan()      { return std::numeric_limits<double>::quiet_NaN(); }
   static type lowest()   { return -std::numeric_limits<double>::max(); }
   static type greatest() { return std::numeric_limits<double>::max(); }

   static bool isInf(double x) { return (x==inf()) || (x==neginf()); }
   static bool isNan(double x) { return x != x; }

   static void rndNear() { std::fesetround(FE_TONEAREST); }

   static double add(double x, double y) { return x+y; }
   static double sub(double x, double y) { return x-y; }
   static double mul(double x, double y) { return x*y; }
   static double div(double x, double y) { return x/y; }
   static double min(double x, double y) { return (x<y) ? x : y; }
   static double max(double x, double y) { return (x>y) ? x : y; }
   static double pow(double x, int n)    { return std::pow(x, n); }

   static double abs(double x)  { return (x<0.0) ? (-x) : x; }
   static double usb(double x)  { return -x; }
   static double sqr(double x)  { return x*x; }
   static double sqrt(double x) { return std::sqrt(x); }
   static double log(double x)  { return std::log(x); }
   static double exp(double x)  { return std::exp(x); }
   static double cos(double x)  { return std::cos(x); }
   static double sin(double x)  { return std::sin(x); }
   static double tan(double x)  { return std::tan(x); }
   static double sgn(double x)
   {
      return (x == 0.0) ? 0.0 : ((x < 0.0) ? -1.0 : 1.0);
   }
};

/*****************************************************************************
 * Integers.
 *****************************************************************************/
struct Integer {
   typedef int type;

   static int min() { return std::numeric_limits<int>::min(); }
   static int max() { return std::numeric_limits<int>::max(); }
};
   
} // namespace

#endif
