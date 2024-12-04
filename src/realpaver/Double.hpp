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
 * @file   Double.hpp
 * @brief  Management of floating-point numbers
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DOUBLE_HPP
#define REALPAVER_DOUBLE_HPP

#include <cfenv>
#include <cmath>
#include <limits>
#include "realpaver/Common.hpp"

namespace realpaver {

/// Management of floating-point numbers in double precision
struct Double {
   /// Value type
   using ValueType = double;

   /// Reference type
   using RefType = double&;

   /// Constant reference type
   using ConstRefType = const double&;

   /// Returns +Infinity
   static inline ValueType inf()
   {
      return std::numeric_limits<ValueType>::infinity();
   }

   /// Returns -Infinity
   static inline ValueType neginf()
   {
      return -std::numeric_limits<ValueType>::infinity();
   }

   /// Returns Not a Number
   static inline ValueType nan()
   {
      return std::numeric_limits<ValueType>::quiet_NaN();
   }

   /// Returns the lowest representable value
   static inline ValueType lowest()
   {
      return -std::numeric_limits<ValueType>::max();
   }

   /// Returns the greatest representable value
   static inline ValueType greatest()
   {
      return std::numeric_limits<ValueType>::max();
   }

   /// Returns true if x is equal to -Infinity or +Infinity
   static inline bool isInf(ValueType x)
   {
      return (x==inf()) || (x==neginf());
   }

   /// Returns true if x is not a number
   static inline bool isNan(ValueType x)
   {
      return x != x;
   }

   /**
    * @brief Test of closeness of two numbers.
    * 
    * Given an absolute tolerance abs_tol and a relative tolerance rel_tol, x is
    * close to y if |x - y| <= max( rtol*max(|x|, |y|), atol).
    * 
    * Returns false if x or y is a NaN or +/-inf.
    */
   static inline bool isClose(ValueType x, ValueType y, ValueType rel_tol,
                              ValueType abs_tol)
   {
      if (isNan(x) || isNan(y) || isInf(x) || isInf(y))
         return false;

      return abs(y-x) <= max(rel_tol*max(abs(x), abs(y)), abs_tol);
   }

   /// Returns the double before x
   static inline ValueType prevDouble(ValueType x)
   {
      return std::nextafter(x, neginf());
   }

   /// Returns the double after x
   static inline ValueType nextDouble(ValueType x)
   {
      return std::nextafter(x, inf());
   }

   /// Sets the rounding downward mode of the FP unit
   static inline void rndDn()
   {
      std::fesetround(FE_DOWNWARD);
   }

   /// Sets the rounding to nearest mode of the FP unit
   static inline void rndNear()
   {
      std::fesetround(FE_TONEAREST);
   }

   /// Sets the rounding upward mode of the FP unit
   static inline void rndUp()
   {
      std::fesetround(FE_UPWARD);
   }

   /// Returns the greatest integral number not greater than x
   static inline ValueType floor(ValueType x)
   {
      return std::floor(x);
   }

   /// Returns the smallest integral number not smaller than x
   static inline ValueType ceil(ValueType x)
   {
      return std::ceil(x);
   }

   /// Addition
   static inline ValueType add(ValueType x, ValueType y)
   {
      return x+y;
   }

   /// Subtraction
   static inline ValueType sub(ValueType x, ValueType y)
   {
      return x-y;
   }

   /// Multiplication
   static inline ValueType mul(ValueType x, ValueType y)
   {
      return x*y;
   }

   /// Division
   static inline ValueType div(ValueType x, ValueType y)
   {
      return x/y;
   }

   /// Addition with assignment
   static inline void addAssign(RefType x, ValueType y)
   {
      x += y;
   }

   /// Subtraction with assignment
   static inline void subAssign(RefType x, ValueType y)
   {
      x -= y;
   }

   /// Multiplication with assignment
   static inline void mulAssign(RefType x, ValueType y)
   {
      x *= y;
   }

   /// Division with assignment
   static inline void divAssign(RefType x, ValueType y)
   {
      x /= y;
   }

   /// Minimum of two numbers
   static inline ValueType min(ValueType x, ValueType y)
   {
      return (x<y) ? x : y;
   }

   /// Maximum of two numbers
   static inline ValueType max(ValueType x, ValueType y)
   {
      return (x>y) ? x : y;
   }

   /// Power function
   static inline ValueType pow(ValueType x, int n)
   {
      return std::pow(x, n);
   }

   /// Absolute value
   static inline ValueType abs(ValueType x)
   {
      return (x<0.0) ? (-x) : x;
   }

   /// Unary subtraction
   static inline ValueType usb(ValueType x)
   {
      return -x;
   }

   /// Square
   static inline ValueType sqr(ValueType x)
   {
      return x*x;
   }

   /// Square root
   static inline ValueType sqrt(ValueType x)
   {
      return std::sqrt(x);
   }

   /// Logarithm
   static inline ValueType log(ValueType x)
   {
      return std::log(x);
   }

   /// Exponential
   static inline ValueType exp(ValueType x)
   {
      return std::exp(x);
   }

   /// Cosine
   static inline ValueType cos(ValueType x)
   {
      return std::cos(x);
   }

   /// Sine
   static inline ValueType sin(ValueType x)
   {
      return std::sin(x);
   }

   /// Tangent
   static inline ValueType tan(ValueType x)
   {
      return std::tan(x);
   }

   /// Hyperbolic cosine
   static inline ValueType cosh(ValueType x)
   {
      return std::cosh(x);
   }

   /// Hyperbolic sine
   static inline ValueType sinh(ValueType x)
   {
      return std::sinh(x);
   }

   /// Hyperbolic tangent
   static inline ValueType tanh(ValueType x)
   {
      return std::tanh(x);
   }

   /// Sign function
   static inline ValueType sgn(ValueType x)
   {
      return (x == 0.0) ? 0.0 : ((x < 0.0) ? -1.0 : 1.0);
   }

   /// Hash code
   static inline size_t hashCode(ValueType x)
   {
      return hash1<ValueType>(x);
   }

   /// Test of equality
   static inline bool isEqual(ValueType x, ValueType y)
   {
      return x == y;
   }
};

} // namespace

#endif
