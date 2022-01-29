// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include "realpaver/interval.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of parameters for the different algorithms.
 *****************************************************************************/
class Param {
public:
   // constructor
   Param();

   // precision of real variables
   IntervalPrecision realVarPrecision() const;
   IntervalPrecision realVarPrecision(const IntervalPrecision& val);
   static IntervalPrecision DefRealVarPrecision();
   static IntervalPrecision DefRealVarPrecision(const IntervalPrecision& val);

   // precision of objective function
   IntervalPrecision objPrecision() const;
   IntervalPrecision objPrecision(const IntervalPrecision& val);
   static IntervalPrecision DefObjPrecision();
   static IntervalPrecision DefObjPrecision(const IntervalPrecision& val);

   // univariate interval Newton method: improvement factor
   IntervalImprovement newtonImprovement() const;
   IntervalImprovement newtonImprovement(const IntervalImprovement& val);
   static IntervalImprovement DefNewtonImprovement();
   static IntervalImprovement DefNewtonImprovement(const IntervalImprovement& val);

   // univariate interval Newton method: maximum number of steps
   int newtonMaxSteps() const;
   int newtonMaxSteps(const int& val);
   static int DefNewtonMaxSteps();
   static int DefNewtonMaxSteps(const int& val);

   // univariate interval Newton method: precision of output
   IntervalPrecision newtonPrecision() const;
   IntervalPrecision newtonPrecision(const IntervalPrecision& val);
   static IntervalPrecision DefNewtonPrecision();
   static IntervalPrecision DefNewtonPrecision(const IntervalPrecision& val);

   // inflation: delta
   double inflatorDelta() const;
   double inflatorDelta(const double& val);
   static double DefInflatorDelta();
   static double DefInflatorDelta(const double& val);

   // inflation: chi
   double inflatorChi() const;
   double inflatorChi(const double& val);
   static double DefInflatorChi();
   static double DefInflatorChi(const double& val);

   // BC3Revise operator: width of peels in consistency checking
   double bc3PeelWidth() const;
   double bc3PeelWidth(const double& val);
   static double DefBC3PeelWidth();
   static double DefBC3PeelWidth(const double& val);

   // BC3Revise operator: maximum number of steps
   int bc3MaxSteps() const;
   int bc3MaxSteps(const int& val);
   static int DefBC3MaxSteps();
   static int DefBC3MaxSteps(const int& val);

   // propagation algorithm: maximum number of steps
   int propagatorMaxSteps() const;
   int propagatorMaxSteps(const int& val);
   static int DefPropagatorMaxSteps();
   static int DefPropagatorMaxSteps(const int& val);

   // propagation algorithm: maximum number of steps
   IntervalImprovement propagatorImprovement() const;
   IntervalImprovement propagatorImprovement(const IntervalImprovement& val);
   static IntervalImprovement DefPropagatorImprovement();
   static IntervalImprovement DefPropagatorImprovement(const IntervalImprovement& val);

   // timeout in milliseconds
   size_t timeout() const;
   size_t timeout(const size_t& val);
   static size_t DefTimeout();
   static size_t DefTimeout(const size_t& val);

private:
   // precision of real variables
   IntervalPrecision rvar_prec_;
   static IntervalPrecision def_rvar_prec_;

   // precision of objective function
   IntervalPrecision obj_prec_;
   static IntervalPrecision def_obj_prec_;

   // univariate interval Newton method: improvement factor
   IntervalImprovement newton_imp_;
   static IntervalImprovement def_newton_imp_;

   // univariate interval Newton method: maximum number of steps   
   static int def_newton_ms_;
   int newton_ms_;

   // univariate interval Newton method: precision of output
   IntervalPrecision newton_prec_;
   static IntervalPrecision def_newton_prec_;

   // inflation: delta
   double inflator_delta_;
   static double def_inflator_delta_;
   
   // inflation: chi
   double inflator_chi_;
   static double def_inflator_chi_;

   // BC3Revise operator: width of peels in consistency checking
   double bc3_wpeel_;
   static double def_bc3_wpeel_;

   // BC3Revise operator: maximum number of steps
   int bc3_ms_;
   static int def_bc3_ms_;

   // propagation algorithm: improvement factor
   IntervalImprovement propag_imp_;
   static IntervalImprovement def_propag_imp_;

   // propagation algorithm: maximum number of steps
   int propag_ms_;
   static int def_propag_ms_;

   // timeout
   size_t timeout_;
   static size_t def_timeout_;
};

} // namespace

#endif
