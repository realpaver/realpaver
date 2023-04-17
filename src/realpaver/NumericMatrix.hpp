///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NUMERIC_MATRIX_HPP
#define REALPAVER_NUMERIC_MATRIX_HPP

#include <iostream>
#include <vector>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/NumericTraits.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a numeric matrix of elements of type T.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class NumericMatrix {
protected:
   /// Traits class
  typedef NumericTraits<T> TraitsType;
      
public:
   /// Value type
   typedef T ValueType;

   /// Reference type
   typedef T& RefType;

   /// Const reference type
   typedef const T& ConstRefType;

   /// Creates a matrix of a given size
   /// @param nrows number of rows
   /// @param ncols number of columns
   NumericMatrix(size_t nrows, size_t ncols);

   /// Creates a matrix and initializes its elements
   /// @param nrows number of rows
   /// @param ncols number of columns
   /// @param init value of all the elements
   NumericMatrix(size_t nrows, size_t ncols, ConstRefType init);

   /// Default copy constructor
   NumericMatrix(const NumericMatrix&) = default;

   /// Default assignment operator
   NumericMatrix& operator=(const NumericMatrix&) = default;

   /// Virtual destructor
   virtual ~NumericMatrix();

   /// @return the number of rows of this
   size_t nrows() const;

   /// @return the number of columns of this
   size_t ncols() const;

   /// @return true if this is a square matrix
   bool isSquare() const;

   /// Constant access in this
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @return the coefficient at (i, j) of this
   ValueType operator()(size_t i, size_t j) const;

   /// Non constant access in this
   /// @param i a row index between 0 and nrows()-1
   /// @param j a column index between 0 and ncols()-1
   /// @return the coefficient at (i, j) of this
   RefType operator()(size_t i, size_t j);

   /// Modification of all the elements of this
   /// @param x a number
   ///
   /// Assigns x to all the element of this
   void setAll(ConstRefType x);

   /// Swaps two rows
   /// @param i a row index
   /// @param j a row index
   void swapRows(size_t i, size_t j);

   /// Swaps two columns
   /// @param i a column index
   /// @param j a column index
   void swapCols(size_t i, size_t j);

   /// Output on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const;

   /// @return true if one element of this in infinite
   bool isInf() const;

   /// @return true if all the elements of this are finite
   bool isFinite() const;

   /// Addition
   /// @param A a matrix
   /// @param B a matrix
   /// @param res result assigned to A + B
   static void add(const NumericMatrix& A, const NumericMatrix& B,
                   NumericMatrix& res);

   /// Subtraction
   /// @param A a matrix
   /// @param B a matrix
   /// @param res result assigned to A - B
   static void sub(const NumericMatrix& A, const NumericMatrix& B,
                   NumericMatrix& res);

   /// Unary subtraction
   /// @param A a matrix
   /// @param res result assigned to - A
   static void usb(const NumericMatrix& A, NumericMatrix& res);

   /// Multiplication
   /// @param a a scalar
   /// @param B a matrix
   /// @param res result assigned to a * B
   static void mulScalar(ConstRefType a, const NumericMatrix& B,
                         NumericMatrix& res);

   /// Division
   /// @param B a matrix
   /// @param a a scalar
   /// @param res result assigned to B / a
   static void divScalar(const NumericMatrix& B, ConstRefType a,
                         NumericMatrix& res);

   /// Multiplication
   /// @param A a matrix
   /// @param B a matrix
   /// @param res result assigned to A * B
   static void mul(const NumericMatrix& A, const NumericMatrix& B,
                   NumericMatrix& res);

   void setNrows(size_t nrows);
   void setNcols(size_t ncols);
   void push(ConstRefType x);

private:
   typedef std::vector<ValueType> MatrixType;
   MatrixType elems_;
   size_t nrows_;
   size_t ncols_;

   NumericMatrix(const MatrixType& m);
};

template <typename T>
NumericMatrix<T>::NumericMatrix(size_t nrows, size_t ncols)
      : elems_(nrows*ncols),
        nrows_(nrows),
        ncols_(ncols)
{}

template <typename T>
NumericMatrix<T>::NumericMatrix(size_t nrows, size_t ncols, ConstRefType init)
      : elems_(nrows*ncols),
        nrows_(nrows),
        ncols_(ncols)
{
   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = init;
}

template <typename T>
NumericMatrix<T>::~NumericMatrix()
{}

template <typename T>
size_t NumericMatrix<T>::nrows() const
{
   return nrows_;
}

template <typename T>
size_t NumericMatrix<T>::ncols() const
{
   return ncols_;
}

template <typename T>
bool NumericMatrix<T>::isSquare() const
{
   return nrows_ == ncols_;
}

template <typename T>
typename NumericMatrix<T>::ValueType
NumericMatrix<T>::operator()(size_t i, size_t j) const
{
   ASSERT(i<nrows() && j<ncols(), "Bad access in a matrix at position "
                                  << i << ", " << j);

   return elems_[i*ncols_+j];
}

template <typename T>
typename NumericMatrix<T>::RefType
NumericMatrix<T>::operator()(size_t i, size_t j)
{
   ASSERT(i<nrows() && j<ncols(), "Bad access in a matrix at position "
                                  << i << ", " << j);

   return elems_[i*ncols_+j];   
}

template <typename T>
void NumericMatrix<T>::setAll(ConstRefType x)
{
   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = x;   
}

template <typename T>
void NumericMatrix<T>::swapRows(size_t i, size_t j)
{
   for (size_t k=0; k<ncols(); ++k)
   {
      T tmp = operator()(i, k);
      operator()(i, k) = operator()(j, k);
      operator()(j, k) = tmp;
   }
}

template <typename T>
void NumericMatrix<T>::swapCols(size_t i, size_t j)
{
   for (size_t k=0; k<nrows(); ++k)
   {
      T tmp = operator()(k, i);
      operator()(k, i) = operator()(k, j);
      operator()(k, j) = tmp;
   }
}

template <typename T>
void NumericMatrix<T>::print(std::ostream& os) const
{
   for (size_t i=0; i<nrows_; ++i)
   {
      if (i>0) os << std::endl;
      os << '(';
      for (size_t j=0; j<ncols_; ++j)
      {
         if (j>0) os << "\t";
         os << operator()(i, j);
      }
      os << ')';
   }
}

template <typename T>
bool NumericMatrix<T>::isInf() const
{
   for (size_t i=0; i<elems_.size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return true;

   return false;
}

template <typename T>
bool NumericMatrix<T>::isFinite() const
{
   for (size_t i=0; i<elems_.size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return false;

   return true;
}

template <typename T>
void NumericMatrix<T>::add(const NumericMatrix& A, const NumericMatrix& B,
                           NumericMatrix& res)
{
   ASSERT(A.nrows() == B.nrows(), "Bad matrix sizes in an addition");
   ASSERT(A.ncols() == B.ncols(), "Bad matrix sizes in an addition");
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in an addition");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in an addition");

   for (size_t i=0; i<A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::add(A.elems_[i], B.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::sub(const NumericMatrix& A, const NumericMatrix& B,
                           NumericMatrix& res)
{
   ASSERT(A.nrows() == B.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == B.ncols(), "Bad matrix sizes in a subtraction");
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in a subtraction");

   for (size_t i=0; i<A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::sub(A.elems_[i], B.elems_[i]);   
}

template <typename T>
void NumericMatrix<T>::usb(const NumericMatrix& A, NumericMatrix& res)
{
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a subtraction");
   ASSERT(A.ncols() == res.ncols(), "Bad matrix sizes in a subtraction");
   
   for (size_t i=0; i<A.elems_.size(); ++i)
      res.elems_[i] = TraitsType::usb(A.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::mulScalar(ConstRefType a, const NumericMatrix& B,
                                 NumericMatrix& res)
{
   ASSERT(B.nrows() == res.nrows(), "Bad matrix sizes in a multiplication");
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a multiplication");

   for (size_t i=0; i<B.elems_.size(); ++i)
      res.elems_[i] = TraitsType::mul(a, B.elems_[i]);
}

template <typename T>
void NumericMatrix<T>::divScalar(const NumericMatrix& B, ConstRefType a,
                                 NumericMatrix& res)
{
   ASSERT(B.nrows() == res.nrows(), "Bad matrix sizes in a division");
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a division");

   for (size_t i=0; i<B.elems_.size(); ++i)
      res.elems_[i] = TraitsType::div(B.elems_[i], a);
}

template <typename T>
void NumericMatrix<T>::mul(const NumericMatrix& A, const NumericMatrix& B,
                           NumericMatrix& res)
{
   ASSERT(A.nrows() == res.nrows(), "Bad matrix sizes in a multiplication");
   ASSERT(A.ncols() == B.nrows(), "Bad matrix sizes in a multiplication");   
   ASSERT(B.ncols() == res.ncols(), "Bad matrix sizes in a multiplication");

   for (size_t i=0; i<res.nrows(); ++i)
      for (size_t j=0; j<res.ncols(); ++j)
      {
         ValueType s = TraitsType::zero();
         for (size_t k=0; k<A.ncols(); ++k)
            TraitsType::addAssign(s, TraitsType::mul(A(i, k), B(k, j)));

         res(i, j) = s;
      }
}

template <typename T>
NumericMatrix<T>::NumericMatrix(const MatrixType& m) : elems_(m)
{}

template <typename T>
void NumericMatrix<T>::setNrows(size_t nrows)
{
   nrows_ = nrows;
}

template <typename T>
void NumericMatrix<T>::setNcols(size_t ncols)
{
   ncols_ = ncols;
}

template <typename T>
void NumericMatrix<T>::push(ConstRefType x)
{
   elems_.push_back(x);
}

} // namespace

#endif
