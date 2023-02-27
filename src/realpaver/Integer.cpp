///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Integer.hpp"

namespace realpaver {

Integer::Integer(const int& n) : val_(n)
{}

Integer::Integer(const size_t& n) : val_(n)
{}

Integer::Integer(const ValueType& n) : val_(n)
{}

Integer::ValueType Integer::get() const
{
   return val_;
}

Integer::ValueType Integer::MIN()
{
   return std::numeric_limits<ValueType>::min();
}

Integer::ValueType Integer::MAX()
{
   return std::numeric_limits<ValueType>::max();
}

bool Integer::isEven() const
{
   return val_ % 2 == 0;
}

bool Integer::isOdd() const
{
   return val_ % 2 != 0;   
}

double Integer::toDouble() const
{
   return static_cast<double>(val_);
}

bool operator==(const Integer& a, const Integer& b)
{
   return a.val_ == b.val_;
}

bool operator!=(const Integer& a, const Integer& b)
{
   return a.val_ != b.val_;
}

bool operator>=(const Integer& a, const Integer& b)
{
   return a.val_ >= b.val_;
}

bool operator>(const Integer& a, const Integer& b)
{
   return a.val_ > b.val_;
}

bool operator<=(const Integer& a, const Integer& b)
{
   return a.val_ <= b.val_;
}

bool operator<(const Integer& a, const Integer& b)
{
   return a.val_ < b.val_;
}

bool Integer::OutOfRangeAdd(const ValueType& a, const ValueType& b)
{
   return ((a>0) && (b>0) && (a>MAX()-b)) ||
          ((a<0) && (b<0) && (a<MIN()-b));
}

bool Integer::OutOfRangeSub(const ValueType& a, const ValueType& b)
{
   return ((a>=0) && (b<0) && (a>MAX()+b)) ||
          ((a<=0) && (b>0) && (a<MIN()+b));
}

bool Integer::OutOfRangeMul(const ValueType& a, const ValueType& b)
{
   if (a == 0 || b == 0)
      return false;
   else
   {
      if ((a<0 && b<0) || (a>0 && b>0))
         return Abs(a)>MAX()/Abs(b);

      else if (a<0)
         return a<MIN()/b;

      else
         return b<MIN()/a;
   }
}

bool Integer::OutOfRangeDiv(const ValueType& a, const ValueType& b)
{
   return (b == -1) ? OutOfRangeSub(0, a) : false;
}

Integer::ValueType Integer::Abs(const ValueType& a)
{
   if (a >= 0)
      return a;

   else if (OutOfRangeSub(0, a))
      THROW("Out of range: |" << a << "|");

   else
      return -a;
}

Integer& Integer::operator+=(const Integer& other)
{
   if (Integer::OutOfRangeAdd(val_, other.val_))
      THROW("Out of range: " << val_ << " + " << other.val_);

   val_ += other.val_;
   return *this;
}

Integer& Integer::operator-=(const Integer& other)
{
   if (Integer::OutOfRangeSub(val_, other.val_))
      THROW("Out of range: " << val_ << " - " << other.val_);

   val_ -= other.val_;
   return *this;
}

Integer& Integer::operator*=(const Integer& other)
{
   if (Integer::OutOfRangeMul(val_, other.val_))
      THROW("Out of range: " << val_ << " * " << other.val_);

   val_ *= other.val_;
   return *this;
}

Integer& Integer::operator/=(const Integer& other)
{
   val_ /= other.val_;
   return *this;
}

Integer& Integer::operator++()
{
   THROW_IF(val_ == MAX(), "Out of range: ++" << val_);

   ++val_;
   return *this;
}

Integer& Integer::operator--()
{
   THROW_IF(val_ == MIN(), "Out of range: ++" << val_);

   --val_;
   return *this;
}

Integer operator+(const Integer& a, const Integer& b)
{
   if (Integer::OutOfRangeAdd(a.val_, b.val_))
      THROW("Out of range: " << a.val_ << " + " << b.val_);

   return Integer(a.val_ + b.val_);
}

Integer operator-(const Integer& a, const Integer& b)
{
   if (Integer::OutOfRangeSub(a.val_, b.val_))
      THROW("Out of range: " << a.val_ << " - " << b.val_);

   return Integer(a.val_ - b.val_);
}

Integer operator-(const Integer& a)
{
   if (Integer::OutOfRangeSub(0, a.val_))
      THROW("Out of range: " << " - " << a.val_);

   return Integer(- a.val_);   
}

Integer operator*(const Integer& a, const Integer& b)
{
   if (Integer::OutOfRangeMul(a.val_, b.val_))
      THROW("Out of range: " << a.val_ << " * " << b.val_);

   return Integer(a.val_ * b.val_);
}

Integer operator/(const Integer& a, const Integer& b)
{
   if (b == 0)
      THROW("Integer divided by zero");

   if (Integer::OutOfRangeDiv(a.val_, b.val_))
      THROW("Out of range: " << a.val_ << " / " << b.val_);

   return Integer(a.val_ / b.val_);
}

Integer operator%(const Integer& a, int n)
{
   if (n <= 0)
      THROW("Integer modulo 0");

   return Integer(a.val_ % n);
}

Integer abs(const Integer& a)
{
   return Integer( Integer::Abs(a.val_));
}

Integer sqr(const Integer& a)
{
   if (Integer::OutOfRangeMul(a.val_, a.val_))
      THROW("Out of range: " << a.val_ << "^2");

   return Integer(a.val_ * a.val_);
}

Integer min(const Integer& a, const Integer& b)
{
   return Integer(std::min(a.val_, b.val_));
}

Integer max(const Integer& a, const Integer& b)
{
   return Integer(std::max(a.val_, b.val_));
}

Integer divFloor(const Integer& a, const Integer& b)
{
   // precond: b != 0
   Integer::ValueType res;
   if (a.val_==0) res = 0;
   else
   {
      res = a.val_/b.val_;
      bool sgn = (a.val_>0 && b.val_<0) || (a.val_<0 && b.val_>0);
      if (a.val_%b.val_ != 0 && sgn)
         res -= 1;
   }
   return res;
}

Integer divCeil(const Integer& a, const Integer& b)
{
   // precond: b != 0
   Integer::ValueType res;
   if (a.val_==0) res = 0;
   else
   {
      res = a.val_/b.val_;
      bool sgn = (a.val_>0 && b.val_>0) || (a.val_<0 && b.val_<0);
      if (a.val_%b.val_ != 0 && sgn)
         res += 1;
   }
   return Integer(res);
}

Integer sqrtFloor(const Integer& a)
{
   double x = std::sqrt((double)a.val_);
   Integer::ValueType b = Integer::ValueType(x);
   return Integer(b);
}

Integer sqrtCeil(const Integer& a)
{
   double x = std::sqrt((double)a.val_);
   Integer::ValueType b = Integer::ValueType(x);
   return (b == x) ? b : b + 1;
}

std::ostream& operator<<(std::ostream& os, const Integer& a)
{
   os << a.val_;
   return os;
}

Integer Integer::floor(const double& a)
{
   if (a < MIN() || a > MAX())
      THROW("Bad cast:" << a << " to integer");

   if (a < MIN() + 1)
      return MIN();

   if (a == MAX())
      return MAX();

   double r = std::round(a);
   if (r > a) r -= 1;

   return Integer( static_cast<ValueType>(r) );
}

Integer Integer::ceil(const double& a)
{
   if (a < MIN() || a > MAX())
      THROW("Bad cast:" << a << " to integer");

   if (a == MIN())
      return MIN();

   if (a > MAX() - 1)
      return MAX();

   double r = std::round(a);
   if (r < a) r += 1;

   return Integer( static_cast<ValueType>(r) );
}

int Integer::toInt() const
{
   if (val_>=std::numeric_limits<int>::min() &&
       val_<=std::numeric_limits<int>::max())
      return static_cast<int>(val_);

   else
      THROW("Bad cast to an int");
}

} // namespace
