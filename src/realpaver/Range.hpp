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
 * @file   Range.hpp
 * @brief  Finite integer interval bounded by safe integers
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#ifndef REALPAVER_RANGE_HPP
#define REALPAVER_RANGE_HPP

#include "realpaver/Integer.hpp"

namespace realpaver {

class Interval;

/**
 * @brief Finite integer interval bounded by safe integers.
 *
 * The range called universe is equal to [min, max] where min is the smallest
 * Integer value and max is the greatest Integer value.
 *
 * The arithmetic operations on ranges throw an exception in case of underflow
 * or overflow.
 */
class Range {
public:
   /// Type of bounds
   using BoundType = Integer;

   /// @name Constructors
   ///@{

   /// Creates the universe
   Range();

   /// Creates the range [a, a]
   Range(const int& a);

   /// Creates the range [a, b]
   Range(const int& a, const int& b);

   /// Creates the range [a, a]
   Range(const Integer& a);

   /// Creates the range [a, b]
   Range(const Integer& a, const Integer& b);

   /// Default copy constructor
   Range(const Range&) = default;

   /// Default assignment operator
   Range& operator=(const Range&) = default;

   /// Default destructor
   ~Range() = default;

   /// Returns the range [MIN(), a]
   static Range lessThan(const Integer& a);

   /// Returns the range [MIN(), a-1]
   static Range strictlyLessThan(const Integer& a);

   /// Returns the range [a, MAX()]
   static Range moreThan(const Integer& a);

   /// Returns the range [a+1, MAX()]
   static Range strictlyGreaterThan(const Integer& a);
   ///@}

   /// Returns the smallest finite bound
   static Integer MIN();

   /// Returns the greatest finite bound
   static Integer MAX();

   /// Returns the interval [MIN(), MAX()]
   static Range universe();

   /// Returns the interval [0, MAX()]
   static Range positive();

   /// Returns the interval [MIN(), 0]
   static Range negative();

   /// Returns the empty set
   static Range emptyset();

   /**
    * @brief Outward rounding of an interval.
    * 
    * Returns the smallest range that contains x; throws an exception if x is
    * not representable
    */
   static Range roundOutward(const Interval& x);

   /**
    * @brief Inward rounding of an interval.
    * 
    * Returns the largest range that is contained in x; throws an exception if
    * x is not representable
    */
   static Range roundInward(const Interval& x);

   /// Returns an interval representation of this
   Interval toInterval() const;

   /// Assigns the left bound of this
   void setLeft(const Integer& a);

   /// Assigns the right bound of this
   void setRight(const Integer& a);

   /// Assigns this to the empty set
   void setEmpty();

   /// Assigns this to the universe
   void setUniverse();

   /// Returns the left bound of this
   Integer left() const;

   /// @return the right bound of this
   Integer right() const;

   /// Returns the number of elements in this
   unsigned long nbElems() const;

   /// Returns the midpoint of this
   Integer midpoint() const;

   /// Returns the mignitude of this
   Integer mig() const;

   /// Returns the magnitude of this
   Integer mag() const;

   /// Returns true if this is empty
   bool isEmpty() const;

   /// Returns true if this is reduced to one integer
   bool isSingleton() const;

   /// Returns true if this is equal to 0
   bool isZero() const;

   /// @name Set operations
   ///@{
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

   /**
    * @brief Returns true if (this union other) = hull(this union other)
    *        and if this and other are non empty.
    */
   bool isJoinable(const Range& other) const;

   bool isCertainlyLeZero() const;
   bool isCertainlyLtZero() const;
   bool isCertainlyGeZero() const;
   bool isCertainlyGtZero() const;
   ///@}

   /// @name Intersection
   ///@{
   Range& operator&=(const Range& other);
   friend Range operator&(const Range& x, const Range& y);
   ///@}

   /// @name Hull
   ///@{
   Range& operator|=(const Range& other);
   friend Range operator|(const Range& x, const Range& y);
   ///@}

   /// @name Addition
   ///@{
   Range& operator+=(const Range& other);
   friend Range operator+(const Range& x, const Range& y);
   friend Range addPX(const Range& x, const Range& y, const Range& z);
   friend Range addPY(const Range& x, const Range& y, const Range& z);
   friend Range addPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Subtraction
   ///@{
   Range& operator-=(const Range& other);
   friend Range operator-(const Range& x, const Range& y);
   friend Range subPX(const Range& x, const Range& y, const Range& z);
   friend Range subPY(const Range& x, const Range& y, const Range& z);
   friend Range subPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Unary subtraction
   ///@{
   friend Range operator-(const Range& x);
   friend Range usubPX(const Range& x, const Range& y);
   friend Range usubPY(const Range& x, const Range& y);
   ///@}

   /// @name Multiplication
   ///@{
   Range& operator*=(const Range& other);
   friend Range operator*(const Range& x, const Range& y);
   friend Range mulPX(const Range& x, const Range& y, const Range& z);
   friend Range mulPY(const Range& x, const Range& y, const Range& z);
   friend Range mulPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Division
   ///@{
   Range& operator/=(const Range& other);
   friend Range operator/(const Range& x, const Range& y);
   friend Range divPX(const Range& x, const Range& y, const Range& z);
   friend Range divPY(const Range& x, const Range& y, const Range& z);
   friend Range divPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Modulo
   ///@{
   friend Range operator%(const Range& x, int n);
   friend std::pair<Range,Range> extMod(const Range& x, int n);
   friend Range modPX(const Range& x, int n, const Range& y);
   friend Range modPY(const Range& x, int n, const Range& y);
   ///@}

   /// @name Square
   ///@{
   friend Range sqr(const Range& x);
   friend Range sqrPX(const Range& x, const Range& y);
   friend Range sqrPY(const Range& x, const Range& y);   

   /// @name Minimum
   ///@{
   friend Range min(const Range& x, const Range& y);
   friend Range minPX(const Range& x, const Range& y, const Range& z);
   friend Range minPY(const Range& x, const Range& y, const Range& z);
   friend Range minPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Maximum
   ///@{
   friend Range max(const Range& x, const Range& y);
   friend Range maxPX(const Range& x, const Range& y, const Range& z);
   friend Range maxPY(const Range& x, const Range& y, const Range& z);
   friend Range maxPZ(const Range& x, const Range& y, const Range& z);
   ///@}

   /// @name Absolute value
   ///@{
   friend Range abs(const Range& x);
   friend Range absPX(const Range& x, const Range& y);
   friend Range absPY(const Range& x, const Range& y);
   ///@}

   /// @name Sign
   ///@{
   friend Range sgn(const Range& x);
   friend Range sgnPX(const Range& x, const Range& y);
   friend Range sgnPY(const Range& x, const Range& y);
   ///@}

private:
   Integer l_, r_;   // bounds
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Range& x);

} // namespace

#endif
