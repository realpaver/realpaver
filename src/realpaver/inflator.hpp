// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_INFLATOR_HPP
#define REALPAVER_INFLATOR_HPP

#include "realpaver/box.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of inflators used to inflate intervals and interval vectors.
 * 
 * Given an interval x and two real numbers delta > 1.0 and chi > 0.0,
 * an inflator calculates the interval m(x) + delta*(x - m(x)) + chi*[-1,1]
 * where m(x) is the midpoint of x.
 *****************************************************************************/
class Inflator {
public:
   // constructors with default values of delta and chi
   Inflator();

   // constructor
   Inflator(const double& delta, const double& chi);

   // getters
   double delta() const;
   double chi() const;

   // setters
   void setDelta(const double& delta);
   void setChi(const double& chi);

   // inflation methods
   Interval inflate(const Interval& x) const;
   Box inflate(const Box& B) const;

private:
   double delta_;
   double chi_;
};

inline double Inflator::delta() const
{
   return delta_;
}

inline double Inflator::chi() const
{
   return chi_;
}

inline void Inflator::setDelta(const double& delta)
{
   ASSERT(delta > 1.0, "bad inflation factor delta " << delta);

   delta_ = delta;
}

inline void Inflator::setChi(const double& chi)
{
   ASSERT(chi > 0.0, "bad inflation factor chi " << chi);

   chi_ = chi;
}

} // namespace

#endif
