///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DOUBLE_HPP
#define REALPAVER_DOUBLE_HPP

#include <cfenv>
#include <cmath>
#include <limits>
#include "realpaver/Common.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This describes values and operations on floating-point numbers.
///////////////////////////////////////////////////////////////////////////////
struct Double {
   /// Value type
   typedef double ValueType;

   /// Reference type
   typedef double& RefType;

   /// Constant reference type
   typedef const double& ConstRefType;

   /// @return +Infinity
   static inline ValueType inf()
   {
      return std::numeric_limits<ValueType>::infinity();
   }

   /// @return -Infinity
   static inline ValueType neginf()
   {
      return -std::numeric_limits<ValueType>::infinity();
   }

   /// @return Not a Number
   static inline ValueType nan()
   {
      return std::numeric_limits<ValueType>::quiet_NaN();
   }

   /// @return the lowest representable value
   static inline ValueType lowest()
   {
      return -std::numeric_limits<ValueType>::max();
   }

   /// @return the greatest representable value
   static inline ValueType greatest()
   {
      return std::numeric_limits<ValueType>::max();
   }

   /// @param x a number
   /// @return true if x is equal to -Infinity or +Infinity
   static inline bool isInf(ValueType x)
   {
      return (x==inf()) || (x==neginf());
   }

   /// @param x a number
   /// @return true if x is not a number
   static inline bool isNan(ValueType x)
   {
      return x != x;
   }

   /// @param x a number
   /// @return the double before x
   static inline ValueType prevDouble(ValueType x)
   {
      return std::nextafter(x, neginf());
   }

   /// @param x a number
   /// @return the double after x
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
