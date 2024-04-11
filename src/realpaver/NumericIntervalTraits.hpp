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
 * @file   NumericIntervalTraits.hpp
 * @brief  Specialization of template NumericTraits for intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NUMERIC_INTERVAL_TRAITS_HPP
#define REALPAVER_NUMERIC_INTERVAL_TRAITS_HPP

#include "Interval.hpp"
#include "NumericTraits.hpp"

namespace realpaver {
   
static inline Interval itv_sqr(const Interval& x)
{
   return sqr(x);
}

static inline Interval itv_sqrt(const Interval& x)
{
   return sqrt(x);
}


/**
 * @brief Specialization of template NumericTraits for intervals.
 * 
 * This traits is used to parameterize matrices, vectors, and algorithms
 * from linear algebra.
 * 
 * @see NumericTraits
 */
template <>
struct NumericTraits<Interval>
{
   using ValueType = Interval;
   using RefType = Interval&;
   using ConstRefType = const Interval&;

   static inline ValueType zero()
   {
      return Interval::zero();
   }

   static inline ValueType one()
   {
      return Interval::one();
   }

   static inline ValueType add(ConstRefType x, ConstRefType y)
   {
      return x + y;
   }

   static inline ValueType sub(ConstRefType x, ConstRefType y)
   {
      return x - y;
   }

   static ValueType usb(ConstRefType x)
   {
      return -x;
   }

   static inline ValueType mul(ConstRefType x, ConstRefType y)
   {
      return x * y;
   }

   static inline ValueType div(ConstRefType x, ConstRefType y)
   {
      return x / y;
   }

   static inline ValueType sqr(ConstRefType x)
   {
      return itv_sqr(x);
   }

   static inline ValueType sqrt(ConstRefType x)
   {
      return itv_sqrt(x);
   }

   static inline void addAssign(RefType x, ConstRefType y)
   {
      x += y;
   }

   static inline void subAssign(ValueType& x, ConstRefType y)
   {
      x -= y;
   }

   static inline void mulAssign(RefType x, ConstRefType y)
   {
      x *= y;
   }

   static inline void divAssign(ValueType& x, ConstRefType y)
   {
      x /= y;
   }

   static inline bool isInf(ConstRefType x)
   {
      return x.isInf();
   }

   static inline bool isEqual(ConstRefType x, ConstRefType y)
   {
      return x.isSetEq(y);
   }

   static inline size_t hashCode(ConstRefType  x)
   {
      return x.hashCode();
   }
};

} // namespace

#endif
