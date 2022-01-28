// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PARAM_HPP
#define REALPAVER_PARAM_HPP

#include "realpaver/stopping.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of parameters for the different algorithms.
 *****************************************************************************/
class Param {
public:
   // constructor
   Param();

   // precision of real variables
   Precision realVarPrecision() const;
   Precision realVarPrecision(const Precision& val);
   static Precision DefRealVarPrecision();
   static Precision DefRealVarPrecision(const Precision& val);

   // precision of objective function
   Precision objPrecision() const;
   Precision objPrecision(const Precision& val);
   static Precision DefObjPrecision();
   static Precision DefObjPrecision(const Precision& val);

   // univariate interval Newton method: improvement factor
   Improvement newtonImprovement() const;
   Improvement newtonImprovement(const Improvement& val);
   static Improvement DefNewtonImprovement();
   static Improvement DefNewtonImprovement(const Improvement& val);

   // univariate interval Newton method: maximum number of steps
   int newtonMaxSteps() const;
   int newtonMaxSteps(const int& val);
   static int DefNewtonMaxSteps();
   static int DefNewtonMaxSteps(const int& val);

   // univariate interval Newton method: precision of output
   Precision newtonPrecision() const;
   Precision newtonPrecision(const Precision& val);
   static Precision DefNewtonPrecision();
   static Precision DefNewtonPrecision(const Precision& val);

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
   Improvement propagatorImprovement() const;
   Improvement propagatorImprovement(const Improvement& val);
   static Improvement DefPropagatorImprovement();
   static Improvement DefPropagatorImprovement(const Improvement& val);

   // timeout in milliseconds
   size_t timeout() const;
   size_t timeout(const size_t& val);
   static size_t DefTimeout();
   static size_t DefTimeout(const size_t& val);

private:
   // precision of real variables
   Precision rvar_prec_;
   static Precision def_rvar_prec_;

   // precision of objective function
   Precision obj_prec_;
   static Precision def_obj_prec_;

   // univariate interval Newton method: improvement factor
   Improvement newton_imp_;
   static Improvement def_newton_imp_;

   // univariate interval Newton method: maximum number of steps   
   static int def_newton_ms_;
   int newton_ms_;

   // univariate interval Newton method: precision of output
   Precision newton_prec_;
   static Precision def_newton_prec_;

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
   Improvement propag_imp_;
   static Improvement def_propag_imp_;

   // propagation algorithm: maximum number of steps
   int propag_ms_;
   static int def_propag_ms_;

   // timeout
   size_t timeout_;
   static size_t def_timeout_;
};

} // namespace

#endif
