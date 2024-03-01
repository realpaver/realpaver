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

#if defined(VERSION)
#undef VERSION
#endif

namespace realpaver {

/// Raw interval type
using RawInterval = inum64::interval;

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
      return interval(l, r);
   }

   static inline interval create(const char* s)
   {
      return interval(s, s);
   }

   static inline interval create(const char* sl, char const* sr)
   {
      return interval(sl, sr);
   }

   static inline void setEmpty(interval& x)
   {
      x.set_empty();
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
      return x.hash_code();
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
      static interval x = zero() | interval::two_pi();
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
      return x.is_left_unbounded();
   }

   static inline bool isInfRight(const interval& x)
   {
      return x.is_right_unbounded();
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
      return x.is_an_int();
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
      return x.strictly_contains_zero();
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
      return x.is_negative();
   }

   static inline bool isStrictlyNegative(const interval& x)
   {
      return x.is_strictly_negative();
   }

   static inline bool isPositive(const interval& x)
   {
      return x.is_positive();
   }

   static inline bool isStrictlyPositive(const interval& x)
   {
      return x.is_strictly_positive();
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
      return  inum64::distance(x, y);
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
      return inum64::round(x);
   }

   static inline void print(std::ostream& os, const interval& x)
   {
      os << x;
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
      return inum64::add_px(x, y, z);
   }

   static inline interval addPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::add_py(x, y, z);
   }

   static inline interval addPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::add_pz(x, y, z);
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
      return inum64::sub_px(x, y, z);
   }

   static inline interval subPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::sub_py(x, y, z);
   }

   static inline interval subPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::sub_pz(x, y, z);
   }

   static inline interval usub(const interval& x)
   {
      return -x;
   }

   static inline interval usubPX(const interval& x, const interval& y)
   {
      return inum64::usub_px(x, y);
   }

   static inline interval usubPY(const interval& x, const interval& y)
   {
      return inum64::usub_py(x, y);
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
      return inum64::mul_px(x, y, z);
   }

   static inline interval mulPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::mul_py(x, y, z);;
   }

   static inline interval mulPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::mul_pz(x, y, z);;
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
      return inum64::div_px(x, y, z);
   }

   static inline interval divPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::div_py(x, y, z);
   }

   static inline interval divPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::div_pz(x, y, z);     
   }

   static inline interval sqr(const interval& x)
   {
      return inum64::sqr(x);
   }

   static inline interval sqrPX(const interval& x, const interval& y)
   {
      return inum64::sqr_px(x, y);
   }

   static inline interval sqrPY(const interval& x, const interval& y)
   {
      return inum64::sqr_py(x, y);
   }

   static inline interval sqrt(const interval& x)
   {
      return inum64::sqrt(x);
   }

   static inline interval sqrtPX(const interval& x, const interval& y)
   {
      return inum64::sqrt_px(x, y);
   }

   static inline interval sqrtPY(const interval& x, const interval& y)
   {
      return inum64::sqrt_py(x, y);
   }

   static inline interval pow(const interval& x, int n)
   {
      return inum64::pow(x, n);
   }

   static inline interval powPX(const interval& x, int n,
                                const interval& y)
   {
      return inum64::pow_px(x, y, n);
   }

   static inline interval powPY(const interval& x, int n,
                                const interval& y)
   {
      return inum64::pow_py(x, y, n);
   }

   static inline interval exp(const interval& x)
   {
      return inum64::exp(x);
   }

   static inline interval expPX(const interval& x, const interval& y)
   {
      return inum64::exp_px(x, y);
   }

   static inline interval expPY(const interval& x, const interval& y)
   {
      return inum64::exp_py(x, y);
   }

   static inline interval log(const interval& x)
   {
      return inum64::log(x);
   }

   static inline interval logPX(const interval& x, const interval& y)
   {
      return inum64::log_px(x, y);
   }

   static inline interval logPY(const interval& x, const interval& y)
   {
      return inum64::log_py(x, y);
   }

   static inline interval cos(const interval& x)
   {
      return inum64::cos(x);
   }

   static inline interval cosPX(const interval& x, const interval& y)
   {
      return inum64::cos_px(x, y);
   }

   static inline interval cosPY(const interval& x, const interval& y)
   {
      return inum64::cos_py(x, y);
   }

   static inline interval sin(const interval& x)
   {
      return inum64::sin(x);
   }

   static inline interval sinPX(const interval& x, const interval& y)
   {
      return inum64::sin_px(x, y);
   }

   static inline interval sinPY(const interval& x, const interval& y)
   {
      return inum64::sin_py(x, y);
   }

   static inline interval tan(const interval& x)
   {
      return inum64::tan(x);
   }

   static inline interval tanPX(const interval& x, const interval& y)
   {
      return inum64::tan_px(x, y);
   }

   static inline interval tanPY(const interval& x, const interval& y)
   {
      return inum64::tan_py(x, y);
   }

   static inline interval cosh(const interval& x)
   {
      return inum64::cosh(x);
   }

   static inline interval coshPX(const interval& x, const interval& y)
   {
      return inum64::cosh_px(x, y);
   }

   static inline interval coshPY(const interval& x, const interval& y)
   {
      return inum64::cosh_py(x, y);
   }

   static inline interval sinh(const interval& x)
   {
      return inum64::sinh(x);
   }

   static inline interval sinhPX(const interval& x, const interval& y)
   {
      return inum64::sinh_px(x, y);
   }

   static inline interval sinhPY(const interval& x, const interval& y)
   {
      return inum64::sinh_py(x, y);
   }

   static inline interval tanh(const interval& x)
   {
      return inum64::tanh(x);
   }

   static inline interval tanhPX(const interval& x, const interval& y)
   {
      return inum64::tanh_px(x, y);
   }

   static inline interval tanhPY(const interval& x, const interval& y)
   {
      return inum64::tanh_py(x, y);
   }

   static inline interval abs(const interval& x)
   {
      return inum64::abs(x);
   }

   static inline interval absPX(const interval& x, const interval& y)
   {      
      return inum64::abs_px(x, y);
   }

   static inline interval absPY(const interval& x, const interval& y)
   {
      return inum64::abs_py(x, y);
   }

   static inline interval min(const interval& x, const interval& y)
   {
      return inum64::min(x, y);
   }

   static interval minPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::min_px(x, y, z);
   }

   static inline interval minPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::min_py(x, y, z);
   }

   static inline interval minPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::min_pz(x, y, z);
   }

   static inline interval max(const interval& x, const interval& y)
   {
      return inum64::max(x, y);
   }

   static interval maxPX(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::max_px(x, y, z);
   }

   static inline interval maxPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::max_py(x, y, z);
   }

   static inline interval maxPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return inum64::max_pz(x, y, z);
   }

   static interval sgn(const interval& x)
   {
      return inum64::sgn(x);
   }

   static interval sgnPX(const interval& x, const interval& y)
   {
      return inum64::sgn_px(x, y);
   }

   static inline interval sgnPY(const interval& x, const interval& y)
   {
      return inum64::sgn_py(x, y);
   }
};

} // namespace

#endif
