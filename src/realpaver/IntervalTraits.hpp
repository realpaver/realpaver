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
 * @file   IntervalTraits.hpp
 * @brief  Traits class interfacing interval arithmetic libraries
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/
#ifndef REALPAVER_INTERVAL_TRAITS_HPP
#define REALPAVER_INTERVAL_TRAITS_HPP

namespace realpaver {

/// Traits class interfacing interval arithmetic libraries
template <typename T>
struct IntervalTraits
{
   /// Raw interval type
   using interval = T;

   /// @name Constructors
   ///@{

   /// Creates [-oo, +oo]
   static interval create();

   /// Creates [a, a]
   static interval create(double a);

   /// Creates [l, r]
   static interval create(double l, double r);

   /// Creates [s rounded downward, s rounded upward]
   static interval create(const char* s);

   /// Creates [sl rounded downward, sr rounded upward]
   static interval create(const char* sl, char const* sr);
   ///@}

   /// Assigns x to the empty set
   static void setEmpty(interval& x);

   /// Returns the left bound of x
   static double left(const interval& x);

   /// Returns the right bound of x
   static double right(const interval& x);

   /// Returns the hash code of x
   static size_t hashCode(const interval& x);

   /// @name Constants
   ///@{

   /// Returns [-oo, +oo]
   static interval universe();

   /// Returns [0, +oo]
   static interval positive();

   /// Returns [-oo, 0]
   static interval negative();

   /// Returns the emptyset
   static interval emptyset();

   /// Returns the hull of Pi
   static interval pi();

   /// Returns the hull of Pi/2
   static interval halfPi();

   /// Returns the hull of 2*Pi
   static interval twoPi();

   /// Returns [0, 0]
   static interval zero();

   /// Returns [1, 1]
   static interval one();

   /// Returns [-1, -1]
   static interval minusOne();

   /// Returns [-1, 0]
   static interval minusOneZero();

   /// Returns [0, 1]
   static interval zeroPlusOne();

   /// Returns [-1, 1]
   static interval minusOnePlusOne();

   /// Returns the hull of [-Pi, +Pi]
   static interval minusPiPlusPi();

   /// Returns the hull of [0, Pi]
   static interval zeroPi();

   /// Returns the hull of [0, 2*Pi]
   static interval zeroTwoPi();

   /// Returns +oo (double)
   static double infinity();
   ///@}

   /// Returns the width of x
   static double width(const interval& x);

   /// Returns the radius of x
   static double radius(const interval& x);

   /// Returns the midpoint of x
   static double midpoint(const interval& x);

   /// Returns the mignitude of x
   static double mig(const interval& x);

   /// Returns the magnitude of x
   static double mag(const interval& x);

   /// Returns true if x is empty
   static bool isEmpty(const interval& x);

   /// Returns true if x has at most two floats
   static bool isCanonical(const interval& x);

   /// Returns true if x is finite
   static bool isFinite(const interval& x);

   /// Returns true if left(x) == -oo
   static bool isInfLeft(const interval& x);

   /// Returns true if right(x) == +oo
   static bool isInfRight(const interval& x);

   /// Returns true if x is a point interval
   static bool isSingleton(const interval& x);

   /// Returns true if x is [0, 0]
   static bool isZero(const interval& x);

   /// Returns true if x is an int
   static bool isAnInt(const interval& x);

   /// @name Containment tests
   ///@{

   static bool contains(const interval& x, double a);

   static bool strictlyContains(const interval& x, double a);

   static bool containsZero(const interval& x);

   static bool strictlyContainsZero(const interval& x);

   static bool contains(const interval& x, const interval& y);

   static bool strictlyContains(const interval& x, const interval& y);

   static bool isSetEq(const interval& x, const interval& y);

   static bool isSetNeq(const interval& x, const interval& y);
   ///@}

   /// @name Set operations
   ///@{

   static bool isNegative(const interval& x);

   static bool isStrictlyNegative(const interval& x);

   static bool isPositive(const interval& x);

   static bool isStrictlyPositive(const interval& x);

   static bool isPossiblyEq(const interval& x, const interval& y);

   static bool isPossiblyNeq(const interval& x, const interval& y);

   static bool isPossiblyLe(const interval& x, const interval& y);

   static bool isPossiblyLt(const interval& x, const interval& y);

   static bool isPossiblyGe(const interval& x, const interval& y);

   static bool isPossiblyGt(const interval& x, const interval& y);

   static bool isCertainlyEq(const interval& x, const interval& y);

   static bool isCertainlyNeq(const interval& x, const interval& y);

   static bool isCertainlyLe(const interval& x, const interval& y);

   static bool isCertainlyLt(const interval& x, const interval& y);

   static bool isCertainlyGe(const interval& x, const interval& y);

   static bool isCertainlyGt(const interval& x, const interval& y);

   static bool isDisjoint(const interval& x, const interval& y);

   static bool overlaps(const interval& x, const interval& y);
   ///@}

   /// Returns the Hausdorff distance between x and y
   static double distance(const interval& x, const interval& y);

   /// Intersection with assignment
   static void inter_assign(interval& x, const interval& y);

   /// Intersection
   static interval inter(const interval& x, const interval& y);

   /// Hull with assignment
   static void hull_assign(interval& x, const interval& y);

   /// Hull
   static interval hull(const interval& x, const interval& y);

   /// Rounds x to integral bounds
   static interval round(const interval& x);

   /// Output on a stream
   static void print(std::ostream& os, const interval& x);

   /// @name Addition
   ///@{

   static void addAssign(interval& x, const interval& y);
   static interval add(const interval& x, const interval& y);
   static interval addPX(const interval& x, const interval& y,
                         const interval& z);
   static interval addPY(const interval& x, const interval& y,
                         const interval& z);
   static interval addPZ(const interval& x, const interval& y,
                         const interval& z);
   ///@}

   /// @name Subtraction
   ///@{

   static void subAssign(interval& x, const interval& y);
   static interval sub(const interval& x, const interval& y);
   static interval subPX(const interval& x, const interval& y,
                         const interval& z);
   static interval subPY(const interval& x, const interval& y,
                         const interval& z);
   static interval subPZ(const interval& x, const interval& y,
                         const interval& z);
   ///@}

   /// @name Unary subtraction
   ///@{

   static interval usub(const interval& x);
   static interval usubPX(const interval& x, const interval& y);
   static interval usubPY(const interval& x, const interval& y);
   ///@}

   /// @name Multiplication
   ///@{

   static void mulAssign(interval& x, const interval& y);
   static interval mul(const interval& x, const interval& y);
   static interval mulPX(const interval& x, const interval& y,
                         const interval& z);
   static interval mulPY(const interval& x, const interval& y,
                         const interval& z);
   static interval mulPZ(const interval& x, const interval& y,
                         const interval& z);

   ///@}

   /// @name Division
   ///@{

   static void divAssign(interval& x, const interval& y);
   static interval div(const interval& x, const interval& y);
   static interval divPX(const interval& x, const interval& y,
                         const interval& z);
   static interval divPY(const interval& x, const interval& y,
                         const interval& z);
   static interval divPZ(const interval& x, const interval& y,
                         const interval& z);
   ///@}

   /// @name Square
   ///@{

   static interval sqr(const interval& x);
   static interval sqrPX(const interval& x, const interval& y);
   static interval sqrPY(const interval& x, const interval& y);
   ///@}

   /// @name Square root
   ///@{
   static interval sqrt(const interval& x);
   static interval sqrtPX(const interval& x, const interval& y);
   static interval sqrtPY(const interval& x, const interval& y);
   ///@}

   /// @name Power function
   ///@{

   static interval pow(const interval& x, int n);
   static interval powPX(const interval& x, int n, const interval& y);
   static interval powPY(const interval& x, int n, const interval& y);
   ///@}

   /// @name Exponential function
   ///@{

   static interval exp(const interval& x);
   static interval expPX(const interval& x, const interval& y);
   static interval expPY(const interval& x, const interval& y);
   ///@}

   /// @name Logarithm
   ///@{

   static interval log(const interval& x);
   static interval logPX(const interval& x, const interval& y);
   static interval logPY(const interval& x, const interval& y);
   ///@}

   /// @name Cosine
   ///@{
   static interval cos(const interval& x);
   static interval cosPX(const interval& x, const interval& y);
   static interval cosPY(const interval& x, const interval& y);
   ///@}

   /// @name Sibe
   ///@{

   static interval sin(const interval& x);
   static interval sinPX(const interval& x, const interval& y);
   static interval sinPY(const interval& x, const interval& y);
   ///@}

   /// @name Tangent
   ///@{

   static interval tan(const interval& x);
   static interval tanPX(const interval& x, const interval& y);
   static interval tanPY(const interval& x, const interval& y);
   ///@}

   /// @name Hyperbolic cosine
   ///@{

   static interval cosh(const interval& x);
   static interval coshPX(const interval& x, const interval& y);
   static interval coshPY(const interval& x, const interval& y);
   ///@}

   /// @name Hyperbolic sine
   ///@{

   static interval sinh(const interval& x);
   static interval sinhPX(const interval& x, const interval& y);
   static interval sinhPY(const interval& x, const interval& y);
   ///@}

   /// @name Hyperbolic tangent
   ///@{

   static interval tanh(const interval& x);
   static interval tanhPX(const interval& x, const interval& y);
   static interval tanhPY(const interval& x, const interval& y);
   ///@}

   /// @name Absolute value
   ///@{

   static interval abs(const interval& x);
   static interval absPX(const interval& x, const interval& y);
   static interval absPY(const interval& x, const interval& y);
   ///@}

   /// @name Minimum
   ///@{

   static interval min(const interval& x, const interval& y);
   static interval minPX(const interval& x, const interval& y,
                         const interval& z);
   static interval minPY(const interval& x, const interval& y,
                         const interval& z);
   static interval minPZ(const interval& x, const interval& y,
                         const interval& z);
   ///@}

   /// @name Maximum
   ///@{

   static interval max(const interval& x, const interval& y);
   static interval maxPX(const interval& x, const interval& y,
                         const interval& z);
   static interval maxPY(const interval& x, const interval& y,
                         const interval& z);
   static interval maxPZ(const interval& x, const interval& y,
                         const interval& z);
   ///@}

   /// @name Sign function
   ///@{

   static interval sgn(const interval& x);
   static interval sgnPX(const interval& x, const interval& y);
   static interval sgnPY(const interval& x, const interval& y);
   ///@}
};

} // namespace

#endif
