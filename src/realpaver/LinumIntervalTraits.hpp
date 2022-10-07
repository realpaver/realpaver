///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LINUM_INTERVAL_TRAITS_HPP
#define REALPAVER_LINUM_INTERVAL_TRAITS_HPP

#include "Interval.hpp"
#include "LinumTraits.hpp"

namespace realpaver {
   
static inline Interval itv_sqr(const Interval& x)
{
   return sqr(x);
}

static inline Interval itv_sqrt(const Interval& x)
{
   return sqrt(x);
}

///////////////////////////////////////////////////////////////////////////////
/// This is a specialization of a traits class for intervals.
///////////////////////////////////////////////////////////////////////////////
template <>
struct LinumTraits<Interval>
{
   /// Value type
   typedef Interval ValueType;

   /// Reference type
   typedef Interval& RefType;

   /// Const reference type
   typedef const Interval& ConstRefType;

   /// @return the number zero
   static inline ValueType zero()
   {
      return Interval::zero();
   }

   /// @return the number one
   static inline ValueType one()
   {
      return Interval::one();
   }

   /// Addition
   /// @param x a number
   /// @param y a number
   /// @return x + y
   static inline ValueType add(ConstRefType x, ConstRefType y)
   {
      return x + y;
   }

   /// Subtraction
   /// @param x a number
   /// @param y a number
   /// @return x - y
   static inline ValueType sub(ConstRefType x, ConstRefType y)
   {
      return x - y;
   }

   /// Unary subtraction
   /// @param x a number
   /// @return -x
   static ValueType usb(ConstRefType x)
   {
      return -x;
   }

   /// Multiplication
   /// @param x a number
   /// @param y a number
   /// @return x * y
   static inline ValueType mul(ConstRefType x, ConstRefType y)
   {
      return x * y;
   }

   /// Division
   /// @param x a number
   /// @param y a number
   /// @return x / y
   static inline ValueType div(ConstRefType x, ConstRefType y)
   {
      return x / y;
   }

   /// Square
   /// @param x a number
   /// @return square of x
   static inline ValueType sqr(ConstRefType x)
   {
      return itv_sqr(x);
   }

   /// Square root
   /// @param x a number
   /// @return square root of x
   static inline ValueType sqrt(ConstRefType x)
   {
      return itv_sqrt(x);
   }

   /// Addition with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x += y
   static inline void addAssign(RefType x, ConstRefType y)
   {
      x += y;
   }

   /// Subtraction with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x -= y
   static inline void subAssign(ValueType& x, ConstRefType y)
   {
      x -= y;
   }

   /// Multiplication with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x *= y
   static inline void mulAssign(RefType x, ConstRefType y)
   {
      x *= y;
   }

  /// Division with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x /= y
   static inline void divAssign(ValueType& x, ConstRefType y)
   {
      x /= y;
   }

   /// Test of infinity
   /// @param x a number
   /// @return true if 'x' is infinite
   static inline bool isInf(ConstRefType x)
   {
      return x.isInf();
   }

   /// Test of equality
   /// @param x a number
   /// @param y a number
   /// @return true if 'x' is equal to 'y'
   static inline bool isEqual(ConstRefType x, ConstRefType y)
   {
      return x.isSetEq(y);
   }

   /// Hash code
   /// @param x a number
   /// @return the hash code of x
   static inline size_t hashCode(ConstRefType  x)
   {
      return x.hashCode();
   }
};

} // namespace

#endif
