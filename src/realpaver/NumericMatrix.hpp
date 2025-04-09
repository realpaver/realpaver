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
 * @file   NumericMatrix.hpp
 * @brief  Dense numeric matrix
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NUMERIC_MATRIX_HPP
#define REALPAVER_NUMERIC_MATRIX_HPP

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/NumericTraits.hpp"
#include <iostream>
#include <vector>

namespace realpaver {

/**
 * @brief Dense numeric matrix of elements of type T.
 *
 * A matrix is represented by a vector following a row orientation.
 */
template <typename T> class NumericMatrix {
protected:
   /// Traits class
   using TraitsType = NumericTraits<T>;

public:
   /// Value type
   using ValueType = T;

   /// Reference type
   using RefType = T &;

   /// Const reference type
   using ConstRefType = const T &;

   /// Constructor
   NumericMatrix(size_t nrows, size_t ncols);

   /**
    * @brief Constructor.
    * @param nrows number of rows
    * @param ncols number of columns
    * @param init value of all the elements
    */
   NumericMatrix(size_t nrows, size_t ncols, ConstRefType init);

   /// Default copy constructor
   NumericMatrix(const NumericMatrix &) = default;

   /// Default assignment operator
   NumericMatrix &operator=(const NumericMatrix &) = default;

   /// Virtual destructor
   virtual ~NumericMatrix();

   /// Returns the number of rows of this
   size_t nrows() const;

   /// Returns the number of columns of this
   size_t ncols() const;

   /// Returns true if this is a square matrix
   bool isSquare() const;

   /// Returns this(i, j) (constant access)
   ValueType operator()(size_t i, size_t j) const;

   /// Returns this(i, j) (non constant access)
   RefType operator()(size_t i, size_t j);

   /// Assigns all the element of this to x
   void setAll(ConstRefType x);

   /// Swaps the i-th row and the j-th row
   void swapRows(size_t i, size_t j);

   /// Swaps the i-th column and the j-th column
   void swapCols(size_t i, size_t j);

   /// Output on a stream
   virtual void print(std::ostream &os) const;

   /// Returns true if one element of this in infinite
   bool isInf() const;

   /// Returns true if all the elements of this are finite
   bool isFinite() const;

   /// Assigns res to A + B
   static void add(const NumericMatrix &A, const NumericMatrix &B, NumericMatrix &res);

   /// Assigns res to A - B
   static void sub(const NumericMatrix &A, const NumericMatrix &B, NumericMatrix &res);

   /// Assigns res to -A
   static void usb(const NumericMatrix &A, NumericMatrix &res);

   /// Assigns res to a * B
   static void mulScalar(ConstRefType a, const NumericMatrix &B, NumericMatrix &res);

   /// Assigns res to B / a
   static void divScalar(const NumericMatrix &B, ConstRefType a, NumericMatrix &res);

   /// Assigns res to A * B
   static void mul(const NumericMatrix &A, const NumericMatrix &B, NumericMatrix &res);

   /// Assigns the number of rows
   void setNrows(size_t nrows);

   /// Assigns the number of columns
   void setNcols(size_t ncols);

   /// Inserts x at the end of the vector representing this
   void push(ConstRefType x);

private:
   typedef std::vector<ValueType> MatrixType;
   MatrixType elems_; // vector of coefficients
   size_t nrows_;     // number of rows
   size_t ncols_;     // number of columns

   // Constructor from a vector
   NumericMatrix(const MatrixType &m);
};

template <typename T>
NumericMatrix<T>::NumericMatrix(size_t nrows, size_t ncols)
    : elems_(nrows * ncols)
    , nrows_(nrows)
    , ncols_(ncols)
{
}

template <typename T>
NumericMatrix<T>::NumericMatrix(size_t nrows, size_t ncols, ConstRefType init)
    : elems_(nrows * ncols)
    , nrows_(nrows)
    , ncols_(ncols)
{
   for (size_t i = 0; i < elems_.size(); ++i)
      elems_[i] = init;
}

template <typename T> NumericMatrix<T>::~NumericMatrix()
{
}

template <typename T> size_t NumericMatrix<T>::nrows() const
{
   return nrows_;
}

template <typename T> size_t NumericMatrix<T>::ncols() const
{
   return ncols_;
}

template <typename T> bool NumericMatrix<T>::isSquare() const
{
   return nrows_ == ncols_;
}

template <typename T>
typename NumericMatrix<T>::ValueType NumericMatrix<T>::operator()(size_t i,
                                                                  size_t j) const
{
   ASSERT(i < nrows() && j < ncols(),
          "Bad access in a matrix at position " << i << ", " << j);

   return elems_[i * ncols_ + j];
}

template <typename T>
typename NumericMatrix<T>::RefType NumericMatrix<T>::operator()(size_t i, size_t j)
{
   ASSERT(i < nrows() && j < ncols(),
          "Bad access in a matrix at position " << i << ", " << j);

   return elems_[i * ncols_ + j];
}

template <typename T> void NumericMatrix<T>::setAll(ConstRefType x)
{
   for (size_t i = 0; i < elems_.size(); ++i)
      elems_[i] = x;
}

template <typename T> void NumericMatrix<T>::swapRows(size_t i, size_t j)
{
   for (size_t k = 0; k < ncols(); ++k)
   {
      T tmp = operator()(i, k);
      operator()(i, k) = operator()(j, k);
      operator()(j, k) = tmp;
   }
}

template <typename T> void NumericMatrix<T>::swapCols(size_t i, size_t j)
{
   for (size_t k = 0; k < nrows(); ++k)
   {
      T tmp = operator()(k, i);
      operator()(k, i) = operator()(k, j);
      operator()(k, j) = tmp;
   }
}

template <typename T> void NumericMatrix<T>::print(std::ostream &os) const
{
   os << '(';
   for (size_t i = 0; i < nrows_; ++i)
   {
      if (i > 0)
         os << std::endl;
      os << '(';
      for (size_t j = 0; j < ncols_; ++j)
      {
         if (j > 0)
            os << " ; ";
         os << operator()(i, j);
      }
      os << ')';
   }
   os << ')';
}

template <typename T> bool NumericMatrix<T>::isInf() const
{
   for (size_t i = 0; i < elems_.size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return true;

   return false;
}

template <typename T> bool NumericMatrix<T>::isFinite() const
{
   for (size_t i = 0; i < elems_.size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return false;

   return true;
}

template <typename T>
void NumericMatrix<T>::add(const NumericMatrix &A, const NumericMatrix &B,
                           NumericMatrix &res)
{
   ASSERT(A.nrows() == B.nrows(), "Bad matrix sizes in an addition");
   ASSERT(A.ncols() == B.ncols(), "Bad matrix sizes in an addition");
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in an addition");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in an addition");

   for (size_t i = 0; i < A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::add(A.elems_[i], B.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::sub(const NumericMatrix &A, const NumericMatrix &B,
                           NumericMatrix &res)
{
   ASSERT(A.nrows() == B.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == B.ncols(), "Bad matrix sizes in a subtraction");
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in a subtraction");

   for (size_t i = 0; i < A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::sub(A.elems_[i], B.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::usb(const NumericMatrix &A, NumericMatrix &res)
{
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in a subtraction");

   for (size_t i = 0; i < A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::usb(A.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::mulScalar(ConstRefType a, const NumericMatrix &B,
                                 NumericMatrix &res)
{
   ASSERT(B.nrows() == res.nrows(), "Bad matrix sizes in a multiplication");
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a multiplication");

   for (size_t i = 0; i < B.elems_.size(); ++i)
      res.elems_[i] = TraitsType::mul(a, B.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::divScalar(const NumericMatrix &B, ConstRefType a,
                                 NumericMatrix &res)
{
   ASSERT(B.nrows() == res.nrows(), "Bad matrix sizes in a division");
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a division");

   for (size_t i = 0; i < B.elems_.size(); ++i)
      res.elems_[i] = TraitsType::div(B.elems_[i], a);
}

template <typename T>
void NumericMatrix<T>::mul(const NumericMatrix &A, const NumericMatrix &B,
                           NumericMatrix &res)
{
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a multiplication");
   ASSERT(A.ncols() == B.nrows(), "Bad matrix sizes in a multiplication");
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a multiplication");

   for (size_t i = 0; i < res.nrows(); ++i)
      for (size_t j = 0; j < res.ncols(); ++j)
      {
         ValueType s = TraitsType::zero();
         for (size_t k = 0; k < A.ncols(); ++k)
            TraitsType::addAssign(s, TraitsType::mul(A(i, k), B(k, j)));

         res(i, j) = s;
      }
}

template <typename T>
NumericMatrix<T>::NumericMatrix(const MatrixType &m)
    : elems_(m)
{
}

template <typename T> void NumericMatrix<T>::setNrows(size_t nrows)
{
   nrows_ = nrows;
}

template <typename T> void NumericMatrix<T>::setNcols(size_t ncols)
{
   ncols_ = ncols;
}

template <typename T> void NumericMatrix<T>::push(ConstRefType x)
{
   elems_.push_back(x);
}

} // namespace realpaver

#endif
