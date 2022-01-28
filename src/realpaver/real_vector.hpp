///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, a reliable interval solver of nonlinear   //
//                                 constraint satisfaction and optimization  //
//                                 problems over the real numbers.           //
//                                                                           //
// Copyright (C) 2020-2022 Laboratoire des Sciences du Numérique de Nantes   //
//                                                                           //
// Realpaver is a software distributed under the terms of the MIT License.   //
// See the file COPYING.                                                     //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_VECTOR_HPP
#define REALPAVER_REAL_VECTOR_HPP

#include <iostream>
#include <vector>
#include "realpaver/number.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a vector of real numbers.
///
/// The elements of a vector of size n are indexed from 0 to n-1.
///////////////////////////////////////////////////////////////////////////////
class RealVector {
public:
   /// Creates an empty vector
   RealVector();

   /// Creates a vector
   /// @param n number of elements
   /// @param x initialization value
   RealVector(size_t n, double x = 0.0);
   
   /// Creates a vector from a list
   /// @param l list of elements inserted in this
   RealVector(const std::initializer_list<double>& l);

   /// Default copy constructor
   RealVector(const RealVector&) = default;

   /// No assignment
   RealVector& operator=(const RealVector&) = delete;

   /// @return the number of elements of this
   size_t size() const;

   /// Constant access in this
   /// @param i index of an element
   /// @return the i-th element of this
   double operator[](size_t i) const;

   /// Constant and safe access in this
   /// @param i index of an element
   /// @return the i-th element of this
   ///
   /// An assertion verifies that i is a valid index.
   double at(size_t i) const;

   /// Assignment of an element
   /// @param i index of an element
   /// @param x the new value of the i-th element of this
   void set(size_t i, double x);

   /// Assignment of all the elements
   /// @param x the new value of all the elements of this
   void setAll(double x);

   /// Inserts an element at the end
   /// @param x the new element inserted at the end of this
   void push(double x);

   /// @return hash code of this
   size_t hashCode() const;

   /// @return true if all the elements of this are finite
   bool isFinite() const;

   /// @return true if an element of this is infinite
   bool isInf() const;

   /// @return true if an element of this is not a number
   bool isNan() const;

   /// @return the one-norm of this (sum of absolute values of elements)
   double oneNorm() const;

   /// @return the infinite-norm of this (maximum of absolute values
   ///         of elements)
   double infNorm() const;

private:
   std::vector<double> v_;
};

/// Output on a stream
/// @param os an output stream
/// @param v vector written on os
/// @return os
std::ostream& operator<<(std::ostream& os, const RealVector& v);

} // namespace

#endif
