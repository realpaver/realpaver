// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_box.hpp"

namespace realpaver {

Box::Box() : v_()
{}

Box::Box(size_t n, const Interval& x) : v_(n,x)
{}

Box::Box(const std::initializer_list<Interval>& l) : v_()
{
   for (auto x : l)
      v_.push_back(x);
}

const Interval& Box::at(size_t i) const
{
   ASSERT(i < size(), "access out of range in a box @ " << i);

   return v_[i];
}

void Box::set(size_t i, const Interval& x)
{
   ASSERT(i < size(), "access out of range in a box @ " << i);

   v_[i] = x;
}

void Box::set(const Variable& v, const Interval& x)
{
   ASSERT(v.id() < size(), "access out of range in a box @ " << v.id());

   v_[v.id()] = x;
}

void Box::setAll(const Interval& x)
{
   for (size_t i=0; i<size(); ++i)
      v_[i] = x;
}

void Box::set(const Box& other, const Scope& s)
{
   for (auto v : s)
      set(v.id(), other[v.id()]);
}

void Box::setHull(const Box& other, const Scope& s)
{
   for (auto v : s)
   {
      Interval x( other[v.id()] | (*this)[v.id()] );
      set(v.id(), x);
   }
}

Box* Box::clone() const
{
   return new Box(*this);
}

size_t Box::hashCode() const
{
   size_t h = 0;

   if (size() != 0)
   {
      h = v_[0].hashCode();
      for (size_t i=1; i<size(); ++i)
         h = Hash2(h, v_[i].hashCode());
   }

   return h;
}

bool Box::isEmpty() const
{
   for (size_t i=0; i<size(); ++i)
      if (v_[i].isEmpty())
         return true;

   return false;
}

bool Box::isFinite() const
{
   for (size_t i=0; i<size(); ++i)
      if (v_[i].isInf())
         return false;

   return true;
}

bool Box::isInf() const
{
   return !isFinite();
}

bool Box::contains(const Box& other) const
{
   ASSERT(size() == other.size(),
          "inclusion test of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!v_[i].contains(other.v_[i]))
         return false;

   return true;
}

bool Box::strictlyContains(const Box& other) const
{
   ASSERT(size() == other.size(),
          "inclusion test of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!v_[i].strictlyContains(other.v_[i]))
         return false;

   return true;
}

bool Box::containsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!v_[i].containsZero())
         return false;

   return true;   
}

bool Box::strictlyContainsZero() const
{
   for (size_t i=0; i<size(); ++i)
      if (!v_[i].strictlyContainsZero())
         return false;

   return true;      
}

bool Box::isDisjoint(const Box& other) const
{
   ASSERT(size() == other.size(),
          "disjunction test of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (v_[i].isDisjoint(other.v_[i]))
         return true;

   return false;
}

bool Box::overlaps(const Box& other) const
{
   ASSERT(size() == other.size(),
          "intersection test of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      if (!v_[i].overlaps(other.v_[i]))
         return false;

   return true;
}

Point Box::midpoint() const
{
   Point P( size() );
   for (size_t i=0; i<size(); ++i)
      P.set(i, v_[i].midpoint());

   return P;
}

double Box::oneNorm() const
{
   Interval norm( 0.0 );

   for (size_t i=0; i<size(); ++i)
      norm += v_[i].mag();

   return norm.right();
}

double Box::infNorm() const
{
   double norm = 0.0, m;

   for (size_t i=0; i<size(); ++i)
   {
      m = v_[i].mag();
      if (m > norm) norm = m;
   }

   return norm;
}

double Box::width() const
{
   double wid = 0.0, w;

   for (size_t i=0; i<size(); ++i)
   {
      w = v_[i].width();
      if (w > wid) wid = w;
   }

   return wid;
}

   // intersection
Box& Box::operator&=(const Box& other)
{
   ASSERT(size() == other.size(),
          "intersection of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      v_[i] &= other.v_[i];

   return *this;
}

Box operator&(const Box& x, const Box& y)
{
   Box z( x );
   z &= y;
   return z;
}

Box& Box::operator|=(const Box& other)
{
   ASSERT(size() == other.size(), "hull of two boxes with different sizes");

   for (size_t i=0; i<size(); ++i)
      v_[i] |= other.v_[i];

   return *this;   
}

Box operator|(const Box& x, const Box& y)
{
   Box z( x );
   z |= y;
   return z;
}

std::ostream& operator<<(std::ostream& os, const Box& x)
{
   os << "(";
   for (size_t i=0; i<x.size(); ++i)
   {
      if (i != 0) os << ", ";
      os << x[i];
   }
   os << ")";
   return os;
}

} // namespace
