// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_BCO_LOCAL_HPP
#define REALPAVER_BCO_LOCAL_HPP

#include "realpaver/box.hpp"
#include "realpaver/point.hpp"

namespace realpaver {

/*****************************************************************************
 * Pure abstract class of local optimization solvers.
 *****************************************************************************/
class BcoLocal {
public:
   // virtual destructor
   virtual ~BcoLocal();

   // local optimization from x in region
   // returns true if the local solver converges
   virtual bool minimize(const Box& region, const Point& x) = 0;

   // returns the initial point given as input of minimize
   virtual Point initialPoint() const = 0;

   // returns the value of the objective function at the initial point
   virtual double initialBound() const = 0;

   // returns the point calculated by minimize
   virtual Point finalPoint() const = 0;

   // returns the value of the objective function at the final point
   virtual double finalBound() const = 0;
};

} // namespace

#endif
