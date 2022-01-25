// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_STOPPING_HPP
#define REALPAVER_STOPPING_HPP

#include "realpaver/interval.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of improvement factors used to stop a sequence of nested intervals.
 *****************************************************************************/
class Improvement {
public:
   // constructor given a percentage f in [0,100]
   Improvement(const double& f = 0.0);

   // returns true if the width of x is less than the width of old multiplied
   // by f percent where f is the factor enclosed in this
   bool test(const Interval& x, const Interval& old) const;

   // returns the improvement factor between the width of x and the width
   // of old
   static Improvement calculate(const Interval& x, const Interval& old);

   // gets the value
   double val() const;

   // modifies the value
   void setVal(const double& f);

private:
   double f_;     // percentage in [0,100]
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Improvement& imp);

inline double Improvement::val() const
{
   return f_;
}

/*****************************************************************************
 * Class of precisions used to check the tightness of intervals.
 * 
 * The precision of an interval x is equal to
 *     # -1 if x is empty
 *     # 0 if x is canonical
 *     # the width of x if the precision is ABSOLUTE
 *     # otherwise
 *         ## the width of x if x is included in [-1,+1]
 *         ## the relative width of x otherwise
 *****************************************************************************/
class Precision {
public:
   // constructor
   Precision(const double& val = 0.0, bool absolute = true);

   // magnitude
   double val() const;

   // returns true if x has this precision
   bool test(const Interval& x) const;

   // returns true if this precision is absolute
   bool isAbsolute() const;

   // returns true if this precision is adaptive
   bool isAdaptive() const;

   // returns the precision of x
   static double precisionOf(const Interval& x);

   // creates an absolute precision
   static Precision absolute(const double& val);

   // creates an adaptive precision
   static Precision adaptive(const double& val);

private:
   double val_;   // magnitude
   bool abs_;     // true for an absolute precision
};

// output on a stream
std::ostream& operator<<(std::ostream& os, const Precision& p);

inline double Precision::val() const
{
   return val_;
}

inline bool Precision::isAbsolute() const
{
   return abs_;
}

inline bool Precision::isAdaptive() const
{
   return !abs_;
}

inline Precision Precision::absolute(const double& val)
{
   return Precision(val, true);
}

inline Precision Precision::adaptive(const double& val)
{
   return Precision(val, false);
}

} // namespace

#endif
