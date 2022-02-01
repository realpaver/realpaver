///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LINUM_DOUBLE_TRAITS_HPP
#define REALPAVER_LINUM_DOUBLE_TRAITS_HPP

#include "Double.hpp"
#include "LinumTraits.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a specialization of a traits class for real numbers.
///////////////////////////////////////////////////////////////////////////////
template <>
struct LinearTraits<double>
{
   /// Value type
   typedef double ValueType;

   /// Reference type
   typedef double& RefType;

   /// Const reference type
   typedef const double& ConstRefType;

   /// @return the number zero
   static inline ValueType zero()
   {
      return 0.0;
   }

   /// @return the number one
   static inline ValueType one()
   {
      return 1.0;
   }

   /// Addition
   /// @param x a number
   /// @param y a number
   /// @return x + y
   static inline ValueType add(ConstRefType x, ConstRefType y)
   {
      return Double::add(x, y);
   }

   /// Subtraction
   /// @param x a number
   /// @param y a number
   /// @return x - y
   static inline ValueType sub(ConstRefType x, ConstRefType y)
   {
      return Double::sub(x, y);
   }

   /// Multiplication
   /// @param x a number
   /// @param y a number
   /// @return x * y
   static inline ValueType mul(ConstRefType x, ConstRefType y)
   {
      return Double::mul(x, y);
   }

   /// Division
   /// @param x a number
   /// @param y a number
   /// @return x / y
   static inline ValueType div(ConstRefType x, ConstRefType y)
   {
      return Double::div(x, y);
   }

   /// Square
   /// @param x a number
   /// @return square of x
   static inline ValueType sqr(ConstRefType x)
   {
      return Double::sqr(x);
   }

   /// Square root
   /// @param x a number
   /// @return square root of x
   static inline ValueType sqrt(ConstRefType x)
   {
      return Double::sqrt(x);
   }

   /// Addition with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x += y
   static inline void addAssign(RefType x, ConstRefType y)
   {
      Double::addAssign(x, y);
   }

   /// Subtraction with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x -= y
   static inline void subAssign(ValueType& x, ConstRefType y)
   {
      Double::subAssign(x, y);
   }

   /// Multiplication with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x *= y
   static inline void mulAssign(RefType x, ConstRefType y)
   {
      Double::mulAssign(x, y);
   }

  /// Division with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x /= y
   static inline void divAssign(ValueType& x, ConstRefType y)
   {
      Double::divAssign(x, y);
   }

   /// Test of infinity
   /// @param x a number
   /// @return true if 'x' is infinite
   static inline bool isInf(ConstRefType x)
   {
      return Double::isInf(x);
   }

   /// Test of equality
   /// @param x a number
   /// @param y a number
   /// @return true if 'x' is equal to 'y'
   static inline bool isEqual(ConstRefType x, ConstRefType y)
   {
      return Double::isEqual(x, y);
   }

   /// Hash code
   /// @param x a number
   /// @return the hash code of x
   static inline size_t hashCode(ConstRefType  x)
   {
      return Double::hashCode(x);
   }
};

} // namespace

#endif
