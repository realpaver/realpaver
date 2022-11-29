///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_RANGE_HPP
#define REALPAVER_RANGE_HPP

#include "realpaver/Integer.hpp"

namespace realpaver {

class Interval;

///////////////////////////////////////////////////////////////////////////////
/// This is a finite integer interval bounded by safe integers.
///
/// The range called universe is equal to [min, max] where min is the smallest
/// Integer value and max is the greatest Integer value.
///
/// The arithmetic operations on ranges throw an exception in case of underflow
/// or overflow.
///////////////////////////////////////////////////////////////////////////////
class Range {
public:
   /// Type of bounds
   typedef Integer BoundType;

   /// Creates the universe
   Range();

   /// Creates a range reduced to one point
   /// @param a value of this
   Range(const int& a);

   /// Creates a range
   /// @param a left bound
   /// @param b right bound
   Range(const int& a, const int& b);

   /// Creates a range reduced to one point
   /// @param a value of this
   Range(const Integer& a);

   /// Creates a range
   /// @param a left bound
   /// @param b right bound
   Range(const Integer& a, const Integer& b);

   /// Default copy constructor
   Range(const Range&) = default;

   /// Default assignment operator
   Range& operator=(const Range&) = default;

   /// Default destructor
   ~Range() = default;

   ///@{
   /// Construction methods
   static Range lessThan(const Integer& a);
   static Range strictlyLessThan(const Integer& a);
   static Range moreThan(const Integer& a);
   static Range strictlyGreaterThan(const Integer& a);
   ///@}

   /// @return the smallest inite bound
   static Integer MIN();

   /// @return the greatest inite bound
   static Integer MAX();

   /// @return the interval [-inf, +inf]
   static Range universe();

   /// @return the interval [0, +inf]
   static Range positive();

   /// @return the interval [-inf, 0]
   static Range negative();

   /// @reeturn the empty set
   static Range emptyset();

   /// Outward rounding of an interval
   /// @param x an interval
   /// @return the smallest range that contains 'x'
   /// throws an exception if 'x' is not representable
   static Range roundOutward(const Interval& x);

   /// Inward rounding of an interval
   /// @param x an interval
   /// @return the largest range that is contained in 'x'
   /// throws an exception if 'x' is not representable
   static Range roundInward(const Interval& x);

   /// @returns an interval representation of this
   Interval toInterval() const;

   /// assigns the left bound of this
   /// @param a new value
   void setLeft(const Integer& a);

   /// assigns the right bound of this
   /// @param a new value
   void setRight(const Integer& a);

   /// assigns this to the empty set
   void setEmpty();

   /// assigns this to the universe
   void setUniverse();

   /// @return the left bound of this
   Integer left() const;

   /// @return the right bound of this
   Integer right() const;

   /// @return the number of elements in this
   Integer nbElems() const;

   /// @return the midpoint of this
   Integer midpoint() const;

   /// @return the mignitude of this
   Integer mig() const;

   /// @return the magnitude of this
   Integer mag() const;

   /// @return true if this is empty
   bool isEmpty() const;

   /// @return true if this is reduced to one integer
   bool isSingleton() const;

   /// @return true if this is equal to 0
   bool isZero() const;

   ///@{
   /// Set operations
   bool contains(const Integer& a) const;
   bool strictlyContains(const Integer& a) const;
   bool containsZero() const;
   bool strictlyContainsZero() const;
   bool contains(const Range& other) const;
   bool strictlyContains(const Range& other) const;

   bool isSetEq(const Range& other) const;
   bool isSetNeq(const Range& other) const;
   bool isPossiblyEq(const Range& other) const;
   bool isPossiblyNeq(const Range& other) const;
   bool isPossiblyLe(const Range& other) const;
   bool isPossiblyLt(const Range& other) const;
   bool isPossiblyGe(const Range& other) const;
   bool isPossiblyGt(const Range& other) const;

   bool isCertainlyEq(const Range& other) const;
   bool isCertainlyNeq(const Range& other) const;
   bool isCertainlyLe(const Range& other) const;
   bool isCertainlyLt(const Range& other) const;
   bool isCertainlyGe(const Range& other) const;
   bool isCertainlyGt(const Range& other) const;

   bool isDisjoint(const Range& other) const;
   bool overlaps(const Range& other) const;

   bool isCertainlyLeZero() const;
   bool isCertainlyLtZero() const;
   bool isCertainlyGeZero() const;
   bool isCertainlyGtZero() const;
   ///@}

   ///@{
   /// Intersection
   Range& operator&=(const Range& other);
   friend Range operator&(const Range& x, const Range& y);
   ///@}

   ///@{
   /// Hull
   Range& operator|=(const Range& other);
   friend Range operator|(const Range& x, const Range& y);
   ///@}

   ///@{
   /// Addition
   Range& operator+=(const Range& other);
   friend Range operator+(const Range& x, const Range& y);
   friend Range addPX(const Range& x, const Range& y, const Range& z);
   friend Range addPY(const Range& x, const Range& y, const Range& z);
   friend Range addPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Subtraction
   Range& operator-=(const Range& other);
   friend Range operator-(const Range& x, const Range& y);
   friend Range subPX(const Range& x, const Range& y, const Range& z);
   friend Range subPY(const Range& x, const Range& y, const Range& z);
   friend Range subPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Unary subtraction
   friend Range operator-(const Range& x);
   friend Range usubPX(const Range& x, const Range& y);
   friend Range usubPY(const Range& x, const Range& y);
   ///@}

   ///@{
   /// Multiplication
   Range& operator*=(const Range& other);
   friend Range operator*(const Range& x, const Range& y);
   friend Range mulPX(const Range& x, const Range& y, const Range& z);
   friend Range mulPY(const Range& x, const Range& y, const Range& z);
   friend Range mulPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Division
   Range& operator/=(const Range& other);
   friend Range operator/(const Range& x, const Range& y);
   friend Range divPX(const Range& x, const Range& y, const Range& z);
   friend Range divPY(const Range& x, const Range& y, const Range& z);
   friend Range divPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Modulo
   friend Range operator%(const Range& x, int n);
   friend std::pair<Range,Range> extMod(const Range& x, int n);
   friend Range modPX(const Range& x, int n, const Range& y);
   friend Range modPY(const Range& x, int n, const Range& y);
   ///@}

   ///@{
   /// Square
   friend Range sqr(const Range& x);
   friend Range sqrPX(const Range& x, const Range& y);
   friend Range sqrPY(const Range& x, const Range& y);   

   ///@{
   /// Minimum
   friend Range min(const Range& x, const Range& y);
   friend Range minPX(const Range& x, const Range& y, const Range& z);
   friend Range minPY(const Range& x, const Range& y, const Range& z);
   friend Range minPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Maximum
   friend Range max(const Range& x, const Range& y);
   friend Range maxPX(const Range& x, const Range& y, const Range& z);
   friend Range maxPY(const Range& x, const Range& y, const Range& z);
   friend Range maxPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   ///@{
   /// Absolute value
   friend Range abs(const Range& x);
   friend Range absPX(const Range& x, const Range& y);
   friend Range absPY(const Range& x, const Range& y);
   ///@}

   ///@{
   /// Sign
   friend Range sgn(const Range& x);
   friend Range sgnPX(const Range& x, const Range& y);
   friend Range sgnPY(const Range& x, const Range& y);
   ///@}

private:
   Integer l_, r_;
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Range& x);

} // namespace

#endif
