///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/Stat.hpp"

namespace realpaver {

Interval::Interval() : Interval(Interval::Traits::create())
{}

Interval::Interval(double l, double r)
      : Interval(Interval::Traits::create(l,r))
{}

Interval::Interval(double a) : Interval(Interval::Traits::create(a))
{}

Interval::Interval(const std::string& s)
   : Interval(Interval::Traits::create(s.c_str()))
{}

Interval::Interval(const std::string& sl, const std::string& sr)
      : Interval(Interval::Traits::create(sl.c_str(), sr.c_str()))
{}

Interval Interval::lessThan(double a)
{
   return Interval(Interval::universe().left(), a);
}

Interval Interval::moreThan(double a)
{
   return Interval(a, Interval::universe().right());   
}

Interval::Interval(const Interval::Traits::interval& x):
   impl_(x)
{}

Interval Interval::universe()
{
   static Interval x(Interval::Traits::universe());
   return x;
}

Interval Interval::positive()
{
   static Interval x(Interval::Traits::positive());
   return x;
}

Interval Interval::negative()
{
   static Interval x(Interval::Traits::negative());
   return x;
}

Interval Interval::minusOnePlusOne()
{
   static Interval x( Interval::minusOne() | Interval::one() );
   return x;
}

Interval Interval::minusOneZero()
{
   static Interval x( Interval::minusOne() | Interval::zero() );
   return x;
}

Interval Interval::zeroPlusOne()
{
   static Interval x( Interval::zero() | Interval::one() );
   return x;
}

Interval Interval::emptyset()
{
   static Interval x(Interval::Traits::emptyset());
   return x;
}

Interval Interval::halfPi()
{
   static Interval x(Interval::Traits::halfPi());
   return x;
}

Interval Interval::pi()
{
   static Interval x(Interval::Traits::pi());
   return x;
}

Interval Interval::twoPi()
{
   static Interval x(Interval::Traits::twoPi());
   return x;
}

Interval Interval::zero()
{
   static Interval x(Interval::Traits::zero());
   return x;
}

Interval Interval::one()
{
   static Interval x(Interval::Traits::one());
   return x;
}

Interval Interval::minusOne()
{
   static Interval x(Interval::Traits::minusOne());
   return x;
}

Interval Interval::minusPiPlusPi()
{
   static Interval x(Interval::Traits::minusPiPlusPi());
   return x;
}

Interval Interval::zeroPi()
{
   static Interval x(Interval::Traits::zeroPi());
   return x;
}

Interval Interval::zeroTwoPi()
{
   static Interval x(Interval::Traits::zeroTwoPi());
   return x;
}

double Interval::infinity()
{
   static double a = Interval::Traits::infinity();
   return a;
}

Interval Interval::inflate(double delta, double chi) const
{
   ASSERT(delta > 1.0, "Bad inflation factor delta = " << delta);
   ASSERT(chi > 0.0, "Bad inflation factor chi = " << chi);

   if (isEmpty() || isInf())
      return *this;

   Interval m(midpoint());

   return m + delta*(*this - m) + chi*minusOnePlusOne();
}

std::pair<Interval,Interval> complement(const Interval& x)
{
   Interval E(Interval::emptyset());

   if (x.isEmpty())
      return std::make_pair(Interval::universe(), E);

   bool infl = x.isInfLeft(),
        infr = x.isInfRight();

   if (infl)
   {
      if (infr)
         return std::make_pair(E, E);

      else
         return std::make_pair(Interval::moreThan(x.right()), E);
   }
   else
   {
      if (infr)
         return std::make_pair(Interval::lessThan(x.left()), E);

      else
         return std::make_pair(Interval::lessThan(x.left()),
                               Interval::moreThan(x.right()));
   }
}

std::pair<Interval,Interval> setminus(const Interval& x, const Interval& y)
{
   Interval E(Interval::emptyset());

   if (x.isEmpty() || y.isEmpty())
      return std::make_pair(x, E);

   if (y.contains(x))
      return std::make_pair(E, E);

   if (x.contains(y))
   {
      if (y.isSingleton())
         return std::make_pair(x, E);

      if (x.left() == y.left())
      {
         Interval z(y.right(), x.right());
         return std::make_pair(z, E);
      }
      else if (x.right() == y.right())
      {
         Interval z(x.left(), y.left());
         return std::make_pair(z, E);         
      }
      else
      {
         Interval u(x.left(), y.left());
         Interval v(y.right(), x.right());
         return std::make_pair(u, v);
      }
   }

   if (y.contains(x.left()))
   {
      Interval z(y.right(), x.right());
      return std::make_pair(z, E);
   }

   if (y.contains(x.right()))
   {
      Interval z(x.left(), y.left());
      return std::make_pair(z, E);
   }

   // last case: x and y are disjoint
   return std::make_pair(x, E);
}

std::pair<Interval,Interval> extDiv(const Interval& x, const Interval& y)
{
   if (y.strictlyContainsZero())
   {
      Interval z1( x / (y & Interval::negative()) ),
               z2( x / (y & Interval::positive()) );

      if (z1.isDisjoint(z2))
      {
         if (z1.isCertainlyLt(z2))
            return std::make_pair(z1, z2);
         else
            return std::make_pair(z2, z1);
      }
      else
         return std::make_pair(z1|z2, Interval::emptyset());
   }
   else
   {
      return std::make_pair(x/y,Interval::emptyset()); 
   }
}

std::ostream& operator<<(std::ostream& os, const Interval& x)
{
   Interval::Traits::print(os, x.impl_);
   return os;
}

std::streamsize Interval::precision(std::streamsize n)
{
   return Interval::Traits::precision(n);
}

std::streamsize Interval::precision()
{
   return Interval::Traits::precision();
}

double Interval::left() const
{
   return Interval::Traits::left(impl_);
}

double Interval::right() const
{
   return Interval::Traits::right(impl_);
}

void Interval::setLeft(double a)
{
   impl_ = Interval::Traits::create(a, right());
}

void Interval::setRight(double a)
{
   impl_ = Interval::Traits::create(left(), a);   
}

double Interval::width() const
{
   return Interval::Traits::width(impl_);
}

double Interval::radius() const
{
   return Interval::Traits::radius(impl_);
}

size_t Interval::hashCode() const
{
   return Interval::Traits::hashCode(impl_);
}

double Interval::relWidth() const
{
   return Interval::Traits::relWidth(impl_);
}

double Interval::midpoint() const
{
   return Interval::Traits::midpoint(impl_);
}

double Interval::mig() const
{
   return Interval::Traits::mig(impl_);
}

double Interval::mag() const
{
   return Interval::Traits::mag(impl_);
}

void Interval::setEmpty()
{
   Interval::Traits::setEmpty(impl_);
}

bool Interval::isEmpty() const
{
   return Interval::Traits::isEmpty(impl_);
}

bool Interval::isCanonical() const
{
   return Interval::Traits::isCanonical(impl_);
}

bool Interval::isFinite() const
{
   return Interval::Traits::isFinite(impl_);
}

bool Interval::isInf() const
{
   return !isFinite();
}

bool Interval::isUniverse() const
{
   return isInfLeft() && isInfRight();
}

bool Interval::isInfLeft() const
{
   return Interval::Traits::isInfLeft(impl_);
}

bool Interval::isInfRight() const
{
   return Interval::Traits::isInfRight(impl_);
}

bool Interval::isSingleton() const
{
   return Interval::Traits::isSingleton(impl_);
}

bool Interval::isZero() const
{
   return Interval::Traits::isZero(impl_);
}

bool Interval::isAnInt() const
{
  return Interval::Traits::isAnInt(impl_);   
}

bool Interval::contains(double a) const
{
   return Interval::Traits::contains(impl_, a);
}

bool Interval::contains(const Interval& other) const
{
   return Interval::Traits::contains(impl_, other.impl_);
}

bool Interval::strictlyContains(const Interval& other) const
{
   return Interval::Traits::strictlyContains(impl_, other.impl_);
}

bool Interval::strictlyContains(double a) const
{
   return Interval::Traits::strictlyContains(impl_, a);
}

bool Interval::containsZero() const
{
   return Interval::Traits::containsZero(impl_);
}

bool Interval::strictlyContainsZero() const
{
   return Interval::Traits::strictlyContainsZero(impl_);
}

bool Interval::isSetEq(const Interval& other) const
{
   return Interval::Traits::isSetEq(impl_, other.impl_);
}

bool Interval::isSetNeq(const Interval& other) const
{
   return Interval::Traits::isSetNeq(impl_, other.impl_);
}

bool Interval::isNegative() const
{
   return Interval::Traits::isNegative(impl_);
}

bool Interval::isStrictlyNegative() const
{
   return Interval::Traits::isStrictlyNegative(impl_);
}

bool Interval::isPositive() const
{
   return Interval::Traits::isPositive(impl_);
}

bool Interval::isStrictlyPositive() const
{
   return Interval::Traits::isStrictlyPositive(impl_);
}

bool Interval::isPossiblyEq(const Interval& other) const
{
   return Interval::Traits::isPossiblyEq(impl_, other.impl_);
}

bool Interval::isPossiblyNeq(const Interval& other) const
{
   return Interval::Traits::isPossiblyNeq(impl_, other.impl_);
}

bool Interval::isPossiblyLe(const Interval& other) const
{
   return Interval::Traits::isPossiblyLe(impl_, other.impl_);
}

bool Interval::isPossiblyLt(const Interval& other) const
{
   return Interval::Traits::isPossiblyLt(impl_, other.impl_);
}

bool Interval::isPossiblyGe(const Interval& other) const
{
   return Interval::Traits::isPossiblyGe(impl_, other.impl_);
}

bool Interval::isPossiblyGt(const Interval& other) const
{
   return Interval::Traits::isPossiblyGt(impl_, other.impl_);
}

bool Interval::isCertainlyEq(const Interval& other) const
{
   return Interval::Traits::isCertainlyEq(impl_, other.impl_);
}

bool Interval::isCertainlyNeq(const Interval& other) const
{
   return Interval::Traits::isCertainlyNeq(impl_, other.impl_);
}

bool Interval::isCertainlyLe(const Interval& other) const
{
   return Interval::Traits::isCertainlyLe(impl_, other.impl_);
}

bool Interval::isCertainlyLt(const Interval& other) const
{
   return Interval::Traits::isCertainlyLt(impl_, other.impl_);
}

bool Interval::isCertainlyGe(const Interval& other) const
{
   return Interval::Traits::isCertainlyGe(impl_, other.impl_);
}

bool Interval::isCertainlyGt(const Interval& other) const
{
   return Interval::Traits::isCertainlyGt(impl_, other.impl_);
}

bool Interval::isPossiblyEqZero() const
{
   return isPossiblyEq(Interval::zero());
}

bool Interval::isPossiblyLeZero() const
{
   return isPossiblyLe(Interval::zero());
}

bool Interval::isPossiblyLtZero() const
{
   return isPossiblyLt(Interval::zero());
}

bool Interval::isPossiblyGeZero() const
{
   return isPossiblyGe(Interval::zero());
}

bool Interval::isPossiblyGtZero() const
{
   return isPossiblyGt(Interval::zero());
}

bool Interval::isCertainlyEqZero() const
{
   return isCertainlyEq(Interval::zero());
}

bool Interval::isCertainlyLeZero() const
{
   return isCertainlyLe(Interval::zero());
}

bool Interval::isCertainlyLtZero() const
{
   return isCertainlyLt(Interval::zero());
}

bool Interval::isCertainlyGeZero() const
{
   return isCertainlyGe(Interval::zero());
}

bool Interval::isCertainlyGtZero() const
{
   return isCertainlyGt(Interval::zero());
}

bool Interval::isDisjoint(const Interval& other) const
{
   return Interval::Traits::isDisjoint(impl_, other.impl_);
}

bool Interval::overlaps(const Interval& other) const
{
   return Interval::Traits::overlaps(impl_, other.impl_);
}

double Interval::distance(const Interval& other) const
{
   return Interval::Traits::distance(impl_, other.impl_);
}

double Interval::gap(const Interval& other) const
{
   if (isEmpty() || other.isEmpty())
      return Double::inf();

   if (isCertainlyLt(other))
   {
      Double::rndUp();
      return other.left() - right();
   }

   if (isCertainlyGt(other))
   {
      Double::rndUp();
      return left() - other.right();
   }

   return 0.0;
}

Interval& Interval::operator&=(const Interval& other)
{
   Interval::Traits::inter_assign(impl_, other.impl_);  
   return *this;
}

Interval operator&(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::inter(x.impl_, y.impl_));
}

Interval& Interval::operator|=(const Interval& other)
{
   Interval::Traits::hull_assign(impl_, other.impl_);     
   return *this;
}

Interval operator|(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::hull(x.impl_, y.impl_));   
}

Interval round(const Interval& x)
{
   return Interval(Interval::Traits::round(x.impl_));      
}

Interval& Interval::operator+=(const Interval& other)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_ADD");
#endif

   Interval::Traits::addAssign(impl_, other.impl_);     
   return *this;
}

Interval operator+(const Interval& x, const Interval& y)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_ADD");
#endif

   return Interval(Interval::Traits::add(x.impl_, y.impl_));      
}

Interval addPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_ADD_PX");
#endif

   return Interval(Interval::Traits::addPX(x.impl_, y.impl_, z.impl_));
}

Interval addPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_ADD_PY");
#endif

   return Interval(Interval::Traits::addPY(x.impl_, y.impl_, z.impl_));   
}

Interval addPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_ADD_PZ");
#endif

   return Interval(Interval::Traits::addPZ(x.impl_, y.impl_, z.impl_));   
}

Interval& Interval::operator-=(const Interval& other)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_SUB");
#endif

   Interval::Traits::subAssign(impl_, other.impl_);     
   return *this;   
}

Interval operator-(const Interval& x, const Interval& y)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_SUB");
#endif

   return Interval(Interval::Traits::sub(x.impl_, y.impl_));      
}

Interval subPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_SUB_PX");
#endif

   return Interval(Interval::Traits::subPX(x.impl_, y.impl_, z.impl_));
}

Interval subPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_SUB_PY");
#endif

   return Interval(Interval::Traits::subPY(x.impl_, y.impl_, z.impl_));   
}

Interval subPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_SUB_PZ");
#endif

   return Interval(Interval::Traits::subPZ(x.impl_, y.impl_, z.impl_));   
}

Interval operator-(const Interval& x)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_USUB");
#endif

   return Interval(Interval::Traits::usub(x.impl_));
}

Interval usubPX(const Interval& x, const Interval& y)
{

#if STAT_ON
   ++Stat::getIntStat("ITV_OP_USUB_PX");
#endif

   return Interval(Interval::Traits::usubPX(x.impl_, y.impl_));   
}

Interval usubPY(const Interval& x, const Interval& y)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_USUB_PY");
#endif

   return Interval(Interval::Traits::usubPY(x.impl_, y.impl_));      
}

Interval& Interval::operator*=(const Interval& other)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_MUL");
#endif

   Interval::Traits::mulAssign(impl_, other.impl_);  
   return *this;   
}

Interval operator*(const Interval& x, const Interval& y)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_MUL");
#endif

   return Interval(Interval::Traits::mul(x.impl_, y.impl_));      
}

Interval mulPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_MUL_PX");
#endif

   return Interval(Interval::Traits::mulPX(x.impl_, y.impl_, z.impl_));
}

Interval mulPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_MUL_PY");
#endif

   return Interval(Interval::Traits::mulPY(x.impl_, y.impl_, z.impl_));   
}

Interval mulPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
#if STAT_ON
   ++Stat::getIntStat("ITV_OP_MUL_PZ");
#endif

   return Interval(Interval::Traits::mulPZ(x.impl_, y.impl_, z.impl_));   
}

Interval& Interval::operator/=(const Interval& other)
{
   Interval::Traits::divAssign(impl_, other.impl_);
   return *this;   
}

Interval operator/(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::div(x.impl_, y.impl_));      
}

Interval divPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPX(x.impl_, y.impl_, z.impl_));
}

Interval divPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPY(x.impl_, y.impl_, z.impl_));   
}

Interval divPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::divPZ(x.impl_, y.impl_, z.impl_));   
}

Interval sqr(const Interval& x)
{
   return Interval(Interval::Traits::sqr(x.impl_));
}

Interval sqrPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrPX(x.impl_, y.impl_));
}

Interval sqrPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrPY(x.impl_, y.impl_));
}

Interval sqrt(const Interval& x)
{
   return Interval(Interval::Traits::sqrt(x.impl_));
}

Interval sqrtPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrtPX(x.impl_, y.impl_));
}

Interval sqrtPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sqrtPY(x.impl_, y.impl_));
}

Interval pow(const Interval& x, int n)
{
   return Interval(Interval::Traits::pow(x.impl_, n));
}

Interval powPX(const Interval& x, int n, const Interval& y)
{
   return Interval(Interval::Traits::powPX(x.impl_, n, y.impl_));
}

Interval powPY(const Interval& x, int n, const Interval& y)
{
   return Interval(Interval::Traits::powPY(x.impl_, n, y.impl_));   
}

Interval exp(const Interval& x)
{
   return Interval(Interval::Traits::exp(x.impl_));
}

Interval expPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::expPX(x.impl_, y.impl_));
}

Interval expPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::expPY(x.impl_, y.impl_));
}

Interval log(const Interval& x)
{
   return Interval(Interval::Traits::log(x.impl_));
}

Interval logPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::logPX(x.impl_, y.impl_));
}

Interval logPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::logPY(x.impl_, y.impl_));
}

Interval sin(const Interval& x)
{
   return Interval(Interval::Traits::sin(x.impl_));
}

Interval sinPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sinPX(x.impl_, y.impl_));
}

Interval sinPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sinPY(x.impl_, y.impl_));
}

Interval cos(const Interval& x)
{
   return Interval(Interval::Traits::cos(x.impl_));
}

Interval cosPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::cosPX(x.impl_, y.impl_));
}

Interval cosPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::cosPY(x.impl_, y.impl_));
}

Interval tan(const Interval& x)
{
   return Interval(Interval::Traits::tan(x.impl_));
}

Interval tanPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::tanPX(x.impl_, y.impl_));
}

Interval tanPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::tanPY(x.impl_, y.impl_));
}

Interval abs(const Interval& x)
{
   return Interval(Interval::Traits::abs(x.impl_));
}

Interval absPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::absPX(x.impl_, y.impl_));
}

Interval absPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::absPY(x.impl_, y.impl_));
}

Interval min(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::min(x.impl_, y.impl_));
}

Interval minPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPX(x.impl_, y.impl_, z.impl_));
}

Interval minPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPY(x.impl_, y.impl_, z.impl_));
}

Interval minPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::minPZ(x.impl_, y.impl_, z.impl_));
}

Interval max(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::max(x.impl_, y.impl_));
}

Interval maxPX(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPX(x.impl_, y.impl_, z.impl_));
}

Interval maxPY(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPY(x.impl_, y.impl_, z.impl_));
}

Interval maxPZ(const Interval& x, const Interval& y,
                      const Interval& z)
{
   return Interval(Interval::Traits::maxPZ(x.impl_, y.impl_, z.impl_));
}

Interval sgn(const Interval& x)
{
   return Interval(Interval::Traits::sgn(x.impl_));
}

Interval sgnPX(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sgnPX(x.impl_, y.impl_));
}

Interval sgnPY(const Interval& x, const Interval& y)
{
   return Interval(Interval::Traits::sgnPY(x.impl_, y.impl_));
}

} // namespace
