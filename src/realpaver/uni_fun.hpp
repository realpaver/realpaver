// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_UNI_FUN_HPP
#define REALPAVER_UNI_FUN_HPP

#include "realpaver/Variable.hpp"

namespace realpaver {

/*****************************************************************************
 * Pure abstract class of univariate functions.
 *****************************************************************************/
class UniFun {
public:
   // virtual destructor
   virtual ~UniFun();

   // evaluates this function over x
   virtual Interval eval(const Interval& x) = 0;

   // differentiates this function over x
   virtual Interval diff(const Interval& x) = 0;

   // evaluates and differentiates this function over x
   virtual std::pair<Interval,Interval> evalDiff(const Interval& x) = 0;
};

} // namespace

#endif
