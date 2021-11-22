// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include "realpaver_stopping.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of parameters for the different algorithms.
 *****************************************************************************/
class Param {
public:
   // real variables
   static Precision RealVarPrecision();
   static Precision RealVarPrecision(const Precision& val);

   // univariate interval Newton method
   static Improvement NewtonImprovement();
   static Improvement NewtonImprovement(const Improvement& val);

   static int NewtonMaxSteps();
   static int NewtonMaxSteps(const int& val);

   static Precision NewtonPrecision();
   static Precision NewtonPrecision(const Precision& val);

   // inflation
   static double InflatorDelta();
   static double InflatorDelta(const double& val);

   static double InflatorChi();
   static double InflatorChi(const double& val);

   // BC3Revise operator
   static double BC3PeelWidth();
   static double BC3PeelWidth(const double& val);

   static int BC3MaxSteps();
   static int BC3MaxSteps(const int& val);

   // propagation algorithm
   static int PropagatorMaxSteps();
   static int PropagatorMaxSteps(const int& val);

   static Improvement PropagatorImprovement();
   static Improvement PropagatorImprovement(const Improvement& val);

private:
   // real variables
   static Precision rvar_prec_;

   // univariate interval Newton method
   static Improvement newton_imp_;  // improvement factor
   static int newton_ms_;           // maximum number of steps
   static Precision newton_prec_;   // precision

   // inflation
   static double inflator_delta_;
   static double inflator_chi_;

   // BC3Revise operator
   static double  bc3_wpeel_;       // width of peels in consistency checking
   static int bc3_ms_;

   // propagation algorithm
   static Improvement propag_imp_;  // improvement factor
   static int propag_ms_;           // maximum number of steps
};

} // namespace

#endif
