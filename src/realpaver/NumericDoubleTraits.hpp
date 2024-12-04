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
 * @file   NumericDoubleTraits.hpp
 * @brief  Specialization of template NumericTraits for doubles
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NUMERIC_DOUBLE_TRAITS_HPP
#define REALPAVER_NUMERIC_DOUBLE_TRAITS_HPP

#include "Double.hpp"
#include "NumericTraits.hpp"

namespace realpaver {

/**
 * @brief Specialization of template NumericTraits for doubles.
 * 
 * This traits is used to parameterize matrices, vectors, and algorithms
 * from linear algebra.
 * 
 * @see NumericTraits
 */
template <>
struct NumericTraits<double>
{
   using ValueType    = double;
   using RefType      = double&;
   using ConstRefType = const double&;

   static inline ValueType zero()
   {
      return 0.0;
   }

   static inline ValueType one()
   {
      return 1.0;
   }

   static inline ValueType add(ConstRefType x, ConstRefType y)
   {
      return Double::add(x, y);
   }

   static inline ValueType sub(ConstRefType x, ConstRefType y)
   {
      return Double::sub(x, y);
   }

   static ValueType usb(ConstRefType x)
   {
      return -x;
   }

   static inline ValueType mul(ConstRefType x, ConstRefType y)
   {
      return Double::mul(x, y);
   }

   static inline ValueType div(ConstRefType x, ConstRefType y)
   {
      return Double::div(x, y);
   }

   static inline ValueType sqr(ConstRefType x)
   {
      return Double::sqr(x);
   }

   static inline ValueType sqrt(ConstRefType x)
   {
      return Double::sqrt(x);
   }

   static inline void addAssign(RefType x, ConstRefType y)
   {
      Double::addAssign(x, y);
   }

   static inline void subAssign(ValueType& x, ConstRefType y)
   {
      Double::subAssign(x, y);
   }

   static inline void mulAssign(RefType x, ConstRefType y)
   {
      Double::mulAssign(x, y);
   }

   static inline void divAssign(ValueType& x, ConstRefType y)
   {
      Double::divAssign(x, y);
   }

   static inline bool isInf(ConstRefType x)
   {
      return Double::isInf(x);
   }

   static inline bool isEqual(ConstRefType x, ConstRefType y)
   {
      return Double::isEqual(x, y);
   }

   static inline size_t hashCode(ConstRefType  x)
   {
      return Double::hashCode(x);
   }
};

} // namespace

#endif
