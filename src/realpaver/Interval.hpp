///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_HPP
#define REALPAVER_INTERVAL_HPP

#include <utility>
#include "realpaver/Common.hpp"
#include "realpaver/IntervalImpl.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interval bounded by floats in double precision.
///
/// This class is a wrapper for an arithmetic interval library selected at
/// configuration-time. An interval of this class just encloses an interval
/// of the external library.
///
/// It provides the classical interval operations and the relational
/// operations used to calculate constraint projections.
///
/// For example, let us consider the relation
/// R = { (x, y, z) in R3 : z = x + y, x in Ix, y in Iy, z in Iz }.
/// - addPX(Ix, Iy, Iz) returns the hull of the projection of R on x;
/// - addPY(Ix, Iy, Iz) returns the hull of the projection of R on y;
/// - addPZ(Ix, Iy, Iz) returns the hull of the projection of R on z.
///////////////////////////////////////////////////////////////////////////////
class Interval {
public:
   /// Creates an interval equal to the universe
   Interval();

   /// Creates a point interval
   /// @param a value of the point interval
   Interval(double a);

   /// Creates an interval given its bounds
   /// @param l left bound
   /// @param r right bound
   Interval(double l, double r);

   /// Creates an interval fron a string
   /// @param s string representation of a floating-point number
   Interval(const char* s);

   /// Creates an interval given its bounds
   /// @param sl string representation of the left bound 
   /// @param sr string representation of the right bound 
   Interval(const char* sl, const char* sr);

   /// Default copy constructor
   Interval(const Interval&) = default;

   /// Creates a right-bounded interval
   /// @param a value of the right bound
   static Interval lessThan(double a);

   /// Creates a left-bounded interval
   /// @param a value of the left bound
   static Interval moreThan(double a);

   /// @return the hash code of this
   size_t hashCode() const;

   ///@{
   /// Interval constants
   static Interval universe();
   static Interval positive();
   static Interval negative();
   static Interval emptyset();
   static Interval zero();
   static Interval one();
   static Interval minusOne();
   static Interval minusOnePlusOne();
   static Interval zeroPlusOne();
   static Interval minusOneZero();
   static Interval pi();
   static Interval halfPi();
   static Interval twoPi();
   static Interval minusPiPlusPi();
   static Interval zeroTwoPi();
   ///@}

   ///@return the value of +infinity
   static double infinity();

   /// @return the left bound of this
   double left() const;

   /// return the right bound of this
   double right() const;

   /// Assigns the left bound of this
   /// @param a new value of the left bound
   void setLeft(double a);

   /// Assigns the right bound of this
   /// @param a new value of the right bound
   void setRight(double a);

   /// @return the width of this
   double width() const;

   /// @return the radius of this
   double radius() const;

   /// @return the relative width of this
   double relWidth() const;

   /// @return the midpoint of this
   double midpoint() const;

   /// @return the mignitude of this
   double mig() const;

   /// @return the magnitude of this
   double mag() const;

   /// Assigns this interval to the empty set
   void setEmpty();

   ///@{
   /// Tests
   bool isEmpty() const;
   bool isCanonical() const;
   bool isFinite() const;
   bool isUniverse() const;
   bool isInf() const;
   bool isInfLeft() const;
   bool isInfRight() const;
   bool isSingleton() const;
   bool isZero() const;
   bool isAnInt() const;
   ///@}

   ///@{
   /// Set operations
   bool contains(double a) const;
   bool strictlyContains(double a) const;
   bool contains(const Interval& other) const;
   bool strictlyContains(const Interval& other) const;
   bool isSetEq(const Interval& other) const;
   bool isSetNeq(const Interval& other) const;
   bool isPossiblyEq(const Interval& other) const;
   bool isPossiblyNeq(const Interval& other) const;
   bool isPossiblyLe(const Interval& other) const;
   bool isPossiblyLt(const Interval& other) const;
   bool isPossiblyGe(const Interval& other) const;
   bool isPossiblyGt(const Interval& other) const;
   bool isCertainlyEq(const Interval& other) const;
   bool isCertainlyNeq(const Interval& other) const;
   bool isCertainlyLe(const Interval& other) const;
   bool isCertainlyLt(const Interval& other) const;
   bool isCertainlyGe(const Interval& other) const;
   bool isCertainlyGt(const Interval& other) const;

   bool isCertainlyEqZero() const;
   bool isCertainlyLeZero() const;
   bool isCertainlyLtZero() const;
   bool isCertainlyGeZero() const;
   bool isCertainlyGtZero() const;

   bool isPossiblyEqZero() const;
   bool isPossiblyLeZero() const;
   bool isPossiblyLtZero() const;
   bool isPossiblyGeZero() const;
   bool isPossiblyGtZero() const;

   bool containsZero() const;
   bool strictlyContainsZero() const;
   bool isNegative() const;
   bool isStrictlyNegative() const;
   bool isPositive() const;
   bool isStrictlyPositive() const;
   ///@}

   /// Non intersection test
   /// @param other an interval
   /// @return true if this and other do not overlap
   bool isDisjoint(const Interval& other) const;

   /// Intersection test
   /// @param other an interval
   /// @return true if this and other overlap
   bool overlaps(const Interval& other) const;

   /// Hausdorff distance
   /// @param other an interval
   /// @return the distance between this and other
   double distance(const Interval& other) const;

   ///@{
   /// Intersection
   Interval& operator&=(const Interval& other);
   friend Interval operator&(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Interval hull
   Interval& operator|=(const Interval& other);
   friend Interval operator|(const Interval& x, const Interval& y);
   ///@}

   /// Set complement
   friend std::pair<Interval,Interval> complement(const Interval& x);

   /// Set difference
   friend std::pair<Interval,Interval> setminus(const Interval& x,
                                                const Interval& y);

   /// Rounds an interval to integral bounds
   /// @param x an interval
   /// @return the largest interval of integers included in x
   friend Interval round(const Interval& x);

   /// Inflation method
   /// @param delta a real > 1.0
   /// @param chi a real > 0.0
   /// @return the inflated interval
   ///
   /// Let x be this interval and let m(x) be the midpoint.
   /// Returns m(x) + delta*(x - m(x)) + chi*[-1,1]
   Interval inflate(double delta, double chi) const;

   ///@{
   /// Output on a stream
   friend std::ostream& operator<<(std::ostream& os, const Interval& x);
   static std::streamsize precision(std::streamsize n);
   static std::streamsize precision();
   ///@}

   ///@{
   /// Addition
   Interval& operator+=(const Interval& other);
   friend Interval operator+(const Interval& x, const Interval& y);

   friend Interval addPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval addPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval addPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Subtraction
   Interval& operator-=(const Interval& other);
   friend Interval operator-(const Interval& x, const Interval& y);

   friend Interval subPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval subPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval subPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Unary subtraction
   friend Interval operator-(const Interval& x);

   friend Interval usubPX(const Interval& x, const Interval& y);
   friend Interval usubPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Multiplication
   Interval& operator*=(const Interval& other);
   friend Interval operator*(const Interval& x, const Interval& y);

   friend Interval mulPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval mulPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval mulPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Division
   Interval& operator/=(const Interval& other);
   friend Interval operator/(const Interval& x, const Interval& y);

   friend std::pair<Interval,Interval> extDiv(const Interval& x,
                                              const Interval& y);

   friend Interval divPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval divPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval divPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Square
   friend Interval sqr(const Interval& x);

   friend Interval sqrPX(const Interval& x, const Interval& y);
   friend Interval sqrPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Square root
   friend Interval sqrt(const Interval& x);

   friend Interval sqrtPX(const Interval& x, const Interval& y);
   friend Interval sqrtPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Power function
   friend Interval pow(const Interval& x, int n);

   friend Interval powPX(const Interval& x, int n, const Interval& y);
   friend Interval powPY(const Interval& x, int n, const Interval& y);
   ///@}

   ///@{
   /// Exponential
   friend Interval exp(const Interval& x);

   friend Interval expPX(const Interval& x, const Interval& y);
   friend Interval expPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Logarithm
   friend Interval log(const Interval& x);

   friend Interval logPX(const Interval& x, const Interval& y);
   friend Interval logPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Sine
   friend Interval sin(const Interval& x);

   friend Interval sinPX(const Interval& x, const Interval& y);
   friend Interval sinPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Cosine
   friend Interval cos(const Interval& x);

   friend Interval cosPX(const Interval& x, const Interval& y);
   friend Interval cosPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Tangent
   friend Interval tan(const Interval& x);
   friend Interval tanPX(const Interval& x, const Interval& y);
   friend Interval tanPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Absolute value
   friend Interval abs(const Interval& x);

   friend Interval absPX(const Interval& x, const Interval& y);
   friend Interval absPY(const Interval& x, const Interval& y);
   ///@}

   ///@{
   /// Minimum function
   friend Interval min(const Interval& x, const Interval& y);

   friend Interval minPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval minPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval minPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Maximum function
   friend Interval max(const Interval& x, const Interval& y);

   friend Interval maxPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval maxPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval maxPZ(const Interval& x, const Interval& y,
                         const Interval& z);
   ///@}

   ///@{
   /// Sign function
   friend Interval sgn(const Interval& x);

   friend Interval sgnPX(const Interval& x, const Interval& y);
   friend Interval sgnPY(const Interval& x, const Interval& y);   
   ///@}

private:
   typedef IntervalTraits<RawInterval> Traits;
   RawInterval impl_;    // interval enclosed

   /// Creates an interval
   /// @param x raw interval enclosed in this
   Interval(const Interval::Traits::interval& x);
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Interval& x);

} // namespace

#endif
