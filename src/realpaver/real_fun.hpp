// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_REAL_FUN_HPP
#define REALPAVER_REAL_FUN_HPP

#include "realpaver/real_vector.hpp"
#include "realpaver/scope.hpp"

namespace realpaver {

/*****************************************************************************
 * Pure abstract class of real-valued functions.
 *****************************************************************************/
class RealFun {
public:
   // virtual destructor
   virtual ~RealFun();

   // evaluates this function at x
   virtual double reval(const RealVector& x) = 0;

   // evaluates the gradient g of this function at x
   virtual void rdiff(const RealVector& x, RealVector& g) = 0;

   // evaluates this function at x and the gradient g of this function at x
   virtual void revalDiff(const RealVector& x, double& val, RealVector& g) = 0;
};

} // namespace

#endif
