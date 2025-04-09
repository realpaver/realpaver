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
 * @file   NumericVector.hpp
 * @brief  Numeric vector
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_NUMERIC_VECTOR_HPP
#define REALPAVER_NUMERIC_VECTOR_HPP

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/NumericTraits.hpp"
#include <cwchar>
#include <iostream>
#include <vector>

namespace realpaver {

/// Numeric vector of elements of type T
template <typename T> class NumericVector {
protected:
   /// Traits class
   using TraitsType = NumericTraits<T>;

public:
   /// Value type
   using ValueType = T;

   /// Pointer type
   using PointerType = T *;

   /// Constant pointer type
   using ConstPointerType = const T *;

   /// Reference type
   using RefType = T &;

   /// Const reference type
   using ConstRefType = const T &;

   /// Constructor
   NumericVector(size_t size = 0);

   /// Constructor that initializes all the elements to init
   NumericVector(size_t size, ConstRefType init);

   /// Default copy constructor
   NumericVector(const NumericVector &) = default;

   /// Default assignment operator
   NumericVector &operator=(const NumericVector &) = default;

   /// Virtual destructor
   virtual ~NumericVector();

   /// Returns the size of this
   size_t size() const;

   /// Resizes the vector given the new size n
   void resize(size_t n);

   /// Returns the i-th element of this
   ValueType operator[](size_t i) const;

   /// Returns a reference to the i-th element of this
   RefType operator[](size_t i);

   /// Assigns all the element of this to x
   void setAll(ConstRefType x);

   /// Assigns this to V
   void setAll(const NumericVector &V);

   /// Assigns this from an array
   void setArray(ConstPointerType array);

   /// Inserts x at the end of this
   void push(const T &x);

   /// Output on a stream
   virtual void print(std::ostream &os) const;

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns true if one element of this in infinite
   bool isInf() const;

   /// Returns true if all the elements of this are finite
   bool isFinite() const;

   /// Returns a pointer to the data array
   ValueType *data();

   /// Assigns res to V + W
   static void add(const NumericVector &V, const NumericVector &W, NumericVector &res);

   /// Assigns res to V - W
   static void sub(const NumericVector &V, const NumericVector &W, NumericVector &res);

   /// Assigns res to -V
   static void usb(const NumericVector &V, NumericVector &res);

   /// Assigns res to a * V
   static void mulScalar(ConstRefType a, const NumericVector &V, NumericVector &res);

   /// Assigns res to V / a
   static void divScalar(const NumericVector &V, ConstRefType a, NumericVector &res);

private:
   typedef std::vector<ValueType> VectorType;
   VectorType elems_;

   NumericVector(const VectorType &v);
};

template <typename T>
NumericVector<T>::NumericVector(size_t size)
    : elems_(size)
{
}

template <typename T>
NumericVector<T>::NumericVector(size_t size, ConstRefType init)
    : elems_(size)
{
   for (size_t i = 0; i < size; ++i)
      elems_[i] = init;
}

template <typename T> NumericVector<T>::~NumericVector()
{
}

template <typename T> size_t NumericVector<T>::size() const
{
   return elems_.size();
}

template <typename T> void NumericVector<T>::resize(size_t n)
{
   elems_.resize(n);
}

template <typename T>
typename NumericVector<T>::ValueType NumericVector<T>::operator[](size_t i) const
{
   ASSERT(i < size(), "Bad access in a vector at index " << i);

   return elems_[i];
}

template <typename T>
typename NumericVector<T>::RefType NumericVector<T>::operator[](size_t i)
{
   ASSERT(i < size(), "Bad access in a vector at index " << i);

   return elems_[i];
}

template <typename T> void NumericVector<T>::setAll(ConstRefType x)
{
   for (size_t i = 0; i < elems_.size(); ++i)
      elems_[i] = x;
}

template <typename T> void NumericVector<T>::setAll(const NumericVector &V)
{
   ASSERT(size() == V.size(), "Bad assignemnt of numeric vectors");

   for (size_t i = 0; i < elems_.size(); ++i)
      elems_[i] = V.elems_[i];
}

template <typename T> void NumericVector<T>::setArray(ConstPointerType array)
{
   for (size_t i = 0; i < elems_.size(); ++i)
      elems_[i] = array[i];
}

template <typename T> void NumericVector<T>::push(const T &x)
{
   elems_.push_back(x);
}

template <typename T>
NumericVector<T>::NumericVector(const VectorType &v)
    : elems_(v)
{
}

template <typename T> void NumericVector<T>::print(std::ostream &os) const
{
   os << '(';
   for (size_t i = 0; i < size(); ++i)
   {
      if (i != 0)
         os << " ; ";
      os << operator[](i);
   }
   os << ')';
}

template <typename T> size_t NumericVector<T>::hashCode() const
{
   size_t h = 0;

   if (size() != 0)
   {
      h = TraitsType::hashCode(elems_[0]);

      for (size_t i = 1; i < size(); ++i)
         h = hash2(h, TraitsType::hashCode(elems_[i]));
   }

   return h;
}

template <typename T> bool NumericVector<T>::isInf() const
{
   for (size_t i = 0; i < size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return true;

   return false;
}

template <typename T> bool NumericVector<T>::isFinite() const
{
   for (size_t i = 0; i < size(); ++i)
      if (TraitsType::isInf(elems_[i]))
         return false;

   return true;
}

template <typename T> typename NumericVector<T>::ValueType *NumericVector<T>::data()
{
   return elems_.data();
}

template <typename T>
void NumericVector<T>::add(const NumericVector &V, const NumericVector &W,
                           NumericVector &res)
{
   ASSERT(V.size() == W.size(), "Bad vector sizes in an addition");
   ASSERT(V.size() == res.size(), "Bad vector sizes in an addition");

   for (size_t i = 0; i < V.size(); ++i)
      res.elems_[i] = TraitsType::add(V.elems_[i], W.elems_[i]);
}

template <typename T>
void NumericVector<T>::sub(const NumericVector &V, const NumericVector &W,
                           NumericVector &res)
{
   ASSERT(V.size() == W.size(), "Bad vector sizes in a subtraction");
   ASSERT(V.size() == res.size(), "Bad vector sizes in a subtraction");

   for (size_t i = 0; i < V.size(); ++i)
      res.elems_[i] = TraitsType::sub(V.elems_[i], W.elems_[i]);
}

template <typename T>
void NumericVector<T>::usb(const NumericVector &V, NumericVector &res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a subtraction");

   for (size_t i = 0; i < V.size(); ++i)
      res.elems_[i] = TraitsType::usb(V.elems_[i]);
}

template <typename T>
void NumericVector<T>::mulScalar(ConstRefType a, const NumericVector &V,
                                 NumericVector &res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a multiplication");

   for (size_t i = 0; i < V.size(); ++i)
      res.elems_[i] = TraitsType::mul(a, V.elems_[i]);
}

template <typename T>
void NumericVector<T>::divScalar(const NumericVector &V, ConstRefType a,
                                 NumericVector &res)
{
   ASSERT(V.size() == res.size(), "Bad vector sizes in a multiplication");

   for (size_t i = 0; i < V.size(); ++i)
      res.elems_[i] = TraitsType::div(a, V.elems_[i]);
}

} // namespace realpaver

#endif
