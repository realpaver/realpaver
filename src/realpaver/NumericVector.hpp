///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_NUMERIC_VECTOR_HPP
#define REALPAVER_NUMERIC_VECTOR_HPP

#include <iostream>
#include <vector>
#include "AssertDebug.hpp"
#include "Common.hpp"
#include "LinumTraits.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a numeric vector of elements of type T.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class NumericVector {
protected:
   /// Traits class
  typedef LinearTraits<T> TraitsType;
      
public:
   /// Value type
   typedef T ValueType;

   /// Reference type
   typedef T& RefType;

   /// Const reference type
   typedef const T& ConstRefType;

   /// Creates a vector of size zero
   NumericVector();

   /// Creates a vector of a given size
   /// @param size number of elements of this
   NumericVector(size_t size);

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

   /// Constant access in this
   /// @param i an index between 0 and size()-1
   /// @return the i-th element of this
   ValueType at(size_t i) const;

   /// Modification of an element of this
   /// @param i an index between 0 and size()-1
   /// @param x a number
   ///
   /// Assigns x to the i-th element of this
  void setAt(size_t i, ConstRefType x);

   /// Modification of all the element of this
   /// @param x a number
   ///
   /// Assigns x to all the element of this
   void setAll(ConstRefType x);

   /// Insertion of one element at the end of this
   /// @param x a number inserted at the end
   void push(const T& x);

   /// Addition with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this[i] += V[i] for each i
   NumericVector& operator+=(const NumericVector& V);

   /// Subtraction with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this[i] -= V[i] for each i
   NumericVector& operator-=(const NumericVector& V);

   /// Multiplication with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this[i] *= V[i] for each i
   NumericVector& operator*=(const NumericVector& V);

   /// Multiplication with assignment
   /// @param x a number
   /// @return a reference to this
   ///
   /// this[i] *= x for each i
   NumericVector& operator*=(ConstRefType x);

   /// Division with assignment
   /// @param V a vector
   /// @return a reference to this
   ///
   /// this[i] /= V[i] for each i
   NumericVector& operator/=(const NumericVector& V);

   /// Multiplication with assignment
   /// @param x a number
   /// @return a reference to this
   ///
   /// this[i] /= x for each i
   NumericVector& operator/=(ConstRefType x);

   /// Addition
   /// @param V a vector
   /// @return this + V
   NumericVector operator+(const NumericVector& V) const;

   /// Subtraction
   /// @param V a vector
   /// @return this - V
   NumericVector operator-(const NumericVector& V) const;

   /// Unary subtraction
   /// @return -this
   NumericVector operator-() const;

   /// Multiplication
   /// @param V a vector
   /// @return this * V
   NumericVector operator*(const NumericVector& V) const;

   /// Multiplication
   /// @param x a number
   /// @return this * x
   NumericVector operator*(ConstRefType x) const;

   /// Division
   /// @param x a number
   /// @return this / x
   NumericVector operator/(ConstRefType x) const;

   /// Extracts a sub-vector from this
   /// @param i an index of this
   /// @param j an index of this
   /// @return the sub-vector of this between i and j
   NumericVector subVector(size_t i, size_t j) const;

   /// Output on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const;

   /// @return the hash code of this
   size_t hashCode() const;

   /// @return true if one element of this in infinite
   bool isInf() const;

   /// @return true if all the elements of this are finite
   bool isFinite() const;

private:
   typedef std::vector<ValueType> VectorType;
   VectorType elems_;

   NumericVector(const VectorType& v);
};

template <typename T>
NumericVector<T>::NumericVector() :  elems_()
{}

template <typename T>
NumericVector<T>::NumericVector(size_t size) : elems_(size)
{
   ASSERT(size > 0, "Initialization of a vector with a null size");
}

template <typename T>
NumericVector<T>::NumericVector(size_t size, ConstRefType init) : elems_(size)
{
   ASSERT(size > 0, "Initialization of a vector with a null size");

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
typename NumericVector<T>::ValueType
NumericVector<T>::at(size_t i) const
{
   ASSERT(i<size(), "Bad access in a vector at index: " << i);

   return elems_[i];
}

template <typename T>
void
NumericVector<T>::setAt(size_t i, ConstRefType x)
{
   ASSERT(i<size(), "Bad access in a vector at index: " << i);

   elems_[i] = x;
}

template <typename T>
void
NumericVector<T>::setAll(ConstRefType x)
{
   for (size_t i=0; i<elems_.size(); ++i)
      elems_[i] = x;
}

template <typename T>
void NumericVector<T>::push(const T& x)
{
   elems_.push_back(x);
}
  
template <typename T>
NumericVector<T>&
NumericVector<T>::operator+=(const NumericVector& V)
{
   ASSERT(size() == V.size(), "Addition of vectors having different sizes");

   for (size_t i=0; i<elems_.size(); ++i)
      TraitsType::addEq(elems_[i], V.elems_[i]);

   return *this;
}

template <typename T>
NumericVector<T>&
NumericVector<T>::operator-=(const NumericVector& V)
{
   ASSERT(size() == V.size(), "Subtraction of vectors having different sizes");

   for (size_t i=0; i<elems_.size(); ++i)
      TraitsType::subEq(elems_[i], V.elems_[i]);

   return *this;
}

template <typename T>
NumericVector<T>&
NumericVector<T>::operator*=(const NumericVector& V)
{
   ASSERT(size() == V.size(),
          "Multiplication of vectors having different sizes");

   for (size_t i=0; i<elems_.size(); ++i)
      TraitsType::mulEq(elems_[i], V.elems_[i]);

  return *this;
}

template <typename T>
NumericVector<T>&
NumericVector<T>::operator*=(ConstRefType x)
{
   ASSERT(!TraitsType::isNan(x),
		    "Product of a vector with an invalid number: " << x);

   for (size_t i=0; i<elems_.size(); ++i)
      TraitsType::mulEq(elems_[i], x);

   return *this;
}

template <typename T>
NumericVector<T>&
NumericVector<T>::operator/=(const NumericVector& V)
{
  ASSERT(size() == V.size(),
         "Division of vectors having different sizes");

   for (size_t i=0; i<elems_.size(); ++i)
   {
      ASSERT( V.elems_[i] != TraitsType::zero(),
             "Division of a vector by another vector " <<
             "which contains a zero" );

      TraitsType::divEq(elems_[i], V.elems_[i]);
   }

   return *this;
}

template <typename T>
NumericVector<T>&
NumericVector<T>::operator/=(ConstRefType x)
{
   ASSERT((!TraitsType::isNan(x)) && (x!=TraitsType::zero()),
          "Division of a vector by an invalid number: " << x);

   for (size_t i=0; i<elems_.size(); ++i)
      TraitsType::divEq(elems_[i], x);

   return *this;
}

template <typename T>
NumericVector<T>
NumericVector<T>::operator+(const NumericVector& V) const
{
   NumericVector result(*this);
   result += V;
   return result;
}

template <typename T>
NumericVector<T>
NumericVector<T>::operator-(const NumericVector& V) const
{
   NumericVector result(*this);
   result -= V;
   return result;
}

template <typename T>
NumericVector<T>
NumericVector<T>::operator-() const
{
   NumericVector result(size(), TraitsType::zero());
   result -= *this;
   return result;
}

template <typename T>
NumericVector<T>
NumericVector<T>::operator*(const NumericVector& V) const
{
   NumericVector result(*this);
   result *= V;
   return result;
}
      
template <typename T>
NumericVector<T>
NumericVector<T>::operator*(ConstRefType x) const
{
   NumericVector result(*this);
   result *= x;
   return result;
}

template <typename T>
NumericVector<T>
NumericVector<T>::operator/(ConstRefType x) const
{
   NumericVector result(*this);
   result /= x;
   return result;
}
   
template <typename T>
NumericVector<T>
NumericVector<T>::subVector(size_t i, size_t j) const
{
   ASSERT((j>=i) && (j<size()), "sub-vector not defined: wrong indexes");

   typename VectorType::const_iterator pi = elems_.begin();
   pi += i;

   typename VectorType::const_iterator pj = elems_.begin();
   pj += (j+1);

   return NumericVector( VectorType(pi, pj) );
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
      os << at(i);
   }
   os << ')'; 
}

template <typename T>
std::ostream&
operator<<(std::ostream& os, const NumericVector<T>& V)
{
   V.print(os);
   return os;
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

} // namespace

#endif
