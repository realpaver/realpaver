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
 * @file   Range.cpp
 * @brief  Finite integer interval bounded by safe integers
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include <cmath>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Interval.hpp"
#include "realpaver/Range.hpp"

namespace realpaver {

Range::Range()
      : l_(Integer::MIN()),
        r_(Integer::MAX())
{}

Range::Range(const int& a)
      : l_(a),
        r_(a)
{}

Range::Range(const int& a, const int& b)
      : l_(a),
        r_(b)
{}

Range::Range(const Integer& a)
      : l_(a),
        r_(a)
{}

Range::Range(const Integer& a, const Integer& b)
      : l_(a),
        r_(b)
{}

Integer Range::MIN()
{
   return Integer::MIN();
}

Integer Range::MAX()
{
   return Integer::MAX();
}

Integer Range::left() const
{
   return l_;
}

Integer Range::right() const
{
   return r_;
}

unsigned long  Range::nbElems() const
{
   return isEmpty() ? 0 : (unsigned long)r_.toInt() - l_.toInt() + 1;
}

bool Range::isEmpty() const
{
   return l_ > r_;
}

bool Range::isSingleton() const
{
   return l_ == r_;
}

bool Range::isZero() const
{
   return l_ == 0 && r_ == 0;
}

bool Range::contains(const Integer& a) const
{
   return l_ <= a && a <= r_;
}

bool Range::strictlyContains(const Integer& a) const
{
   return l_ < a && a < r_;
}

bool Range::containsZero() const
{
   return l_ <= 0 && 0 <= r_;
}

bool Range::strictlyContainsZero() const
{
   return l_ < 0 && 0 < r_;
}

bool Range::contains(const Range& other) const
{
   return l_<=other.l_ && other.r_<=r_;      
}

bool Range::strictlyContains(const Range& other) const
{
   return l_<other.l_ && other.r_<r_;         
}

bool Range::isSetEq(const Range& other) const
{
   return l_==other.l_ && other.r_==r_;            
}

bool Range::isSetNeq(const Range& other) const
{
   return l_!=other.l_ || other.r_!=r_;
}

bool Range::isPossiblyEq(const Range& other) const
{
   return max(l_,other.l_) <= min(r_,other.r_);
}

bool Range::isPossiblyNeq(const Range& other) const
{
   return !(isSingleton() && isSetEq(other));
}

bool Range::isPossiblyLe(const Range& other) const
{
   return l_ <= other.r_;
}

bool Range::isPossiblyLt(const Range& other) const
{
   return l_ < other.r_;
}

bool Range::isPossiblyGe(const Range& other) const
{
   return r_ >= other.l_;
}

bool Range::isPossiblyGt(const Range& other) const
{
   return r_ > other.l_;
}

bool Range::isCertainlyEq(const Range& other) const
{
   return isSingleton() && isSetEq(other);
}

bool Range::isCertainlyNeq(const Range& other) const
{
   return max(l_,other.l_) > min(r_,other.r_);
}

bool Range::isCertainlyLe(const Range& other) const
{
   return r_ <= other.l_;
}

bool Range::isCertainlyLt(const Range& other) const
{
   return r_ < other.l_;
}

bool Range::isCertainlyGe(const Range& other) const
{
   return l_ >= other.r_;
}

bool Range::isCertainlyGt(const Range& other) const
{
   return l_ > other.r_;
}

bool Range::isDisjoint(const Range& other) const
{
   return max(l_,other.l_) > min(r_,other.r_);   
}

bool Range::overlaps(const Range& other) const
{
   return max(l_,other.l_) <= min(r_,other.r_);   
}

bool Range::isCertainlyLeZero() const
{
   return r_ <= 0;
}

bool Range::isCertainlyLtZero() const
{
   return r_ < 0;
}

bool Range::isCertainlyGeZero() const
{
   return l_ >= 0;
}

bool Range::isCertainlyGtZero() const
{
   return r_ > 0;
}

bool Range::isJoinable(const Range& other) const
{
   if (isEmpty() || other.isEmpty())
      return false;

   if (overlaps(other))
      return true;

   if (isCertainlyLt(other))
      return r_ + 1 == other.l_;

   return other.r_ + 1 == l_;
}

Range Range::roundOutward(const Interval& x)
{
   if (x.isEmpty())
      return Range::emptyset();

   Integer lo = Integer::floor(x.left()),
           up = Integer::ceil(x.right());

   return Range(lo, up);
}

Range Range::roundInward(const Interval& x)
{
   if (x.isEmpty())
      return Range::emptyset();

   Integer lo = Integer::ceil(x.left()),
           up = Integer::floor(x.right());

   return Range(lo, up);
}

Interval Range::toInterval() const
{
   if (isEmpty())
      return Interval::emptyset();

   else
      return Interval(l_.toDouble(), r_.toDouble());
}

Range Range::lessThan(const Integer& a)
{
   return Range(Range::MIN(), a);
}

Range Range::strictlyLessThan(const Integer& a)
{
   return Range(Range::MIN(), a-1);
}

Range Range::moreThan(const Integer& a)
{
   return Range(a, Range::MAX());
}

Range Range::strictlyGreaterThan(const Integer& a)
{
   return Range(a+1, Range::MAX());
}

Range Range::emptyset()
{
   static Range x(MAX(), MIN());
   return x;
}

Range Range::universe()
{
   static Range x(MIN(), MAX());
   return x;
}

Range Range::positive()
{
   static Range x(0, MAX());
   return x;
}

Range Range::negative()
{
   static Range x(MIN(), 0);
   return x;
}

void Range::setLeft(const Integer& a)
{
   l_ = a;
}

void Range::setRight(const Integer& a)
{
   r_ = a;
}

void Range::setEmpty()
{
   l_ = MAX();
   r_ = MIN();
}

void Range::setUniverse()
{
   l_ = MIN();
   r_ = MAX();
}

Integer Range::midpoint() const
{
   if (isSingleton())
      return l_;
   else
   {
      Integer a = l_ / 2,
              b = r_ / 2,
              c = 0;

      if (l_.isOdd() && r_.isOdd())
      {
         if (l_>0)
         {
            if (r_>0) c = 1;
         }
         else
         {
            if (r_<0) c = -1;
         }
      }

      return a + b + c;
   }
}

Integer Range::mig() const
{
   if (isEmpty())
      return -1;

   else if (l_ >= 0)
      return l_;

   else if (r_ < 0)
      return abs(r_);

   else
      return 0;
}

Integer Range::mag() const
{
   if (isEmpty())
      return -1;

   else
      return max(abs(l_), abs(r_));   
}

Range& Range::operator&=(const Range& other)
{
   if (!isEmpty())
   {
      if (other.isEmpty())
         setEmpty();

      else
      {
         l_ = max(l_, other.l_);
         r_ = min(r_, other.r_);
      }
   }
   return *this;
}

Range operator&(const Range& x, const Range& y)
{
   Range res(x);
   res &= y;
   return res;
}

Range& Range::operator|=(const Range& other)
{
   if (isEmpty())
   {
      *this = other;
   }
   else if (!other.isEmpty())
   {
      l_ = min(l_, other.l_);
      r_ = max(r_, other.r_);
   }
   return *this;   
}

Range operator|(const Range& x, const Range& y)
{
   Range res(x);
   res |= y;
   return res;
}

Range& Range::operator+=(const Range& other)
{
   if (!isEmpty())
   {
      if (other.isEmpty())
      {
         setEmpty();
      }
      else
      {
         l_ += other.l_;
         r_ += other.r_;
      }
   }
   return *this;
}

Range operator+(const Range& x, const Range& y)
{
   Range res(x);
   res += y;
   return res;
}

Range addPX(const Range& x, const Range& y, const Range& z)
{
   return x & (z - y);
}

Range addPY(const Range& x, const Range& y, const Range& z)
{
   return y & (z - x);
}

Range addPZ(const Range& x, const Range& y, const Range& z)
{
   return z & (x + y);
}

Range& Range::operator-=(const Range& other)
{
   if (!isEmpty())
   {
      if (other.isEmpty())
         setEmpty();

      else
      {
         l_ -= other.r_;
         r_ -= other.l_;
      }
   }
   return *this;   
}

Range operator-(const Range& x, const Range& y)
{
   Range res(x);
   res -= y;
   return res;
}

Range subPX(const Range& x, const Range& y, const Range& z)
{
   return x & (y + z);
}

Range subPY(const Range& x, const Range& y, const Range& z)
{
   return y & (x - z);
}

Range subPZ(const Range& x, const Range& y, const Range& z)
{
   return z & (x - y);
}

Range operator-(const Range& x)
{
   return Range(-x.r_, -x.l_);
}

Range usubPX(const Range& x, const Range& y)
{
   return x & (-y);   
}

Range usubPY(const Range& x, const Range& y)
{
   return y & (-x);
}

Range& Range::operator*=(const Range& other)
{
   if (!isEmpty())
   {
      if (other.isEmpty())
         setEmpty();

      else
      {
         Integer a1 = l_ * other.l_,
                 a2 = l_ * other.r_,
                 a3 = r_ * other.l_,
                 a4 = r_ * other.r_;

         l_ = min(a1, min(a2, min(a3, a4)));
         r_ = max(a1, max(a2, max(a3, a4)));
      }
   }
   return *this;   
}

Range operator*(const Range& x, const Range& y)
{
   Range res(x);
   res *= y;
   return res;
}

Range mulPX(const Range& x, const Range& y, const Range& z)
{
   if (x.isEmpty() || y.isEmpty() || z.isEmpty())
      return Range::emptyset();

   else
   {
      if (y.containsZero())
      {
         if (z.containsZero())
            return x;

         else
         {
            bool bl = (y.l_ == 0),
                 br = (y.r_ == 0);
            
            if (bl && br)
               return Range::emptyset();

            else if (bl)
            {
               Integer a, b;

               if (z.isCertainlyGtZero())
               {
                  // ex : [3,5] / [0,2] -> [3/2,5/1]
                  a = divCeil(z.l_, y.r_);
                  b = z.r_;
               }
               else
               {
                  // ex : [-5,-3] / [0,2] -> [-5/1,-3/2]
                  a = z.l_;
                  b = divFloor(z.r_, y.r_);
               }

               return x & Range(a,b);
            }
            else if (br)
            {
               Integer a, b;

               if (z.isCertainlyGtZero())
               {
                  // ex : [3,5] / [-2,0] -> [5/-1,3/-2]
                  a = -z.r_;
                  b = divFloor(z.l_, y.l_);
               }
               else
               {
                  // ex : [-5,-3] / [-2,0] -> [-3/-2,-5/-1]
                  a = divCeil(z.r_, y.l_);
                  b = -z.l_;
               }

               return x & Range(a,b);
            }
            else
            {
               // y strictly contains 0
               Integer a, b, c, d;

               if (z.isCertainlyGtZero())
               {
                  // ex : [4,7] / [-2,3] -> [7/-1,4/-2] u [4/3,7/1]
                  a = -z.r_;
                  b = divFloor(z.l_, y.l_);
                  c = divCeil(z.l_, y.r_);
                  d = z.r_;
               }
               else
               {
                  // ex : [-7,-4] / [-2,3] -> [-7/1,-4/3] u [-4/-2,-7/-1]
                  a = z.l_;
                  b = divFloor(z.r_, y.r_);
                  c = divCeil(z.r_, y.l_);
                  d = -z.l_;       
               }

               return (x & Range(a, b)) | (x & Range(c, d));
            }
         }
      }
      else
      {
         // 0 does not belong to y
         return x & (z / y);
      }
   }
}

Range mulPY(const Range& x, const Range& y, const Range& z)
{
   return mulPX(y, x, z);
}

Range mulPZ(const Range& x, const Range& y, const Range& z)
{
   return z & (x * y);
}

Range sqr(const Range& x)
{
   if (x.isEmpty())
      return Range::emptyset();
   else
   {
      Integer a = x.mag();

      if (x.l_ > 0)
         return Range(sqr(x.l_), sqr(x.r_));

      else if (x.r_ < 0)
         return Range(sqr(x.r_), sqr(x.l_));

      else
         return Range(0, sqr(a));
   }
}

Range sqrPX(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty() || y.r_ < 0)
      return Range::emptyset();

   else if (y.l_ > 0)
   {
      Integer a = sqrtCeil(y.l_),
              b = sqrtFloor(y.r_);

      return (x & Range(-b, -a)) | (x & Range(a, b));
   }
   else
   {
      Integer b = sqrtFloor(y.r_);
      return x & Range(-b, b);
   }
}

Range sqrPY(const Range& x, const Range& y)
{
   return y & sqr(x);
}

Range min(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty())
      return Range::emptyset();

   else
      return Range(min(x.l_, y.l_),min(x.r_, y.r_));
}

Range minPX(const Range& x, const Range& y, const Range& z)
{
   if (x.isEmpty() || y.isEmpty() || z.isEmpty())
      return Range::emptyset();

   else
   {
      if (z.l_ > y.r_)                    /* z:            |-----| */
         return Range::emptyset();        /* y: |-----|            */

      else if (y.l_ > z.r_)               /* z: |-----|            */
         return x & z;                    /* y:            |-----| */

      else
         return x & Range::moreThan(z.l_);
   }
}

Range minPY(const Range& x, const Range& y, const Range& z)
{
   return minPX(y, x, z);
}

Range minPZ(const Range& x, const Range& y, const Range& z)
{
   return z & min(x, y);
}

Range max(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty())
      return Range::emptyset();

   else
      return Range(max(x.l_, y.l_), max(x.r_, y.r_));
}

Range maxPX(const Range& x, const Range& y, const Range& z)
{
   if (x.isEmpty() || y.isEmpty() || z.isEmpty())
      return Range::emptyset();

   else
   {
      if (z.l_ > y.r_)               /* z :            |-----| */
         return x & z;               /* y : |-----|            */

      else if (y.l_ > z.r_)          /* y :            |-----| */
         return Range::emptyset();   /* z : |-----|            */

      else
         return x & Range::lessThan(z.r_);
   }
}

Range maxPY(const Range& x, const Range& y, const Range& z)
{
   return maxPX(y, x, z);
}

Range maxPZ(const Range& x, const Range& y, const Range& z)
{
   return z & max(x, y);
}

Range abs(const Range& x)
{   
   if (x.isEmpty())
      return Range::emptyset();

   else if (x.l_ >= 0)
      return x;

   else if (x.r_ < 0)
      return -x;

   else
      return Range(0, x.mag());
}

Range absPX(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty())
      return Range::emptyset();

   else
   {
      if (y.l_ > 0)
         return (x & y) | (x & (-y));

      else if (y.r_ >= 0)
         return x & Range(-y.r_, y.r_);

      else
         return Range::emptyset();
   }
}

Range absPY(const Range& x, const Range& y)
{
   return y & abs(x);
}

Range sgn(const Range& x)
{   
   if (x.isEmpty())
      return Range::emptyset();

   else if (x.strictlyContainsZero())
      return Range(-1, 1);

   else if (x.isZero())
      return Range(0);

   else if (x.right() > 0)
      return Range(0, 1);

   else
      return Range(-1, 0);
}

Range sgnPX(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty())
      return Range::emptyset();

   else
   {
      int neg = x.contains(Integer(-1)) ? (1 << 0) : 0;
      int pos = x.contains(Integer(1))  ? (1 << 1) : 0;
      int zro = x.contains(Integer(0))  ? (1 << 2) : 0;
      int s = neg | pos | zro;

      Integer lo, up;

      switch(s)
      {
         case 0:     // -1, 0, 1 not in x => empty
         {
            return Range::emptyset();
         }
         case 1:     // -1 in x
         {
            if (y.left() >= 0)
               return Range::emptyset();

            else
            {
               lo = y.left();
               up = min(y.right(),0);
               return Range(lo,up);
            }
         }
         case 2:     // 1 in x
         {
            if (y.right() <= 0)
               return Range::emptyset();

            else
            {
               lo = max(0,y.left());
               up = y.right();
               return Range(lo, up);
            }
         }
         case 5:     // -1, 0 in x
         {
            lo = y.left();
            up = min(y.right(),0);
            return Range(lo, up);
         }
         case 6:     // 0, 1 in x
         {
            lo = max(0,y.left());
            up = y.right();
            return Range(lo, up);
         }
         case 7:     // -1, 0, 1 in x => no contraction
         {
            return y;
         }
      }
      return Range::universe();
   }
}

Range sgnPY(const Range& x, const Range& y)
{
   return y & sgn(x);
}

Range& Range::operator/=(const Range& other)
{
   return *this = (*this) / other;
}

Range operator/(const Range& x, const Range& y)
{
   if (x.isEmpty() || y.isEmpty())
      return Range::emptyset();

   else if (y.containsZero())
   {
      if (y.isZero())
         return Range::emptyset();
      
      else if (y.l_ == 0)
         return x / Range(1, y.r_);

      else if (y.r_ == 0)
         return x / Range(y.l_, -1);

      else
         // y strictly contains 0 and the extrema for x / y
         // are obtained at y==1 and y==-1
         return x | (-x);
   }
   else
   {
      Integer lo, up;

      if (y.isCertainlyGtZero())
      {
         if (x.isCertainlyGeZero())
         {
            // ex : [1,4] / [3,5] -> [1/5,4/3]
            lo = divCeil(x.l_, y.r_);
            up = divFloor(x.r_, y.l_);
         }
         else if (x.isCertainlyLeZero())
         {
            // ex : [-4,-1] / [3,5] -> [-4/3,-1/5]
            lo = divCeil(x.l_, y.l_);
            up = divFloor(x.r_, y.r_);
         }
         else
         {
            // ex : [-4,1] / [3,5] -> [-4/3,1/3]
            lo = divCeil(x.l_, y.l_);
            up = divFloor(x.r_, y.l_);               
         }
      }
      else
      {
         if (x.isCertainlyGeZero())
         {
            // ex : [1,4] / [-5,-3] -> [4/-3,1/-5]
            lo = divCeil(x.r_, y.r_);
            up = divFloor(x.l_, y.l_);
         }
         else if (x.isCertainlyLeZero())
         {
            // ex : [-4,-1] / [-5,-3] -> [-1/-5,-4/-3]
            lo = divCeil(x.r_, y.l_);
            up = divFloor(x.l_, y.r_);
         }
         else
         {
            // ex : [-4,1] / [-5,-3] -> [1/-3,-4/-3]
            lo = divCeil(x.r_, y.r_);
            up = divFloor(x.l_, y.r_);               
         }
      }
      return Range(lo,up);
   }
}

Range divPX(const Range& x, const Range& y, const Range& z)
{
   return mulPZ(y, z, x);
}

Range divPY(const Range& x, const Range& y, const Range& z)
{
   return mulPX(y, z, x);
}

Range divPZ(const Range& x, const Range& y, const Range& z)
{
   return mulPY(y, z, x);
}

Range operator%(const Range& x, int n)
{
   THROW_IF(n <= 0, "Range " << x << " modulo " << n);

   Range xx = x & Range::positive();

   if (xx.isEmpty())
      return Range::emptyset();

   else if (n==1)
      return Range(0);

   else if (xx.nbElems() >= n)
      return Range(0, n-1);

   else
   {
      Integer a = xx.l_ % n,
              b = xx.r_ % n;

      if (b >= a)
         return Range(a, b);

      else
         // exact result: union [0,b] u [a,n-1]
         return Range(0, n-1);
   }
}

std::pair<Range,Range> extMod(const Range& x, int n)
{
   THROW_IF(n <= 0, "Range " << x << " modulo " << n);

   Range xx = x & Range::positive();

   if (xx.isEmpty())
      return std::make_pair(Range::emptyset(), Range::emptyset());

   else if (n==1)
      return std::make_pair(Range(0), Range::emptyset());

   else if (xx.nbElems() >= n)
      return std::make_pair(Range(0, n-1), Range::emptyset());

   else
   {
      Integer a = xx.l_ % n,
              b = xx.r_ % n;

      if (b >= a)
         return std::make_pair(Range(a, b), Range::emptyset());

      else
         return std::make_pair(Range(0, b), Range(a, n-1));
   }
}   

Range modPX(const Range& x, int n, const Range& y)
{
   THROW_IF(n <= 0, "Range " << x << " modulo " << n);

   Range xx = x & Range::positive(),
         yy = y & Range(0, n-1);

   if (xx.isEmpty() || yy.isEmpty())
      return Range::emptyset();

   else if (n == 1)
      return yy.containsZero() ? x : Range::emptyset();

   else
   {
      Integer a = xx.l_ % n,
              b = xx.r_ % n,
              lo,
              up;
 
      if (yy.contains(a))
         lo = xx.l_;

      else
      {
         Integer gap;
         
         if (a < yy.l_)
            gap = yy.l_ - a;

         else
            gap = yy.l_ - a + n;

         lo = xx.l_ + gap;
      }

      if (yy.contains(b))
         up = xx.r_;

      else
      {
         Integer gap;

         if (b > yy.r_)
            gap = b - yy.r_;

         else
            gap = b - yy.r_ + n;

         up = xx.r_ - gap;
      }

      return Range(lo, up);
   }
}

Range modPY(const Range& x, int n, const Range& y)
{
   std::pair<Range,Range> p = extMod(x, n);
   return (y & p.first) | (y & p.second);
}

std::ostream& operator<<(std::ostream& os, const Range& x)
{
   if (x.isEmpty())
      os << "[empty]";

   else if (x.isSingleton())
      os << x.left();

   else
      os << "[" << x.left() << ":" << x.right() << "]";

   return os;
}

} // namespace
