///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_REAL_VECTOR_HPP
#define REALPAVER_REAL_VECTOR_HPP

#include "realpaver/Double.hpp"
#include "realpaver/LinumDoubleTraits.hpp"
#include "realpaver/NumericVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a vector of real numbers.
///
/// The elements of a vector of size n are indexed from 0 to n-1.
///////////////////////////////////////////////////////////////////////////////
class RealVector : public NumericVector<double> {
public:
   /// Base class
   typedef NumericVector<double> BaseType;

   /// Creates a vector
   /// @param n number of elements
   /// @param x initialization value
   RealVector(size_t n = 0, double x = 0.0);
   
   /// Creates a vector from a list
   /// @param l list of elements inserted in this
   RealVector(const std::initializer_list<double>& l);

   /// Default copy constructor
   RealVector(const RealVector&) = default;

   /// Assignment operator
   /// @param V a vector assigned to this
   /// @return a reference to this
   RealVector& operator=(const RealVector& V);

   /// Default destructor
   ~RealVector() = default;

   /// @param i an index between 0 and size()-1
   /// @return the i-th element of this
   double get(size_t i) const;

   /// Sets an element of this
   /// @param i an index between 0 and size()-1
   /// @param x a real number
   void set(size_t i, double x);

   /// @return true if at least one component is a NaN
   bool isNan() const;

   /// Scalar product
   /// @param V a vector having the same size than this
   /// @return the scalar product of this and V
   double scalarProduct(const RealVector& V) const;

   /// @return the Euclidean norm of this
   double euclideanNorm() const;

   /// @return the one-norm of this
   double oneNorm() const;

   /// @return the infinite-norm of this
   double infNorm() const;

   /// Addition with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this is assigned to this + V
   RealVector& operator+=(const RealVector& V);

   /// Subtraction with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this is assigned to this - V
   RealVector& operator-=(const RealVector& V);

   /// Multiplication with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this * a
   RealVector& operator*=(double a);

   /// Division with assignment
   /// @param a a scalar
   /// @return a reference to this
   ///
   /// this is assigned to this / a
   RealVector& operator/=(double a);
};

///@{
/// Operators on real vectors
RealVector operator+(const RealVector& V, const RealVector& W);
RealVector operator-(const RealVector& V, const RealVector& W);
RealVector operator-(const RealVector& V);

RealVector operator*(double a, const RealVector& V);
RealVector operator*(const RealVector& V, double a);

RealVector operator/(const RealVector& V, double a);
///@}

} // namespace

#endif
