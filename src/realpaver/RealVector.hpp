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
#include "realpaver/NumericVector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// @brief This is a vector of real numbers.
///
/// The elements of a vector of size n are indexed from 0 to n-1.
///////////////////////////////////////////////////////////////////////////////
class RealVector : public NumericVector<double> {
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

   /// Default destructor
   ~RealVector() = default;

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
};

} // namespace

#endif
