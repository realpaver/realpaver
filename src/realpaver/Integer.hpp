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
 * @file   Integer.hpp
 * @brief  Class of safe integers
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_INTEGER_HPP
#define REALPAVER_INTEGER_HPP

#include <iostream>
#include <limits>

namespace realpaver {
   
/**
 * @brief Signed integer with checked underflows and overflows.
 * 
 * Exceptions are thrown when underflows and overflows are detected.
 */
class Integer {
public:
   /// type of value encapsulated in this
   using ValueType = long;

   /// Constructor
   Integer(const int& n = 0);

   /// Constructor
   Integer(const size_t& n);

   /// Constructor
   Integer(const ValueType& n);

   /// Default copy constructor
   Integer(const Integer&) = default;

   /// Default assignment
   Integer& operator=(const Integer&) = default;

   /// Default destructor
   ~Integer() = default;

   /// Returns the smallest integer
   static ValueType MIN();

   /// Returns the greatest integer
   static ValueType MAX();

   /// Returns the value of this in double precision
   double toDouble() const;

   /// Returns an int equivalent to this to an int if it is possible, throws an exception
   /// otherwise
   int toInt() const;

   /**
    * @brief Floor function.
    * 
    * Returns the largest integer smaller than a if 'a' is representable,
    * throws an exception otherwise
    */
   static Integer floor(const double& a);

   /**
    * @brief Ceil function.
    * 
    * Return thes smallest integer greater than a if 'a' is representable,
    * throws an exception otherwise
    */
   static Integer ceil(const double& a);

   /// REeturns the value of this
   ValueType get() const;

   /// Returns true if this is even
   bool isEven() const;

   /// Returns true if this is odd
   bool isOdd() const;

   /// Addition with assignment
   Integer& operator+=(const Integer& other);

   /// Subtraction with assignment
   Integer& operator-=(const Integer& other);

   /// Multiplication with assignment
   Integer& operator*=(const Integer& other);

   /// Division with assignment
   Integer& operator/=(const Integer& other);

   /// Incrementation (prefix)
   Integer& operator++();

   /// Decrementation (prefix)
   Integer& operator--();

   friend Integer operator+(const Integer& a, const Integer& b);
   friend Integer operator-(const Integer& a, const Integer& b);
   friend Integer operator-(const Integer& a);
   friend Integer operator*(const Integer& a, const Integer& b);
   friend Integer operator/(const Integer& a, const Integer& b);
   friend Integer operator%(const Integer& a, int n);

   friend Integer abs(const Integer& a);
   friend Integer sqr(const Integer& a);
   friend Integer divFloor(const Integer& a, const Integer& b);
   friend Integer divCeil(const Integer& a, const Integer& b);
   friend Integer sqrtFloor(const Integer& a);
   friend Integer sqrtCeil(const Integer& a);

   friend Integer min(const Integer& a, const Integer& b);
   friend Integer max(const Integer& a, const Integer& b);

   friend bool operator==(const Integer& a, const Integer& b);
   friend bool operator!=(const Integer& a, const Integer& b);
   friend bool operator>=(const Integer& a, const Integer& b);
   friend bool operator>(const Integer& a, const Integer& b);
   friend bool operator<=(const Integer& a, const Integer& b);
   friend bool operator<(const Integer& a, const Integer& b);

   /// Output on a stream
   friend std::ostream& operator<<(std::ostream& os, const Integer& a);

private:
   ValueType val_;

   // checking of overflows and underflows
   static bool OutOfRangeAdd(const ValueType& a, const ValueType& b);
   static bool OutOfRangeSub(const ValueType& a, const ValueType& b);
   static bool OutOfRangeMul(const ValueType& a, const ValueType& b);
   static bool OutOfRangeDiv(const ValueType& a, const ValueType& b);

   // absolute value
   static ValueType Abs(const ValueType& a);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Integer& a);

/// Addition
Integer operator+(const Integer& a, const Integer& b);

/// Subtraction
Integer operator-(const Integer& a, const Integer& b);

/// Unaru subtraction
Integer operator-(const Integer& a);

/// Multiplication
Integer operator*(const Integer& a, const Integer& b);

/// Division
Integer operator/(const Integer& a, const Integer& b);

/// Modulo
Integer operator%(const Integer& a, int n);

/// Absolute value
Integer abs(const Integer& a);

/// Square
Integer sqr(const Integer& a);

/// Division rounded downward
Integer divFloor(const Integer& a, const Integer& b);

/// Division rounded upward
Integer divCeil(const Integer& a, const Integer& b);

/// Square root rounded downward
Integer sqrtFloor(const Integer& a);

/// Square root rounded upward
Integer sqrtCeil(const Integer& a);

/// Minimum
Integer min(const Integer& a, const Integer& b);

/// Maximum
Integer max(const Integer& a, const Integer& b);

/// Comparison
bool operator==(const Integer& a, const Integer& b);

/// Comparison
bool operator!=(const Integer& a, const Integer& b);

/// Comparison
bool operator>=(const Integer& a, const Integer& b);

/// Comparison
bool operator>(const Integer& a, const Integer& b);

/// Comparison
bool operator<=(const Integer& a, const Integer& b);

/// Comparison
bool operator<(const Integer& a, const Integer& b);

} // namespace

#endif
