///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_INUM64_HPP
#define REALPAVER_INTERVAL_INUM64_HPP

#include <iostream>
#include "inum64/inum64.hpp"
#include "realpaver/IntervalTraits.hpp"

#if defined(PACKAGE_BUGREPORT)
#   undef PACKAGE_BUGREPORT
#endif

#if defined(PACKAGE_NAME)
#   undef PACKAGE_NAME
#endif

#if defined(PACKAGE_STRING)
#   undef PACKAGE_STRING
#endif

#if defined(PACKAGE_TARNAME)
#   undef PACKAGE_TARNAME
#endif

#if defined(PACKAGE_VERSION)
#   undef PACKAGE_VERSION
#endif

#if defined(VERSION)
#   undef VERSION
#endif

namespace realpaver {

/// Raw interval type
typedef inum64 RawInterval;

///////////////////////////////////////////////////////////////////////////////
/// This is a specialization of the interval traits for inum64.
///
/// The operations are either delegated to inum64 if they are available in this
/// library or implemented otherwise.
///////////////////////////////////////////////////////////////////////////////
template <>
struct IntervalTraits<RawInterval> {
   typedef RawInterval interval;

   static inline interval create()
   {
      return interval();
   }

   static inline interval create(double a)
   {
      return interval(a);
   }

   static inline interval create(double l, double r)
   {
      return interval(l,r);
   }

   static inline interval create(const char* s)
   {
      try {
         interval res(s,s);
         return res;
      }
      catch(std::exception& e)
      {}
      return interval::emptyset();
   }

   static inline interval create(const char* sl, char const* sr)
   {
      try {
         interval res(sl, sr);
         return res;
      }
      catch(std::exception& e)
      {}
      return interval::emptyset();
   }

   static inline void setEmpty(interval& x)
   {
      x = interval::emptyset();
   }

   static inline double left(const interval& x)
   {
      return x.left();
   }

   static inline double right(const interval& x)
   {
      return x.right();
   }

   static inline size_t hashCode(const interval& x)
   {
      std::hash<double> f;      
      return f(x.left()) ^ (f(x.right()) << 1);
   }

   static inline interval universe()
   {
      static interval x = interval::universe();
      return x;
   }

   static inline interval positive()
   {
      static interval x = interval::half_positive();
      return x;
   }

   static inline interval negative()
   {
      static interval x = interval::half_negative();
      return x;
   }

   static inline interval emptyset()
   {
      static interval x = interval::emptyset();
      return x;
   }

   static inline interval pi()
   {
      static interval x = interval::pi();
      return x;
   }

   static inline interval halfPi()
   {
      static interval x = interval::half_pi();
      return x;
   }

   static inline interval twoPi()
   {
      static interval x = interval::two_pi();
      return x;
   }

   static inline interval zero()
   {
      static interval x = interval(0.0);
      return x;
   }

   static inline interval one()
   {
      static interval x = interval(1.0);
      return x;
   }

   static inline interval minusOne()
   {
      static interval x = -one();
      return x;
   }

   static inline interval minusOneZero()
   {
      static interval x = (-one()) | zero();
      return x;
   }

   static inline interval zeroPlusOne()
   {
      static interval x = zero() | one();
      return x;
   }

   static inline interval minusOnePlusOne()
   {
      static interval x = (-one()) | one();
      return x;
   }

   static inline interval minusPiPlusPi()
   {
      static interval x = -pi() | pi();
      return x;
   }

   static inline interval zeroPi()
   {
      static interval x = zero() | pi();
      return x;
   }

   static inline interval zeroTwoPi()
   {
      static interval x = zero() | inum64::two_pi();
      return x;
   }

   static inline double infinity()
   {
      static double a = universe().right();
      return a;
   }

   static inline double width(const interval& x)
   {
      return x.wid();
   }

   static inline double radius(const interval& x)
   {
      return x.rad();
   }

   static inline double relWidth(const interval& x)
   {
      return x.rel_wid();
   }

   static inline double midpoint(const interval& x)
   {
      return x.mid();
   }

   static inline double mig(const interval& x)
   {
      return x.mig();
   }

   static inline double mag(const interval& x)
   {
      return x.mag();
   }

   static inline bool isEmpty(const interval& x)
   {
      return x.is_empty();
   }

   static inline bool isCanonical(const interval& x)
   {
      return x.is_canonical();
   }

   static inline bool isFinite(const interval& x)
   {
      return x.is_bounded();
   }

   static inline bool isInfLeft(const interval& x)
   {
      return x.left() == interval::universe().left();
   }

   static inline bool isInfRight(const interval& x)
   {
      return x.right() == interval::universe().right();
   }

   static inline bool isSingleton(const interval& x)
   {
      return x.is_a_point();
   }

   static inline bool isZero(const interval& x)
   {
      return x.is_zero();
   }

   static inline bool isAnInt(const interval& x)
   {
      return round(x) == x;
   }

   static inline bool contains(const interval& x, double a)
   {
      return x.contains(a);      
   }

   static inline bool strictlyContains(const interval& x, double a)
   {
      return x.strictly_contains(a);
   }

   static inline bool containsZero(const interval& x)
   {
      return x.contains_zero();
   }

   static inline bool strictlyContainsZero(const interval& x)
   {
      return x.strictly_contains(0.0);
   }

   static inline bool contains(const interval& x, const interval& y)
   {
      return x.contains(y);
   }

   static inline bool strictlyContains(const interval& x,
                                       const interval& y)
   {
      return x.strictly_contains(y);
   }

   static inline bool isSetEq(const interval& x, const interval& y)
   {
      return x == y;
   }

   static inline bool isSetNeq(const interval& x, const interval& y)
   {
      return x != y;      
   }

   static inline bool isNegative(const interval& x)
   {
      return x.right() <= 0.0;
   }

   static inline bool isStrictlyNegative(const interval& x)
   {
      return x.right() < 0.0;
   }

   static inline bool isPositive(const interval& x)
   {
      return x.left() >= 0.0;
   }

   static inline bool isStrictlyPositive(const interval& x)
   {
      return x.left() > 0.0;
   }

   static inline bool isPossiblyEq(const interval& x, const interval& y)
   {
      return x.is_possibly_eq(y);
   }

   static inline bool isPossiblyNeq(const interval& x, const interval& y)
   {
      return x.is_possibly_neq(y);      
   }

   static inline bool isPossiblyLe(const interval& x, const interval& y)
   {
      return x.is_possibly_le(y);
   }

   static inline bool isPossiblyLt(const interval& x, const interval& y)
   {
      return x.is_possibly_lt(y);      
   }

   static inline bool isPossiblyGe(const interval& x, const interval& y)
   {
      return x.is_possibly_ge(y);
   }

   static inline bool isPossiblyGt(const interval& x, const interval& y)
   {
      return x.is_possibly_gt(y);
   }

   static inline bool isCertainlyEq(const interval& x, const interval& y)
   {
      return x.is_certainly_eq(y);
   }

   static inline bool isCertainlyNeq(const interval& x, const interval& y)
   {
      return x.is_certainly_neq(y);
   }

   static inline bool isCertainlyLe(const interval& x, const interval& y)
   {
      return x.is_certainly_le(y);
   }

   static inline bool isCertainlyLt(const interval& x, const interval& y)
   {
      return x.is_certainly_lt(y);
   }

   static inline bool isCertainlyGe(const interval& x, const interval& y)
   {
      return x.is_certainly_ge(y);
   }

   static inline bool isCertainlyGt(const interval& x, const interval& y)
   {
      return x.is_certainly_gt(y);
   }

   static inline bool isDisjoint(const interval& x, const interval& y)
   {
      return x.is_disjoint(y);
   }

   static inline bool overlaps(const interval& x, const interval& y)
   {
      return x.overlaps(y);
   }

   static inline double distance(const interval& x, const interval& y)
   {
      return  ::distance(x,y);
   }

   static inline void inter_assign(interval& x, const interval& y)
   {
      x &= y;
   }

   static inline interval inter(const interval& x, const interval& y)
   {
      return x & y;
   }

   static inline void hull_assign(interval& x, const interval& y)
   {
      x |= y;
   }

   static inline interval hull(const interval& x, const interval& y)
   {
      return x | y;
   }

   static inline interval round(const interval& x)
   {
      return ::round(x);
   }

   static inline void print(std::ostream& os, const interval& x)
   {
      if (x.is_empty())
         os << "empty";

      else if (isAnInt(x))
      {
         int n = (int)x.left();
         os << n;
      }

      else os << x;
   } 

   static inline std::streamsize precision(std::streamsize n)
   {
      return INUM64_REAL_PREC;
   }

   static inline std::streamsize precision()
   {
      return INUM64_REAL_PREC;
   }

   static inline void addAssign(interval& x, const interval& y)
   {
      x += y;
   }

   static inline interval add(const interval& x, const interval& y)
   {
      return x + y;
   }

   static inline interval addPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return x & (z - y);
   }

   static inline interval addPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return y & (z - x);
   }

   static inline interval addPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return z & (x + y);
   }

   static inline void subAssign(interval& x, const interval& y)
   {
      x -= y;
   }

   static inline interval sub(const interval& x, const interval& y)
   {
      return x - y;
   }

   static inline interval subPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return x & (y + z);
   }

   static inline interval subPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return y & (x - z);
   }

   static inline interval subPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return z & (x - y);
   }

   static inline interval usub(const interval& x)
   {
      return -x;
   }

   static inline interval usubPX(const interval& x, const interval& y)
   {
      return x & (-y);
   }

   static inline interval usubPY(const interval& x, const interval& y)
   {
      return y & (-x);
   }

   static inline void mulAssign(interval& x, const interval& y)
   {
      x *= y;
   }

   static inline interval mul(const interval& x, const interval& y)
   {
      return x * y;
   }

   static inline interval mulPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return mul_px(x, y, z);
   }

   static inline interval mulPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return mul_py(x, y, z);;
   }

   static inline interval mulPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return mul_pz(x, y, z);;
   }

   static inline void divAssign(interval& x, const interval& y)
   {
      x /= y;
   }

   static inline interval div(const interval& x, const interval& y)
   {
      return x / y;
   }

   static inline interval divPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return mulPZ(y,z,x);
   }

   static inline interval divPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return mulPX(y,z,x);
   }

   static inline interval divPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return mulPY(y,z,x);     
   }

   static inline interval sqr(const interval& x)
   {
      return ::sqr(x);
   }

   static inline interval sqrPX(const interval& x, const interval& y)
   {
      return ::sqr_px(x,y);
   }

   static inline interval sqrPY(const interval& x, const interval& y)
   {
      return ::sqr_py(x,y);
   }

   static inline interval sqrt(const interval& x)
   {
      return ::sqrt(x);
   }

   static inline interval sqrtPX(const interval& x, const interval& y)
   {
      return ::sqrt_px(x,y);
   }

   static inline interval sqrtPY(const interval& x, const interval& y)
   {
      return ::sqrt_py(x,y);
   }

   static inline interval pow(const interval& x, int n)
   {
      return ::pow(x,n);
   }

   static inline interval powPX(const interval& x, int n,
                                const interval& y)
   {
      return ::pow_px(x,y,n);
   }

   static inline interval powPY(const interval& x, int n,
                                const interval& y)
   {
      return ::pow_py(x,y,n);
   }

   static inline interval exp(const interval& x)
   {
      return ::exp(x);
   }

   static inline interval expPX(const interval& x, const interval& y)
   {
      return ::exp_px(x,y);
   }

   static inline interval expPY(const interval& x, const interval& y)
   {
      return ::exp_py(x,y);
   }

   static inline interval log(const interval& x)
   {
      return ::log(x);
   }

   static inline interval logPX(const interval& x, const interval& y)
   {
      return ::log_px(x,y);
   }

   static inline interval logPY(const interval& x, const interval& y)
   {
      return ::log_py(x,y);
   }

   static inline interval cos(const interval& x)
   {
      return ::cos(x) & minusOnePlusOne();
   }

   static inline interval cosPX(const interval& x, const interval& y)
   {
      return ::cos_px(x,y);
   }

   static inline interval cosPY(const interval& x, const interval& y)
   {
      return ::cos_py(x,y);
   }

   static inline interval sin(const interval& x)
   {
      return ::sin(x) & minusOnePlusOne();
   }

   static inline interval sinPX(const interval& x, const interval& y)
   {
      return ::sin_px(x,y);
   }

   static inline interval sinPY(const interval& x, const interval& y)
   {
      return ::sin_py(x,y);
   }

   static inline interval tan(const interval& x)
   {
      return ::tan(x);
   }

   static inline interval tanPX(const interval& x, const interval& y)
   {
      return ::tan_px(x,y);
   }

   static inline interval tanPY(const interval& x, const interval& y)
   {
      return ::tan_py(x,y);
   }

   static inline interval abs(const interval& x)
   {
      return ::abs(x);
   }

   static inline interval absPX(const interval& x, const interval& y)
   {      
      return ::abs_px(x,y);
   }

   static inline interval absPY(const interval& x, const interval& y)
   {
      return ::abs_py(x,y);
   }

   static inline interval min(const interval& x, const interval& y)
   {
      return ::min(x,y);
   }

   static interval minPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::min_px(x,y,z);
   }

   static inline interval minPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::min_py(x,y,z);
   }

   static inline interval minPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::min_pz(x,y,z);
   }

   static inline interval max(const interval& x, const interval& y)
   {
      return ::max(x,y);
   }

   static interval maxPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::max_px(x,y,z);
   }

   static inline interval maxPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::max_py(x,y,z);
   }

   static inline interval maxPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return ::max_pz(x,y,z);
   }

   static interval sgn(const interval& x)
   {
      
      return ::sgn(x);
   }

   static interval sgnPX(const interval& x, const interval& y)
   {
      return ::sgn_px(x,y);
   }

   static inline interval sgnPY(const interval& x, const interval& y)
   {
      return ::sgn_py(x,y);
   }

};

} // namespace

#endif
