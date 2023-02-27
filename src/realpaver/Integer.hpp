///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTEGER_HPP
#define REALPAVER_INTEGER_HPP

#include <iostream>
#include <limits>

namespace realpaver {
   
///////////////////////////////////////////////////////////////////////////////
/// This is a signed integer with checked underflows and overflows.
///////////////////////////////////////////////////////////////////////////////
class Integer {
public:
   /// type of value encapsulated in this
   typedef long ValueType;

   /// Creates an integer
   /// @param n value of this
   Integer(const int& n = 0);

   /// Creates an integer
   /// @param n value of this
   Integer(const size_t& n);

   /// Creates an integer
   /// @param n value of this
   Integer(const ValueType& n);

   /// Default copy constructor
   Integer(const Integer&) = default;

   /// Default assignment
   Integer& operator=(const Integer&) = default;

   /// Default destructor
   ~Integer() = default;

   /// @return the smallest integer
   static ValueType MIN();

   /// @return the greatest integer
   static ValueType MAX();

   /// @return the value of this in double precision
   double toDouble() const;

   /// @return casts this to an int if it is possible, throws an exception
   ///         otherwise
   int toInt() const;

   /// Floor function
   /// @param a a real number
   /// @return the largest integer smaller than a if 'a' is representable,
   ///         throws an exception otherwise
   static Integer floor(const double& a);

   /// Ceiling function
   /// @param a a real number
   /// @return the smallest integer greater than a if 'a' is representable,
   ///         throws an exception otherwise
   static Integer ceil(const double& a);

   /// @return the value of this
   ValueType get() const;

   /// @return true if this is even
   bool isEven() const;

   /// @return true if this is odd
   bool isOdd() const;

   ///@{
   /// Arithmetic operations with assignment
   Integer& operator+=(const Integer& other);
   Integer& operator-=(const Integer& other);
   Integer& operator*=(const Integer& other);
   Integer& operator/=(const Integer& other);
   Integer& operator++();
   Integer& operator--();
   ///@}

   ///@{
   /// Arithmetic operations and functions
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
   ///@}

   ///@{
   /// Comparison operators
   friend bool operator==(const Integer& a, const Integer& b);
   friend bool operator!=(const Integer& a, const Integer& b);
   friend bool operator>=(const Integer& a, const Integer& b);
   friend bool operator>(const Integer& a, const Integer& b);
   friend bool operator<=(const Integer& a, const Integer& b);
   friend bool operator<(const Integer& a, const Integer& b);
   ///@}

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

/// output on a stream
std::ostream& operator<<(std::ostream& os, const Integer& a);

///@{
/// Arithmetic operations and functions
Integer operator+(const Integer& a, const Integer& b);
Integer operator-(const Integer& a, const Integer& b);
Integer operator-(const Integer& a);
Integer operator*(const Integer& a, const Integer& b);
Integer operator/(const Integer& a, const Integer& b);
Integer operator%(const Integer& a, int n);
Integer abs(const Integer& a);
Integer sqr(const Integer& a);
Integer divFloor(const Integer& a, const Integer& b);
Integer divCeil(const Integer& a, const Integer& b);
Integer sqrtFloor(const Integer& a);
Integer sqrtCeil(const Integer& a);
Integer min(const Integer& a, const Integer& b);
Integer max(const Integer& a, const Integer& b);
///@}
   
} // namespace

#endif
