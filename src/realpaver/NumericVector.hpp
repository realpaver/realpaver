///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NUMERIC_VECTOR_HPP
#define REALPAVER_NUMERIC_VECTOR_HPP

#include <iostream>
#include <vector>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/NumericTraits.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a numeric vector of elements of type T.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class NumericVector {
protected:
   /// Traits class
  typedef NumericTraits<T> TraitsType;
      
public:
   /// Value type
   typedef T ValueType;

   /// Pointer type
   typedef T* PointerType;

   /// Constant pointer type
   typedef const T* ConstPointerType;

   /// Reference type
   typedef T& RefType;

   /// Const reference type
   typedef const T& ConstRefType;

   /// Creates a vector of a given size
   /// @param size number of elements of this
   NumericVector(size_t size = 0);

   /// Creates a vector and initializes its elements
   NumericVector(size_t size, ConstRefType init);

   /// Default copy constructor
   NumericVector(const NumericVector&) = default;

   /// Default assignment operator
   NumericVector& operator=(const NumericVector&) = default;

   /// Virtual destructor
   virtual ~NumericVector();

   /// @return the size of this
   size_t size() const;

   /// Resizes the vector
   /// @param n new size of this
   void resize(size_t n);

   /// Constant access in this
   /// @param i an index between 0 and size()-1
   /// @return the i-th element of this
   ValueType operator[](size_t i) const;

   /// Non constant access in this
   /// @param i an index between 0 and size()-1
   /// @return a reference to the i-th element of this
   RefType operator[](size_t i);

   /// Modification of all the elements of this
   /// @param x a number
   ///
   /// Assigns x to all the element of this
   void setAll(ConstRefType x);

   /// Assigns another vector to this
   /// @param V a vector having the same size than this
   void setAll(const NumericVector& V);

   /// Asigns this from an array
   /// @param array array of values to be assigned to this
   void setArray(ConstPointerType array);

   /// Insertion of one element at the end of this
   /// @param x a number inserted at the end
   void push(const T& x);

   /// Output on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const;

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return true if one element of this in infinite
   bool isInf() const;

   /// @return true if all the elements of this are finite
   bool isFinite() const;

   /// Addition
   /// @param V a vector
   /// @param W a vector
   /// @param res result assigned to V + W
   static void add(const NumericVector& V, const NumericVector& W,
                   NumericVector& res);

   /// Subtraction
   /// @param V a vector
   /// @param W a vector
   /// @param res result assigned to V - W
   static void sub(const NumericVector& V, const NumericVector& W,
                   NumericVector& res);

   /// Unary subtraction
   /// @param V a vector
   /// @param res result assigned to - V
   static void usb(const NumericVector& V, NumericVector& res);

   /// Multiplication
   /// @param a a scalar
   /// @param V a vector
   /// @param res result assigned to a * V
   static void mulScalar(ConstRefType a, const NumericVector& V,
                         NumericVector& res);

   /// Division
   /// @param V a vector
   /// @param a a scalar
   /// @param res result assigned to V / a
   static void divScalar(const NumericVector& V, ConstRefType a,
                         NumericVector& res);

private:
   typedef std::vector<ValueType> VectorType;
   VectorType elems_;

   NumericVector(const VectorType& v);
};

template <typename T>
NumericVector<T>::NumericVector(size_t size) : elems_(size)
{}

template <typename T>
NumericVector<T>::NumericVector(size_t size, ConstRefType init) : elems_(size)
{
   for (size_t i=0; i<size; ++i)
      elems_[i] = init;
}
   
template <typename T>
NumericVector<T>::~NumericVector()
{}

template <typename T>
size_t NumericVector<T>::size() const
{
   return elems_.size();
}

template <typename T>
void NumericVector<T>::resize(size_t n)
{
   elems_.resize(n);
}

template <typename T>
typename NumericVector<T>::ValueType
NumericVector<T>::operator[](size_t i) const
{
   ASSERT(i<size(), "Bad access in a vector at index " << i);

   return elems_[i];
}

template <typename T>
typename NumericVector<T>::RefType
NumericVector<T>::operator[](size_t i)
{
   ASSERT(i<size(), "Bad access in a vector at index " << i);

   return elems_[i];
}

template <typename T>
void
NumericVector<T>::setAll(ConstRefType x)
{
   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = x;
}

template <typename T>
void
NumericVector<T>::setAll(const NumericVector& V)
{
   ASSERT(size() == V.size(), "Bad assignemnt of numeric vectors");

   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = V.elems_[i];
}

template <typename T>
void
NumericVector<T>::setArray(ConstPointerType array)
{
   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = array[i];
}

template <typename T>
void NumericVector<T>::push(const T& x)
{
   elems_.push_back(x);
}
   
template <typename T>
NumericVector<T>::NumericVector(const VectorType& v) : elems_(v)
{}

template <typename T>
void NumericVector<T>::print(std::ostream& os) const
{
   os << '(';
   for (size_t i=0; i<size(); ++i)
   {
      if (i!=0) os << ", ";
      os << operator[](i);
   }
   os << ')'; 
}

template <typename T>
size_t NumericVector<T>::hashCode() const
{
   size_t h = 0;

   if (size() != 0)
   {
      h = TraitsType::hashCode(elems_[0]);

      for (size_t i=1; i<size(); ++i)
         h = hash2(h, TraitsType::hashCode(elems_[i]));
   }

   return h;
}
   
template <typename T>
bool NumericVector<T>::isInf() const
{
   for (size_t i=0; i<size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return true;

   return false;
}

template <typename T>
bool NumericVector<T>::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return false;

   return true;
}

template <typename T>
void NumericVector<T>::add(const NumericVector& V, const NumericVector& W,
                           NumericVector& res)
{
   ASSERT(V.size() == W.size(), "Bad vector sizes in an addition");
   ASSERT(V.size() == res.size(), "Bad vector sizes in an addition");

   for (size_t i=0; i<V.size(); ++i)
      res.elems_[i] = TraitsType::add(V.elems_[i], W.elems_[i]);
}

template <typename T>
void NumericVector<T>::sub(const NumericVector& V, const NumericVector& W,
                           NumericVector& res)
{
   ASSERT(V.size() == W.size(), "Bad vector sizes in a subtraction");
   ASSERT(V.size() == res.size(), "Bad vector sizes in a subtraction");

   for (size_t i=0; i<V.size(); ++i)
      res.elems_[i] = TraitsType::sub(V.elems_[i], W.elems_[i]);
}

template <typename T>
void NumericVector<T>::usb(const NumericVector& V, NumericVector& res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a subtraction");

   for (size_t i=0; i<V.size(); ++i)
      res.elems_[i] = TraitsType::usb(V.elems_[i]);
}

template <typename T>
void NumericVector<T>::mulScalar(ConstRefType a, const NumericVector& V,
                                 NumericVector& res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a multiplication");

   for (size_t i=0; i<V.size(); ++i)
      res.elems_[i] = TraitsType::mul(a, V.elems_[i]);
}

template <typename T>
void NumericVector<T>::divScalar(const NumericVector& V, ConstRefType a,
                                 NumericVector& res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a multiplication");

   for (size_t i=0; i<V.size(); ++i)
      res.elems_[i] = TraitsType::div(a, V.elems_[i]);
}

} // namespace

#endif
