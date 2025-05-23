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
 * @file   Interval.hpp
 * @brief  Class of intervals
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_INTERVAL_HPP
#define REALPAVER_INTERVAL_HPP

#include "realpaver/Common.hpp"
#include "realpaver/IntervalImpl.hpp"
#include <string>
#include <utility>

namespace realpaver {

/**
 * @brief Interval bounded by floats in double precision
 *
 * This class is a wrapper for an arithmetic interval library selected at
 * configuration-time. An interval of this class just encloses an interval
 * of the external library.
 *
 * It provides the classical interval operations and the relational
 * operations used to calculate constraint projections.
 *
 * For example, let us consider the relation
 * R = { (x, y, z) in R3 : z = x + y, x in Ix, y in Iy, z in Iz }.
 * - addPX(Ix, Iy, Iz) returns the hull of the projection of R on x;
 * - addPY(Ix, Iy, Iz) returns the hull of the projection of R on y;
 * - addPZ(Ix, Iy, Iz) returns the hull of the projection of R on z.
 */
class Interval {
public:
   /// @name Constructors
   ///@{

   /// Creates [-oo, +oo]
   Interval();

   /// Creates [a, a]
   Interval(double a);

   /// Creates [l, r]
   Interval(double l, double r);

   /// Creates [s rounded downward, s rounded upward]
   Interval(const std::string &s);

   /// Creates [sl rounded downward, sr rounded upward]
   Interval(const std::string &sl, const std::string &sr);

   /// Creates [s rounded downward, s rounded upward]
   Interval(const char *s);

   /// Creates [sl rounded downward, sr rounded upward]
   Interval(const char *sl, const char *sr);

   /// Default copy constructor
   Interval(const Interval &) = default;
   ///@}

   /// Creates [-oo, a]
   static Interval lessThan(double a);

   /// Creates [a, +oo]
   static Interval moreThan(double a);

   /// Returns the hash code of this
   size_t hashCode() const;

   /** @name Interval constants */
   /** @{ */
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
   static Interval minusPiZero();
   static Interval minusPiPlusPi();
   static Interval minusHalfPiPlusHalfPi();
   static Interval zeroPi();
   static Interval zeroTwoPi();
   /** @} */

   /// Returns the value of +infinity
   static double infinity();

   /// Returns the left bound of this
   double left() const;

   /// Returns the right bound of this
   double right() const;

   /// Assigns the left bound of this
   void setLeft(double a);

   /// Assigns the right bound of this
   void setRight(double a);

   /// Returns the width of this
   double width() const;

   /// Returns the radius of this
   double radius() const;

   /// Returns the relative width of this
   double relWidth() const;

   /// Returns the midpoint of this
   double midpoint() const;

   /// Returns the mignitude of this
   double mig() const;

   /// Returns the magnitude of this
   double mag() const;

   /// Assigns this interval to the empty set
   void setEmpty();

   /// @name Tests
   ///@{
   bool isEmpty() const;
   bool isCanonical() const;
   bool isFinite() const;
   bool isUniverse() const;
   bool isInf() const;
   bool isInfLeft() const;
   bool isInfRight() const;
   bool isSingleton() const;
   bool isZero() const;
   bool isOne() const;
   bool isAnInt() const;
   ///@}

   /// @name Set operations
   ///@{
   bool contains(double a) const;
   bool strictlyContains(double a) const;
   bool contains(const Interval &other) const;
   bool strictlyContains(const Interval &other) const;
   bool isSetEq(const Interval &other) const;
   bool isSetNeq(const Interval &other) const;
   bool isPossiblyEq(const Interval &other) const;
   bool isPossiblyNeq(const Interval &other) const;
   bool isPossiblyLe(const Interval &other) const;
   bool isPossiblyLt(const Interval &other) const;
   bool isPossiblyGe(const Interval &other) const;
   bool isPossiblyGt(const Interval &other) const;
   bool isCertainlyEq(const Interval &other) const;
   bool isCertainlyNeq(const Interval &other) const;
   bool isCertainlyLe(const Interval &other) const;
   bool isCertainlyLt(const Interval &other) const;
   bool isCertainlyGe(const Interval &other) const;
   bool isCertainlyGt(const Interval &other) const;

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
   bool containskPi() const;
   bool containsHalfPiPluskPi() const;
   ///@}

   /// Returns true if this and other do not overlap
   bool isDisjoint(const Interval &other) const;

   /// Returns true if this and other overlap
   bool overlaps(const Interval &other) const;

   /**
    * @brief Returns the Hausdorff distance between this and other.
    *
    * Given this = [a, b] and other = [c, d], the distance is defined by
    * max(|a-c|, |b-d|)
    */
   double distance(const Interval &other) const;

   /**
    * @brief Returns the Gap between this and other.
    *
    * Given this = [a, b] and other = [c, d], the gap is equal to:
    * - +oo if an interval is empty
    * - 0.0 if both intervals overlap
    * - c-b if c>b
    * - a-d if a>d
    */
   double gap(const Interval &other) const;

   /**
    * @brief Returns the couple (midpoint, radius) of this.
    *
    * Returns (m, r) where m is the midpoint of this and r is its radius
    * such that [m-r, m+r] contains this, if this is non empty; (NaN, NaN)
    * otherwise.
    */
   std::pair<double, double> midrad() const;

   /// Intersection with assignment
   Interval &operator&=(const Interval &other);

   /// Intersection
   friend Interval operator&(const Interval &x, const Interval &y);

   /// Interval hull with assignment
   Interval &operator|=(const Interval &other);

   /// Interval hull
   friend Interval operator|(const Interval &x, const Interval &y);

   /// Set complement
   friend std::pair<Interval, Interval> complement(const Interval &x);

   /// Set difference
   friend std::pair<Interval, Interval> setminus(const Interval &x, const Interval &y);

   /**
    * @brief Rounds an interval to integral bounds.
    *
    * Returns the largest interval of integers included in x
    */
   friend Interval round(const Interval &x);

   /**
    * @brief Inflation method.
    *
    * Given delta > 1.0 and chi > 0.0, returns
    * m(this) + delta*(this - m(this)) + chi*[-1,1]
    */
   Interval inflate(double delta, double chi) const;

   /**
    * @brief Test of improvement.
    *
    * Assume that this is included in old. We want to know is this improves
    * old enough with respect to the tolerance tol. Returns true if both
    * intervals are non empty and (1.0 - x.width() / old.width()) > tol.
    *
    * The (relative) tolerance must be a real number in [0,1].
    */
   bool improves(const Interval &old, double tol) const;

   /// Output on a stream
   friend std::ostream &operator<<(std::ostream &os, const Interval &x);

   /// @name Addition
   ///@{
   Interval &operator+=(const Interval &other);
   friend Interval operator+(const Interval &x, const Interval &y);
   friend Interval addPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval addPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval addPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Subtraction
   ///@{
   Interval &operator-=(const Interval &other);
   friend Interval operator-(const Interval &x, const Interval &y);
   friend Interval subPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval subPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval subPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Unary subtraction
   ///@{
   friend Interval operator-(const Interval &x);
   friend Interval usubPX(const Interval &x, const Interval &y);
   friend Interval usubPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Multiplication
   ///@{
   Interval &operator*=(const Interval &other);
   friend Interval operator*(const Interval &x, const Interval &y);
   friend Interval mulPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval mulPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval mulPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Division
   ///@{
   Interval &operator/=(const Interval &other);
   friend Interval operator/(const Interval &x, const Interval &y);

   friend std::pair<Interval, Interval> extDiv(const Interval &x, const Interval &y);
   friend Interval divPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval divPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval divPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Square
   ///@{
   friend Interval sqr(const Interval &x);
   friend Interval sqrPX(const Interval &x, const Interval &y);
   friend Interval sqrPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Square root
   ///@{
   friend Interval sqrt(const Interval &x);
   friend Interval sqrtPX(const Interval &x, const Interval &y);
   friend Interval sqrtPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Power function
   ///@{
   friend Interval pow(const Interval &x, int n);
   friend Interval powPX(const Interval &x, int n, const Interval &y);
   friend Interval powPY(const Interval &x, int n, const Interval &y);
   ///@}

   /// @name Exponential
   ///@{
   friend Interval exp(const Interval &x);
   friend Interval expPX(const Interval &x, const Interval &y);
   friend Interval expPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Logarithm
   ///@{
   friend Interval log(const Interval &x);
   friend Interval logPX(const Interval &x, const Interval &y);
   friend Interval logPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Sine
   ///@{
   friend Interval sin(const Interval &x);
   friend Interval sinPX(const Interval &x, const Interval &y);
   friend Interval sinPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Cosine
   ///@{
   friend Interval cos(const Interval &x);
   friend Interval cosPX(const Interval &x, const Interval &y);
   friend Interval cosPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Tangent
   ///@{
   friend Interval tan(const Interval &x);
   friend Interval tanPX(const Interval &x, const Interval &y);
   friend Interval tanPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Hyperbolic cosine
   ///@{
   friend Interval cosh(const Interval &x);
   friend Interval coshPX(const Interval &x, const Interval &y);
   friend Interval coshPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Hyperbolic sine
   ///@{
   friend Interval sinh(const Interval &x);
   friend Interval sinhPX(const Interval &x, const Interval &y);
   friend Interval sinhPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Arccosine
   ///@{
   friend Interval acos(const Interval &x);
   ///@}

   /// @name Arcsine
   ///@{
   friend Interval asin(const Interval &x);
   ///@}

   /// @name Arctangent
   ///@{
   friend Interval atan(const Interval &x);
   ///@}

   /// @name Hyperbolic arccosine
   ///@{
   friend Interval acosh(const Interval &x);
   ///@}

   /// @name Hyperbolic arcsine
   ///@{
   friend Interval asinh(const Interval &x);
   ///@}

   /// @name Hyperbolic arctangent
   ///@{
   friend Interval atanh(const Interval &x);
   ///@}

   ///@{
   /// Hyperbolic tangent
   friend Interval tanh(const Interval &x);
   friend Interval tanhPX(const Interval &x, const Interval &y);
   friend Interval tanhPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Absolute value
   ///@{
   friend Interval abs(const Interval &x);
   friend Interval absPX(const Interval &x, const Interval &y);
   friend Interval absPY(const Interval &x, const Interval &y);
   ///@}

   /// @name Minimum function
   ///@{
   friend Interval min(const Interval &x, const Interval &y);
   friend Interval minPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval minPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval minPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Maximum function
   ///@{
   friend Interval max(const Interval &x, const Interval &y);
   friend Interval maxPX(const Interval &x, const Interval &y, const Interval &z);
   friend Interval maxPY(const Interval &x, const Interval &y, const Interval &z);
   friend Interval maxPZ(const Interval &x, const Interval &y, const Interval &z);
   ///@}

   /// @name Sign function
   ///@{
   friend Interval sgn(const Interval &x);
   friend Interval sgnPX(const Interval &x, const Interval &y);
   friend Interval sgnPY(const Interval &x, const Interval &y);
   ///@}

private:
   using Traits = IntervalTraits<RawInterval>;
   RawInterval impl_; // interval enclosed

   /// Constructor
   Interval(const Interval::Traits::interval &x);
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Interval &x);

} // namespace realpaver

#endif
