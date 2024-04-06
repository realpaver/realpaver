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
 * @file   IntervalGaol.hpp
 * @brief  Interface of the Gaol library
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_INTERVAL_GAOL_HPP
#define REALPAVER_INTERVAL_GAOL_HPP

#include <iostream>
#include <iomanip>
#include <gaol/gaol>
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
using RawInterval = gaol::interval;

/**
 * @brief Specialization of the interval traits for Gaol.
 * 
 * @see IntervalTraits
 * 
 *  The operations are either delegated to gaol if they are available in this
 *  library or implemented otherwise.
 */
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
         interval res(s);
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
      static interval x = interval::positive();
      return x;
   }

   static inline interval negative()
   {
      static interval x = interval::negative();
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
      static interval x = interval::zero();
      return x;
   }

   static inline interval one()
   {
      static interval x = interval::one();
      return x;
   }

   static inline interval minusOne()
   {
      static interval x = -interval::one();
      return x;
   }

   static inline interval minusOneZero()
   {
      static interval x = (-interval::one()) | interval::zero();
      return x;
   }

   static inline interval zeroPlusOne()
   {
      static interval x = interval::zero() | interval::one();
      return x;
   }

   static inline interval minusOnePlusOne()
   {
      static interval x = (-interval::one()) | interval::one();
      return x;
   }

   static inline interval minusPiPlusPi()
   {
      static interval x = -interval::pi() | interval::pi();
      return x;
   }

   static inline interval zeroPi()
   {
      static interval x = interval::zero() | interval::pi();
      return x;
   }

   static inline interval zeroTwoPi()
   {
      static interval x = interval::zero() | interval::two_pi();
      return x;
   }

   static inline double infinity()
   {
      static double a = interval::universe().right();
      return a;
   }

   static inline double width(const interval& x)
   {
      return x.width();
   }

   static inline double radius(const interval& x)
   {
      if (x.is_empty())
         return -1.0;

      else if (x.is_a_double())
         return 0.0;

      else
         return x.width() / 2.0;
   }

   static inline double midpoint(const interval& x)
   {
      return x.midpoint();
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
      return x.is_finite();
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
      return x.is_a_double();
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
      return x.set_contains(a);      
   }

   static inline bool strictlyContains(const interval& x, double a)
   {
      return x.set_strictly_contains(a);
   }

   static inline bool containsZero(const interval& x)
   {
      return x.straddles_zero();
   }

   static inline bool strictlyContainsZero(const interval& x)
   {
      return x.strictly_straddles_zero();
   }

   static inline bool contains(const interval& x, const interval& y)
   {
      return x.set_contains(y);
   }

   static inline bool strictlyContains(const interval& x,
                                       const interval& y)
   {
      return x.set_strictly_contains(y);
   }

   static inline bool isSetEq(const interval& x, const interval& y)
   {
      return x.set_eq(y);
   }

   static inline bool isSetNeq(const interval& x, const interval& y)
   {
      return x.set_neq(y);      
   }

   static inline bool isNegative(const interval& x)
   {
      return !x.is_empty() && x.right() <= 0.0;
   }

   static inline bool isStrictlyNegative(const interval& x)
   {
      return !x.is_empty() && x.right() < 0.0;
   }

   static inline bool isPositive(const interval& x)
   {
      return !x.is_empty() && x.left() >= 0.0;
   }

   static inline bool isStrictlyPositive(const interval& x)
   {
      return !x.is_empty() && x.left() > 0.0;
   }

   static inline bool isPossiblyEq(const interval& x, const interval& y)
   {
      return x.possibly_eq(y);
   }

   static inline bool isPossiblyNeq(const interval& x, const interval& y)
   {
      return x.possibly_neq(y);      
   }

   static inline bool isPossiblyLe(const interval& x, const interval& y)
   {
      return x.possibly_leq(y);
   }

   static inline bool isPossiblyLt(const interval& x, const interval& y)
   {
      return x.possibly_le(y);      
   }

   static inline bool isPossiblyGe(const interval& x, const interval& y)
   {
      return x.possibly_geq(y);
   }

   static inline bool isPossiblyGt(const interval& x, const interval& y)
   {
      return x.possibly_ge(y);
   }

   static inline bool isCertainlyEq(const interval& x, const interval& y)
   {
      return x.certainly_eq(y);
   }

   static inline bool isCertainlyNeq(const interval& x, const interval& y)
   {
      return x.certainly_neq(y);
   }

   static inline bool isCertainlyLe(const interval& x, const interval& y)
   {
      return x.certainly_leq(y);
   }

   static inline bool isCertainlyLt(const interval& x, const interval& y)
   {
      return x.certainly_le(y);
   }

   static inline bool isCertainlyGe(const interval& x, const interval& y)
   {
      return x.certainly_geq(y);
   }

   static inline bool isCertainlyGt(const interval& x, const interval& y)
   {
      return x.certainly_ge(y);
   }

   static inline bool isDisjoint(const interval& x, const interval& y)
   {
      return x.set_disjoint(y);
   }

   static inline bool overlaps(const interval& x, const interval& y)
   {
      return x.possibly_eq(y);
   }

   static inline double distance(const interval& x, const interval& y)
   {
      return  gaol::hausdorff(x,y);
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
      return gaol::integer(x);
   }

   static inline void print(std::ostream& os, const interval& x)
   {
      if (x.is_empty())
      {
         os << "empty";
      }
      else if (x.is_an_int())
      {
         int n = (int)x.left();
         os << n;
      }
      else
      {
         std::streamsize size = interval::precision(os.precision());
         os << x;
         interval::precision(size);
      }
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
      return gaol::div_rel(z, y, x);
   }

   static inline interval mulPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return gaol::div_rel(z, x, y);
   }

   static inline interval mulPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return z & (x * y);
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
      return mulPZ(y, z, x);
   }

   static inline interval divPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return mulPX(y, z, x);
   }

   static inline interval divPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return mulPY(y, z, x);     
   }

   static inline interval sqr(const interval& x)
   {
      return gaol::sqr(x);
   }

   static inline interval sqrPX(const interval& x, const interval& y)
   {
      return gaol::sqrt_rel(y, x);
   }

   static inline interval sqrPY(const interval& x, const interval& y)
   {
      return y & gaol::sqr(x);
   }

   static inline interval sqrt(const interval& x)
   {
      return gaol::sqrt(x);
   }

   static inline interval sqrtPX(const interval& x, const interval& y)
   {
      return x & gaol::sqr(y & interval::positive());
   }

   static inline interval sqrtPY(const interval& x, const interval& y)
   {
      return y & gaol::sqrt(x);
   }

   static inline interval pow(const interval& x, int n)
   {
      return gaol::pow(x, n);
   }

   static inline interval powPX(const interval& x, int n,
                                const interval& y)
   {
      return gaol::nth_root_rel(y, n, x);
   }

   static inline interval powPY(const interval& x, int n,
                                const interval& y)
   {
      return y & gaol::pow(x, n);
   }

   static inline interval exp(const interval& x)
   {
      return gaol::exp(x);
   }

   static inline interval expPX(const interval& x, const interval& y)
   {
      return x & gaol::log(y);
   }

   static inline interval expPY(const interval& x, const interval& y)
   {
      return y & gaol::exp(x);
   }

   static inline interval log(const interval& x)
   {
      return gaol::log(x);
   }

   static inline interval logPX(const interval& x, const interval& y)
   {
      return x & gaol::exp(y);
   }

   static inline interval logPY(const interval& x, const interval& y)
   {
      return y & gaol::log(x);
   }

   static inline interval cos(const interval& x)
   {
      return gaol::cos(x);
   }

   static inline interval cosPX(const interval& x, const interval& y)
   {
      return x & gaol::acos_rel(y, x);
   }

   static inline interval cosPY(const interval& x, const interval& y)
   {
      return y & gaol::cos(x);
   }

   static inline interval sin(const interval& x)
   {
      return gaol::sin(x);
   }

   static inline interval sinPX(const interval& x, const interval& y)
   {
      return x & gaol::asin_rel(y, x);
   }

   static inline interval sinPY(const interval& x, const interval& y)
   {
      return y & gaol::sin(x);
   }

   static inline interval tan(const interval& x)
   {
      return gaol::tan(x);
   }

   static inline interval tanPX(const interval& x, const interval& y)
   {
      return x & gaol::atan_rel(y, x);
   }

   static inline interval tanPY(const interval& x, const interval& y)
   {
      return y & gaol::tan(x);
   }

   static inline interval cosh(const interval& x)
   {
      return gaol::cosh(x);
   }

   static inline interval coshPX(const interval& x, const interval& y)
   {
      return x & gaol::acosh_rel(y, x);
   }

   static inline interval coshPY(const interval& x, const interval& y)
   {
      return y & gaol::cosh(x);
   }

   static inline interval sinh(const interval& x)
   {
      return gaol::sinh(x);
   }

   static inline interval sinhPX(const interval& x, const interval& y)
   {
      return x & gaol::asinh_rel(y, x);
   }

   static inline interval sinhPY(const interval& x, const interval& y)
   {
      return y & gaol::sinh(x);
   }

   static inline interval tanh(const interval& x)
   {
      return gaol::tanh(x);
   }

   static inline interval tanhPX(const interval& x, const interval& y)
   {
      return x & gaol::atanh_rel(y, x);
   }

   static inline interval tanhPY(const interval& x, const interval& y)
   {
      return y & gaol::tanh(x);
   }

   static inline interval abs(const interval& x)
   {
      return gaol::abs(x);
   }

   static inline interval absPX(const interval& x, const interval& y)
   {      
      return x & gaol::invabs_rel(y, x);
   }

   static inline interval absPY(const interval& x, const interval& y)
   {
      return y & gaol::abs(x);
   }

   static inline interval min(const interval& x, const interval& y)
   {
      return gaol::min(x, y);
   }

   static interval minPX(const interval& x, const interval& y,
                        const interval& z)
   {
      if (x.is_empty() || y.is_empty() || z.is_empty())
      {
         return interval::emptyset();
      }
   
      interval pz = minPZ(x, y, z);
      if (pz.is_empty())
      {
         return interval::emptyset();
      }
      else if (x.right() < y.left())
      {
         /* z = min(x, y) <=> z = x */
         return x & pz;
      }
      else if (x.left() > y.right())
      {
         /* z = min(x, y) <=> z = y, hence no contraction for x */
         return x;
      }
      else
      {
         /* the left bound of x cannot be smaller than the minimum */
         return interval(std::max(x.left(), pz.left()), x.right());
      }
   }

   static inline interval minPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return minPX(y, x, z);
   }

   static inline interval minPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return z & gaol::min(x, y);
   }

   static inline interval max(const interval& x, const interval& y)
   {
      return gaol::max(x, y);
   }

   static interval maxPX(const interval& x, const interval& y,
                         const interval& z)
   {
      if (x.is_empty() || y.is_empty() || z.is_empty())
      {
         return interval::emptyset();
      }
   
      interval pz = maxPZ(x, y, z);
      if (pz.is_empty())
      {
         return interval::emptyset();
      }
      else if (x.left() > y.right())
      {
         /* z = max(x, y) <=> z = x */
         return x & pz;
      }
      else if (x.right() < y.left())
      {
         /* z = min(x, y) <=> z = y, hence no contraction for x */
         return x;
      }
      else
      {
         /* the right bound of x cannot be greater than the maximum */
         return interval(x.left(), std::min(x.right(), pz.right()));
      }
   }

   static inline interval maxPY(const interval& x, const interval& y,
                                const interval& z)
   {
      return maxPX(y, x, z);      
   }

   static inline interval maxPZ(const interval& x, const interval& y,
                                const interval& z)
   {
      return z & gaol::max(x, y);
   }

   static interval sgn(const interval& x)
   {
      if (x.is_empty())
         return interval::emptyset();
      
      int neg = (x.left() < 0.0) ? (1 << 0) : 0;
      int pos = (x.right() > 0.0) ? (1 << 1) : 0;
      int zro = x.set_contains(0.0) ? (1 << 2) : 0;
      int s = neg | pos | zro;

      switch(s)
      {
         case 0:
         {
            return emptyset();
         }
         case 1:
         {
            return minusOne();
         }
         case 2:
         {
            return one();
         }
         case 4:
         {
            return zero();
         }
         case 5:
         {
            return minusOneZero();
         }
         case 6:
         {
            return zeroPlusOne();
         }
         case 3:
         case 7:
         {
            return minusOnePlusOne();
         }
      }
      return universe();
   }

   static interval sgnPX(const interval& x, const interval& y)
   {
      if (x.is_empty() || y.is_empty())
         return interval::emptyset();

      int neg = y.set_contains(-1.0) ? (1 << 0) : 0;
      int pos = y.set_contains(1.0)  ? (1 << 1) : 0;
      int zro = y.set_contains(0.0)  ? (1 << 2) : 0;
      int s = neg | pos | zro;

      switch(s)
      {
         case 0:     // y is not a sign
         {
            return interval::emptyset();
         }
         case 1:     // -1 in y
         {
            return (x.left() >= 0.0) ?
                      interval::emptyset() : x & negative();
         }
         case 2:     // 1 in y
         {
            return (x.right() <= 0.0) ?
                      interval::emptyset() : x & positive();
         }
         case 4:     // 0 in y
         {
            return x & zero();
         }
         case 5:     // -1, 0 in y
         {
            return x & negative();
         }
         case 6:     // 0, 1 in y
         {
            return x & positive();
         }
         case 3:
         case 7:     // -1, 0, 1 in y => no contraction
         {
            return x;
         }
      }
      return interval::universe();
   }

   static inline interval sgnPY(const interval& x, const interval& y)
   {
      return y & sgn(x);
   }

   // the role of this function is just to remove a warning due to
   // the constant NaN_val that is defined but not used.
   static inline gaol::uintdouble useless_remove_warnings()
   {
      return gaol::NaN_val;
   }
};

} // namespace

#endif
