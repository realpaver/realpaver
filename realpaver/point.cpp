// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/common.hpp"
#include "realpaver/point.hpp"

namespace realpaver {

Point::Point() : v_()
{}

Point::Point(size_t n, const double& x) : v_(n,x)
{}

Point::Point(const std::initializer_list<double>& l) : v_()
{
   for (auto x : l)
      v_.push_back(x);
}

double Point::at(size_t i) const
{
   ASSERT(i < size(), "access out of range in a point @ " << i);

   return v_[i];
}

void Point::set(size_t i, const double& x)
{
   ASSERT(i < size(), "access out of range in a point @ " << i);

   v_[i] = x;
}

void Point::setAll(const double& x)
{
   for (size_t i=0; i<size(); ++i)
      v_[i] = x;
}

size_t Point::hashCode() const
{
   size_t h = 0;

   if (size() != 0)
   {
      h = Hash1(v_[0]);

      for (size_t i=1; i<size(); ++i)
         h = Hash2(h, Hash1(v_[i]));
   }

   return h;
}

size_t Point::size() const
{
   return v_.size();
}

void Point::push(const double& x)
{
   v_.push_back(x);
}

double Point::operator[](size_t i) const
{
   return v_[i];
}

bool Point::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isInf(v_[i]))
         return false;

   return true;
}

bool Point::isInf() const
{
   return !isFinite();
}

bool Point::isNan() const
{
   for (size_t i=0; i<size(); ++i)
      if (Double::isNan(v_[i]))
         return true;

   return true;
}

double Point::oneNorm() const
{
   double norm( 0.0 );

   for (size_t i=0; i<size(); ++i)
      norm += Double::abs(v_[i]);

   return norm;
}

double Point::infNorm() const
{
   double norm = 0.0, m;

   for (size_t i=0; i<size(); ++i)
   {
      m = Double::abs(v_[i]);
      if (m > norm) norm = m;
   }

   return norm;
}

std::ostream& operator<<(std::ostream& os, const Point& P)
{
   os << "(";
   for (size_t i=0; i<P.size(); ++i)
   {
      if (i != 0) os << ", ";
      os << P[i];
   }
   os << ")";
   return os;
}

} // namespace
