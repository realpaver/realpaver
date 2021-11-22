// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_POINT_HPP
#define REALPAVER_POINT_HPP

#include <iostream>
#include <vector>
#include "realpaver_double.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of vectors of floating-point numbers in double precision.
 *****************************************************************************/
class Point {
public:
   // constructors
   Point();
   Point(size_t n, const double& x = 0.0);
   Point(const std::initializer_list<double>& l);

   // copy
   Point(const Point&) = default;
   Point& operator=(const Point&) = delete;

   // size of this vector
   size_t size() const;

   // constant access to the i-th element
   const double& operator[](size_t i) const;

   // constant and safe access to the i-th element
   const double& at(size_t i) const;

   // assignment of the i-th element to x
   void set(size_t i, const double& x);

   // assignment of all the elements to x
   void setAll(const double& x);

   // inserts an interval at the end
   void push(const double& x);

   // hash code
   size_t hashCode() const;

   // test functions
   bool isFinite() const;
   bool isInf() const;
   bool isNan() const;

   // ||P||_1 = sum_i |P_i|
   double oneNorm() const;

   // ||P||_inf = max_i |P_i|
   double infNorm() const;

private:
   std::vector<double> v_;
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Point& P);

inline size_t Point::size() const
{
   return v_.size();
}

inline void Point::push(const double& x)
{
   v_.push_back(x);
}

inline const double& Point::operator[](size_t i) const
{
   return v_[i];
}

} // namespace

#endif
