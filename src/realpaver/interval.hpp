// This file is part of realpaver. License: see COPYING file.

#ifndef REALPAVER_INTERVAL_HPP
#define REALPAVER_INTERVAL_HPP

#include <utility>
#include "realpaver/Common.hpp"
#include "realpaver/IntervalImpl.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of intervals bounded by floats in double precision. This class is a
 * wrapper for an arithmetic interval library selected at configuration-time.
 *****************************************************************************/
class Interval {
public:
   // constructors
   Interval();
   Interval(const double& a);
   Interval(const double& l, const double& r);
   Interval(const char* s);
   Interval(const char* sl, const char* sr);
   Interval(const Interval&) = default;

   // construction methods
   static Interval lessThan(const double& a);
   static Interval moreThan(const double& a);

   // hash code
   size_t hashCode() const;

   // constants
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

   // returns +oo
   static double infinity();

   // returns the left bound
   double left() const;

   // returns the right bound
   double right() const;

   // assigns the left bound
   void setLeft(const double& a);

   // assigns the right bound
   void setRight(const double& a);

   // returns the width
   double width() const;

   // returns the radius
   double radius() const;

   // returns the relative width
   double relWidth() const;

   // returns the midpoint
   double midpoint() const;

   // returns the mignitude
   double mig() const;

   // returns the magnitude
   double mag() const;

   // assigns this interval to the empty set
   void setEmpty();

   // tests
   bool isEmpty() const;
   bool isCanonical() const;
   bool isFinite() const;
   bool isUniverse() const;
   bool isInf() const;
   bool isInfLeft() const;
   bool isInfRight() const;
   bool isSingleton() const;
   bool isZero() const;

   // set operations
   bool contains(const double& a) const;
   bool strictlyContains(const double& a) const;
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

   // non intersection test
   bool isDisjoint(const Interval& other) const;

   // intersection test
   bool overlaps(const Interval& other) const;

   // Hausdorff distance
   double distance(const Interval& other) const;

   // intersection
   Interval& operator&=(const Interval& other);
   friend Interval operator&(const Interval& x, const Interval& y);

   // hull
   Interval& operator|=(const Interval& other);
   friend Interval operator|(const Interval& x, const Interval& y);

   // set complement
   friend std::pair<Interval,Interval> complement(const Interval& x);

   // set difference
   friend std::pair<Interval,Interval> setminus(const Interval& x,
                                                const Interval& y);

   // round to integral bounds
   friend Interval round(const Interval& x);

   // inflation, let x be this interval and let m(x) be the midpoint
   // returns m(x) + delta*(x - m(x)) + chi*[-1,1]
   Interval inflate(const double& delta, const double& chi) const;

   // output on a stream
   friend std::ostream& operator<<(std::ostream& os, const Interval& x);
   static std::streamsize precision(std::streamsize n);
   static std::streamsize precision();

   // addition
   Interval& operator+=(const Interval& other);
   friend Interval operator+(const Interval& x, const Interval& y);
   friend Interval addPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval addPY(const Interval& x, const Interval& y,
                          const Interval& z);
   friend Interval addPZ(const Interval& x, const Interval& y,
                          const Interval& z);

   // subtraction
   Interval& operator-=(const Interval& other);
   friend Interval operator-(const Interval& x, const Interval& y);
   friend Interval subPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval subPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval subPZ(const Interval& x, const Interval& y,
                         const Interval& z);

   // unary subtraction
   friend Interval operator-(const Interval& x);
   friend Interval usubPX(const Interval& x, const Interval& y);
   friend Interval usubPY(const Interval& x, const Interval& y);

   // multiplication
   Interval& operator*=(const Interval& other);
   friend Interval operator*(const Interval& x, const Interval& y);
   friend Interval mulPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval mulPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval mulPZ(const Interval& x, const Interval& y,
                         const Interval& z);

   // division
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

   // square
   friend Interval sqr(const Interval& x);
   friend Interval sqrPX(const Interval& x, const Interval& y);
   friend Interval sqrPY(const Interval& x, const Interval& y);

   // square root
   friend Interval sqrt(const Interval& x);
   friend Interval sqrtPX(const Interval& x, const Interval& y);
   friend Interval sqrtPY(const Interval& x, const Interval& y);

   // power
   friend Interval pow(const Interval& x, int n);
   friend Interval powPX(const Interval& x, int n, const Interval& y);
   friend Interval powPY(const Interval& x, int n, const Interval& y);

   // exponential
   friend Interval exp(const Interval& x);
   friend Interval expPX(const Interval& x, const Interval& y);
   friend Interval expPY(const Interval& x, const Interval& y);

   // logarithm
   friend Interval log(const Interval& x);
   friend Interval logPX(const Interval& x, const Interval& y);
   friend Interval logPY(const Interval& x, const Interval& y);

   // sine
   friend Interval sin(const Interval& x);
   friend Interval sinPX(const Interval& x, const Interval& y);
   friend Interval sinPY(const Interval& x, const Interval& y);

   // cosine
   friend Interval cos(const Interval& x);
   friend Interval cosPX(const Interval& x, const Interval& y);
   friend Interval cosPY(const Interval& x, const Interval& y);

   // tangent
   friend Interval tan(const Interval& x);
   friend Interval tanPX(const Interval& x, const Interval& y);
   friend Interval tanPY(const Interval& x, const Interval& y);

   // absolute value
   friend Interval abs(const Interval& x);
   friend Interval absPX(const Interval& x, const Interval& y);
   friend Interval absPY(const Interval& x, const Interval& y);

   // minimum
   friend Interval min(const Interval& x, const Interval& y);
   friend Interval minPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval minPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval minPZ(const Interval& x, const Interval& y,
                         const Interval& z);

   // maximum
   friend Interval max(const Interval& x, const Interval& y);
   friend Interval maxPX(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval maxPY(const Interval& x, const Interval& y,
                         const Interval& z);
   friend Interval maxPZ(const Interval& x, const Interval& y,
                         const Interval& z);

   // sign
   friend Interval sgn(const Interval& x);
   friend Interval sgnPX(const Interval& x, const Interval& y);
   friend Interval sgnPY(const Interval& x, const Interval& y);   

private:
   typedef IntervalTraits<RawInterval> Traits;
   RawInterval impl_;    // interval enclosed

   // constructor
   Interval(const Interval::Traits::interval& x);
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Interval& x);

inline std::streamsize Interval::precision(std::streamsize n)
{
   return Interval::Traits::precision(n);
}

inline std::streamsize Interval::precision()
{
   return Interval::Traits::precision();
}

inline double Interval::left() const
{
   return Interval::Traits::left(impl_);
}

inline double Interval::right() const
{
   return Interval::Traits::right(impl_);
}

inline void Interval::setLeft(const double& a)
{
   impl_ = Interval::Traits::create(a, right());
}


inline void Interval::setRight(const double& a)
{
   impl_ = Interval::Traits::create(left(), a);   
}

inline double Interval::width() const
{
   return Interval::Traits::width(impl_);
}

inline double Interval::radius() const
{
   return Interval::Traits::radius(impl_);
}

inline size_t Interval::hashCode() const
{
   return Interval::Traits::hashCode(impl_);
}

inline double Interval::relWidth() const
{
   return Interval::Traits::relWidth(impl_);
}

inline double Interval::midpoint() const
{
   return Interval::Traits::midpoint(impl_);
}

inline double Interval::mig() const
{
   return Interval::Traits::mig(impl_);
}

inline double Interval::mag() const
{
   return Interval::Traits::mag(impl_);
}

inline void Interval::setEmpty()
{
   Interval::Traits::setEmpty(impl_);
}

inline bool Interval::isEmpty() const
{
   return Interval::Traits::isEmpty(impl_);
}

inline bool Interval::isCanonical() const
{
   return Interval::Traits::isCanonical(impl_);
}

inline bool Interval::isFinite() const
{
   return Interval::Traits::isFinite(impl_);
}

inline bool Interval::isInf() const
{
   return !isFinite();
}

inline bool Interval::isUniverse() const
{
   return isInfLeft() && isInfRight();
}

inline bool Interval::isInfLeft() const
{
   return Interval::Traits::isInfLeft(impl_);
}

inline bool Interval::isInfRight() const
{
   return Interval::Traits::isInfRight(impl_);
}

inline bool Interval::isSingleton() const
{
   return Interval::Traits::isSingleton(impl_);
}

inline bool Interval::isZero() const
{
   return Interval::Traits::isZero(impl_);
}

inline bool Interval::contains(const double& a) const
{
   return Interval::Traits::contains(impl_, a);
}

inline bool Interval::contains(const Interval& other) const
{
   return Interval::Traits::contains(impl_, other.impl_);
}

inline bool Interval::strictlyContains(const Interval& other) const
{
   return Interval::Traits::strictlyContains(impl_, other.impl_);
}

inline bool Interval::strictlyContains(const double& a) const
{
   return Interval::Traits::strictlyContains(impl_, a);
}

inline bool Interval::containsZero() const
{
   return Interval::Traits::containsZero(impl_);
}

inline bool Interval::strictlyContainsZero() const
{
   return Interval::Traits::strictlyContainsZero(impl_);
}

inline bool Interval::isSetEq(const Interval& other) const
{
   return Interval::Traits::isSetEq(impl_, other.impl_);
}

inline bool Interval::isSetNeq(const Interval& other) const
{
   return Interval::Traits::isSetNeq(impl_, other.impl_);
}

inline bool Interval::isNegative() const
{
   return Interval::Traits::isNegative(impl_);
}

inline bool Interval::isStrictlyNegative() const
{
   return Interval::Traits::isStrictlyNegative(impl_);
}

inline bool Interval::isPositive() const
{
   return Interval::Traits::isPositive(impl_);
}

inline bool Interval::isStrictlyPositive() const
{
   return Interval::Traits::isStrictlyPositive(impl_);
}

inline bool Interval::isPossiblyEq(const Interval& other) const
{
   return Interval::Traits::isPossiblyEq(impl_, other.impl_);
}

inline bool Interval::isPossiblyNeq(const Interval& other) const
{
   return Interval::Traits::isPossiblyNeq(impl_, other.impl_);
}

inline bool Interval::isPossiblyLe(const Interval& other) const
{
   return Interval::Traits::isPossiblyLe(impl_, other.impl_);
}

inline bool Interval::isPossiblyLt(const Interval& other) const
{
   return Interval::Traits::isPossiblyLt(impl_, other.impl_);
}

inline bool Interval::isPossiblyGe(const Interval& other) const
{
   return Interval::Traits::isPossiblyGe(impl_, other.impl_);
}

inline bool Interval::isPossiblyGt(const Interval& other) const
{
   return Interval::Traits::isPossiblyGt(impl_, other.impl_);
}

inline bool Interval::isCertainlyEq(const Interval& other) const
{
   return Interval::Traits::isCertainlyEq(impl_, other.impl_);
}

inline bool Interval::isCertainlyNeq(const Interval& other) const
{
   return Interval::Traits::isCertainlyNeq(impl_, other.impl_);
}

inline bool Interval::isCertainlyLe(const Interval& other) const
{
   return Interval::Traits::isCertainlyLe(impl_, other.impl_);
}

inline bool Interval::isCertainlyLt(const Interval& other) const
{
   return Interval::Traits::isCertainlyLt(impl_, other.impl_);
}

inline bool Interval::isCertainlyGe(const Interval& other) const
{
   return Interval::Traits::isCertainlyGe(impl_, other.impl_);
}

inline bool Interval::isCertainlyGt(const Interval& other) const
{
   return Interval::Traits::isCertainlyGt(impl_, other.impl_);
}

inline bool Interval::isPossiblyEqZero() const
{
   return isPossiblyEq(Interval::zero());
}

inline bool Interval::isPossiblyLeZero() const
{
   return isPossiblyLe(Interval::zero());
}

inline bool Interval::isPossiblyLtZero() const
{
   return isPossiblyLt(Interval::zero());
}

inline bool Interval::isPossiblyGeZero() const
{
   return isPossiblyGe(Interval::zero());
}

inline bool Interval::isPossiblyGtZero() const
{
   return isPossiblyGt(Interval::zero());
}

inline bool Interval::isCertainlyEqZero() const
{
   return isCertainlyEq(Interval::zero());
}

inline bool Interval::isCertainlyLeZero() const
{
   return isCertainlyLe(Interval::zero());
}

inline bool Interval::isCertainlyLtZero() const
{
   return isCertainlyLt(Interval::zero());
}

inline bool Interval::isCertainlyGeZero() const
{
   return isCertainlyGe(Interval::zero());
}

inline bool Interval::isCertainlyGtZero() const
{
   return isCertainlyGt(Interval::zero());
}

inline bool Interval::isDisjoint(const Interval& other) const
{
   return Interval::Traits::isDisjoint(impl_, other.impl_);
}

inline bool Interval::overlaps(const Interval& other) const
{
   return Interval::Traits::overlaps(impl_, other.impl_);
}

inline double Interval::distance(const Interval& other) const
{
   return Interval::Traits::distance(impl_, other.impl_);
}

inline Interval& Interval::operator&=(const Interval& other)
{
   Interval::Traits::inter_assign(impl_, other.impl_);  
   return *this;
}

inline Interval operator&(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::inter(x.impl_, y.impl_));
}

inline Interval& Interval::operator|=(const Interval& other)
{
   Interval::Traits::hull_assign(impl_, other.impl_);     
   return *this;
}

inline Interval operator|(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::hull(x.impl_, y.impl_));   
}

inline Interval round(const Interval& x)
{
   return Interval(Interval::Traits::round(x.impl_));      
}

inline Interval& Interval::operator+=(const Interval& other)
{
   Interval::Traits::addAssign(impl_, other.impl_);     
   return *this;
}

inline Interval operator+(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::add(x.impl_, y.impl_));      
}

inline Interval addPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::addPX(x.impl_, y.impl_, z.impl_));
}

inline Interval addPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::addPY(x.impl_, y.impl_, z.impl_));   
}

inline Interval addPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::addPZ(x.impl_, y.impl_, z.impl_));   
}

inline Interval& Interval::operator-=(const Interval& other)
{
   Interval::Traits::subAssign(impl_, other.impl_);     
   return *this;   
}

inline Interval operator-(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sub(x.impl_, y.impl_));      
}

inline Interval subPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::subPX(x.impl_, y.impl_, z.impl_));
}

inline Interval subPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::subPY(x.impl_, y.impl_, z.impl_));   
}

inline Interval subPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::subPZ(x.impl_, y.impl_, z.impl_));   
}

inline Interval operator-(const Interval& x)
{
   return Interval(Interval::Traits::usub(x.impl_));
}

inline Interval usubPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::usubPX(x.impl_, y.impl_));   
}

inline Interval usubPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::usubPY(x.impl_, y.impl_));      
}

inline Interval& Interval::operator*=(const Interval& other)
{
   Interval::Traits::mulAssign(impl_, other.impl_);  
   return *this;   
}

inline Interval operator*(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::mul(x.impl_, y.impl_));      
}

inline Interval mulPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::mulPX(x.impl_, y.impl_, z.impl_));
}

inline Interval mulPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::mulPY(x.impl_, y.impl_, z.impl_));   
}

inline Interval mulPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::mulPZ(x.impl_, y.impl_, z.impl_));   
}

inline Interval& Interval::operator/=(const Interval& other)
{
   Interval::Traits::divAssign(impl_, other.impl_);
   return *this;   
}

inline Interval operator/(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::div(x.impl_, y.impl_));      
}

inline Interval divPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPX(x.impl_, y.impl_, z.impl_));
}

inline Interval divPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPY(x.impl_, y.impl_, z.impl_));   
}

inline Interval divPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPZ(x.impl_, y.impl_, z.impl_));   
}

inline Interval sqr(const Interval& x)
{
   return Interval(Interval::Traits::sqr(x.impl_));
}

inline Interval sqrPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrPX(x.impl_, y.impl_));
}

inline Interval sqrPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrPY(x.impl_, y.impl_));
}

inline Interval sqrt(const Interval& x)
{
   return Interval(Interval::Traits::sqrt(x.impl_));
}

inline Interval sqrtPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrtPX(x.impl_, y.impl_));
}

inline Interval sqrtPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrtPY(x.impl_, y.impl_));
}

inline Interval pow(const Interval& x, int n)
{
   return Interval(Interval::Traits::pow(x.impl_, n));
}

inline Interval powPX(const Interval& x, int n, const Interval& y)
{
   return Interval(Interval::Traits::powPX(x.impl_, n, y.impl_));
}

inline Interval powPY(const Interval& x, int n, const Interval& y)
{
   return Interval(Interval::Traits::powPY(x.impl_, n, y.impl_));   
}

inline Interval exp(const Interval& x)
{
   return Interval(Interval::Traits::exp(x.impl_));
}

inline Interval expPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::expPX(x.impl_, y.impl_));
}

inline Interval expPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::expPY(x.impl_, y.impl_));
}

inline Interval log(const Interval& x)
{
   return Interval(Interval::Traits::log(x.impl_));
}

inline Interval logPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::logPX(x.impl_, y.impl_));
}

inline Interval logPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::logPY(x.impl_, y.impl_));
}

inline Interval sin(const Interval& x)
{
   return Interval(Interval::Traits::sin(x.impl_));
}

inline Interval sinPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sinPX(x.impl_, y.impl_));
}

inline Interval sinPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sinPY(x.impl_, y.impl_));
}

inline Interval cos(const Interval& x)
{
   return Interval(Interval::Traits::cos(x.impl_));
}

inline Interval cosPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::cosPX(x.impl_, y.impl_));
}

inline Interval cosPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::cosPY(x.impl_, y.impl_));
}

inline Interval tan(const Interval& x)
{
   return Interval(Interval::Traits::tan(x.impl_));
}

inline Interval tanPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::tanPX(x.impl_, y.impl_));
}

inline Interval tanPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::tanPY(x.impl_, y.impl_));
}

inline Interval abs(const Interval& x)
{
   return Interval(Interval::Traits::abs(x.impl_));
}

inline Interval absPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::absPX(x.impl_, y.impl_));
}

inline Interval absPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::absPY(x.impl_, y.impl_));
}

inline Interval min(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::min(x.impl_, y.impl_));
}

inline Interval minPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPX(x.impl_, y.impl_, z.impl_));
}

inline Interval minPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPY(x.impl_, y.impl_, z.impl_));
}

inline Interval minPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPZ(x.impl_, y.impl_, z.impl_));
}

inline Interval max(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::max(x.impl_, y.impl_));
}

inline Interval maxPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPX(x.impl_, y.impl_, z.impl_));
}

inline Interval maxPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPY(x.impl_, y.impl_, z.impl_));
}

inline Interval maxPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPZ(x.impl_, y.impl_, z.impl_));
}

inline Interval sgn(const Interval& x)
{
   return Interval(Interval::Traits::sgn(x.impl_));
}

inline Interval sgnPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sgnPX(x.impl_, y.impl_));
}

inline Interval sgnPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sgnPY(x.impl_, y.impl_));
}



// TODO: supprimer IntervalImprovement
/*****************************************************************************
 * Class of improvement factors used to stop a sequence of nested intervals.
 *****************************************************************************/
class IntervalImprovement {
public:
   // constructor given a percentage f in [0,100]
   IntervalImprovement(double val = 0.0);

   // returns true if the width of x is less than the width of old multiplied
   // by f percent where f is the factor enclosed in this
   bool test(const Interval& x, const Interval& old) const;

   // returns the improvement factor between the width of x and the width
   // of old
   static IntervalImprovement calculate(const Interval& x, const Interval& old);

   // gets the value
   double getVal() const;

   // modifies the value
   void setVal(double val);

private:
   double val_;     // percentage in [0,100]
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const IntervalImprovement& imp);



} // namespace

#endif
