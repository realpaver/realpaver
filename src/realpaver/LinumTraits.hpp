///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LINUM_TRAITS_HPP
#define REALPAVER_LINUM_TRAITS_HPP

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a traits class for numbers used in linear algebra.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct LinearTraits
{
   /// Value type
   typedef T ValueType;

   /// Reference type
   typedef T& RefType;

   /// Const reference type
   typedef const T& ConstRefType;

   /// @return the number zero
   static ValueType zero();

   /// @return the number one
   static ValueType one();

   /// Addition
   /// @param x a number
   /// @param y a number
   /// @return x + y
   static ValueType add(ConstRefType x, ConstRefType y);

   /// Subtraction
   /// @param x a number
   /// @param y a number
   /// @return x - y
   static ValueType sub(ConstRefType x, ConstRefType y);

   /// Multiplication
   /// @param x a number
   /// @param y a number
   /// @return x * y
   static ValueType mul(ConstRefType x, ConstRefType y);

   /// Division
   /// @param x a number
   /// @param y a number
   /// @return x / y
   static ValueType div(ConstRefType x, ConstRefType y);

   /// Square
   /// @param x a number
   /// @return square of x
   static ValueType sqr(ConstRefType x);

   /// Square root
   /// @param x a number
   /// @return square root of x
   static ValueType sqrt(ConstRefType x);

   /// Addition with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x += y
   static void addAssign(RefType x, ConstRefType y);

   /// Subtraction with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x -= y
   static void subAssign(ValueType& x, ConstRefType y);

   /// Multiplication with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x *= y
   static void mulAssign(RefType x, ConstRefType y);

   /// Division with assignment
   /// @param x a number
   /// @param y a number
   ///
   /// x /= y
   static void divAssign(ValueType& x, ConstRefType y);

   /// Test of infinity
   /// @param x a number
   /// @return true if 'x' is infinite
   static bool isInf(ConstRefType x);

   /// Test of equality
   /// @param x a number
   /// @param y a number
   /// @return true if 'x' is equal to 'y'
   static bool isEqual(ConstRefType x, ConstRefType y);

   /// Hash code
   /// @param x a number
   /// @return the hash code of x
   static size_t hashCode(ConstRefType  x);
};

} // namespace

#endif
