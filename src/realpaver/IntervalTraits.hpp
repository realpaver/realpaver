///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_INTERVAL_TRAITS_HPP
#define REALPAVER_INTERVAL_TRAITS_HPP

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a traits class interfacing the interval arithmetic library.
///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IntervalTraits
{
   typedef T interval;

   static interval create();

   static interval create(double a);

   static interval create(double l, double r);

   static interval create(const char* s);

   static interval create(const char* sl, char const* sr);

   static void setEmpty(interval& x);

   static double left(const interval& x);

   static double right(const interval& x);

   static size_t hashCode(const interval& x);

   static interval universe();

   static interval positive();

   static interval negative();

   static interval emptyset();

   static interval pi();

   static interval halfPi();

   static interval twoPi();

   static interval zero();

   static interval one();

   static interval minusOne();

   static interval minusOneZero();

   static interval zeroPlusOne();

   static interval minusOnePlusOne();

   static interval minusPiPlusPi();

   static interval zeroPi();

   static interval zeroTwoPi();

   static double infinity();

   static double width(const interval& x);

   static double radius(const interval& x);

   static double relWidth(const interval& x);

   static double midpoint(const interval& x);

   static double mig(const interval& x);

   static double mag(const interval& x);

   static bool isEmpty(const interval& x);

   static bool isCanonical(const interval& x);

   static bool isFinite(const interval& x);

   static bool isInfLeft(const interval& x);

   static bool isInfRight(const interval& x);

   static bool isSingleton(const interval& x);

   static bool isZero(const interval& x);

   static bool isAnInt(const interval& x);

   static bool contains(const interval& x, double a);

   static bool strictlyContains(const interval& x, double a);

   static bool containsZero(const interval& x);

   static bool strictlyContainsZero(const interval& x);

   static bool contains(const interval& x, const interval& y);

   static bool strictlyContains(const interval& x, const interval& y);

   static bool isSetEq(const interval& x, const interval& y);

   static bool isSetNeq(const interval& x, const interval& y);

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

   static double distance(const interval& x, const interval& y);

   static void inter_assign(interval& x, const interval& y);

   static interval inter(const interval& x, const interval& y);

   static void hull_assign(interval& x, const interval& y);

   static interval hull(const interval& x, const interval& y);

   static interval round(const interval& x);

   static void print(std::ostream& os, const interval& x);

   static std::streamsize precision(std::streamsize n);

   static std::streamsize precision();

   static void addAssign(interval& x, const interval& y);

   static interval add(const interval& x, const interval& y);

   static interval addPX(const interval& x, const interval& y,
                         const interval& z);

   static interval addPY(const interval& x, const interval& y,
                         const interval& z);

   static interval addPZ(const interval& x, const interval& y,
                         const interval& z);

   static void subAssign(interval& x, const interval& y);

   static interval sub(const interval& x, const interval& y);

   static interval subPX(const interval& x, const interval& y,
                         const interval& z);

   static interval subPY(const interval& x, const interval& y,
                         const interval& z);

   static interval subPZ(const interval& x, const interval& y,
                         const interval& z);

   static interval usub(const interval& x);

   static interval usubPX(const interval& x, const interval& y);

   static interval usubPY(const interval& x, const interval& y);

   static void mulAssign(interval& x, const interval& y);

   static interval mul(const interval& x, const interval& y);

   static interval mulPX(const interval& x, const interval& y,
                         const interval& z);

   static interval mulPY(const interval& x, const interval& y,
                         const interval& z);

   static interval mulPZ(const interval& x, const interval& y,
                         const interval& z);

   static void divAssign(interval& x, const interval& y);

   static interval div(const interval& x, const interval& y);

   static interval divPX(const interval& x, const interval& y,
                         const interval& z);

   static interval divPY(const interval& x, const interval& y,
                         const interval& z);

   static interval divPZ(const interval& x, const interval& y,
                         const interval& z);

   static interval sqr(const interval& x);

   static interval sqrPX(const interval& x, const interval& y);

   static interval sqrPY(const interval& x, const interval& y);

   static interval sqrt(const interval& x);

   static interval sqrtPX(const interval& x, const interval& y);

   static interval sqrtPY(const interval& x, const interval& y);

   static interval pow(const interval& x, int n);

   static interval powPX(const interval& x, int n, const interval& y);

   static interval powPY(const interval& x, int n, const interval& y);

   static interval exp(const interval& x);

   static interval expPX(const interval& x, const interval& y);

   static interval expPY(const interval& x, const interval& y);

   static interval log(const interval& x);

   static interval logPX(const interval& x, const interval& y);

   static interval logPY(const interval& x, const interval& y);

   static interval cos(const interval& x);

   static interval cosPX(const interval& x, const interval& y);

   static interval cosPY(const interval& x, const interval& y);

   static interval sin(const interval& x);

   static interval sinPX(const interval& x, const interval& y);

   static interval sinPY(const interval& x, const interval& y);

   static interval tan(const interval& x);

   static interval tanPX(const interval& x, const interval& y);

   static interval tanPY(const interval& x, const interval& y);

   static interval abs(const interval& x);

   static interval absPX(const interval& x, const interval& y);

   static interval absPY(const interval& x, const interval& y);

   static interval min(const interval& x, const interval& y);

   static interval minPX(const interval& x, const interval& y,
                         const interval& z);

   static interval minPY(const interval& x, const interval& y,
                         const interval& z);

   static interval minPZ(const interval& x, const interval& y,
                         const interval& z);

   static interval max(const interval& x, const interval& y);

   static interval maxPX(const interval& x, const interval& y,
                         const interval& z);

   static interval maxPY(const interval& x, const interval& y,
                         const interval& z);

   static interval maxPZ(const interval& x, const interval& y,
                         const interval& z);

   static interval sgn(const interval& x);

   static interval sgnPX(const interval& x, const interval& y);

   static interval sgnPY(const interval& x, const interval& y);
};

} // namespace

#endif
