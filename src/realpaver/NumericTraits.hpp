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
 * @file   NumericTraits.hpp
 * @brief  Traits class for numbers used in linear algebra
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#ifndef REALPAVER_NUMERIC_TRAITS_HPP
#define REALPAVER_NUMERIC_TRAITS_HPP

namespace realpaver {

/// Traits class for numbers used in linear algebra.
template <typename T>
struct NumericTraits
{
   /// Value type
   using ValueType = T;

   /// Reference type
   using RefType = T&;

   /// Const reference type
   using ConstRefType = const T&;

   /// Returns the number zero
   static ValueType zero();

   /// Returns the number one
   static ValueType one();

   /// Returns x + y
   static ValueType add(ConstRefType x, ConstRefType y);

   /// Returns x - y
   static ValueType sub(ConstRefType x, ConstRefType y);

   /// Returns -x
   static ValueType usb(ConstRefType x);

   /// Returns x * y
   static ValueType mul(ConstRefType x, ConstRefType y);

   /// Returns x / y
   static ValueType div(ConstRefType x, ConstRefType y);

   /// Returns the square of x
   static ValueType sqr(ConstRefType x);

   /// Returns the square root of x
   static ValueType sqrt(ConstRefType x);

   /// x += y
   static void addAssign(RefType x, ConstRefType y);

   /// x -= y
   static void subAssign(ValueType& x, ConstRefType y);

   /// x *= y
   static void mulAssign(RefType x, ConstRefType y);

   /// x /= y
   static void divAssign(ValueType& x, ConstRefType y);

   /// Returns true if x is infinite
   static bool isInf(ConstRefType x);

   /// Returns true if x is equal to y
   static bool isEqual(ConstRefType x, ConstRefType y);

   /// Returns the hash code of x
   static size_t hashCode(ConstRefType  x);
};

} // namespace

#endif
