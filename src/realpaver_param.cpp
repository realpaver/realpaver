// This file is part of Realpaver. License: see COPYING file.

#include "realpaver_param.hpp"

namespace realpaver {

Precision Param::rvar_prec_ = Precision::adaptive(1.0e-8);

Precision Param::RealVarPrecision()
{
   return rvar_prec_;
}

Precision Param::RealVarPrecision(const Precision& val)
{
   Precision old = rvar_prec_;
   rvar_prec_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

Improvement Param::newton_imp_( 50.0 );

Improvement Param::NewtonImprovement()
{
   return newton_imp_;
}

Improvement Param::NewtonImprovement(const Improvement& val)
{
   Improvement old = newton_imp_;
   newton_imp_ = val;
   return old;
}

int Param::newton_ms_ = 20;

int Param::NewtonMaxSteps()
{
   return newton_ms_;
}

int Param::NewtonMaxSteps(const int& val)
{
   int old = newton_ms_;
   newton_ms_ = val;
   return old;
}

Precision Param::newton_prec_ = 1.0e-8;

Precision Param::NewtonPrecision()
{
   return newton_prec_;
}

Precision Param::NewtonPrecision(const Precision& val)
{
   Precision old = newton_prec_;
   newton_prec_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

double Param::inflator_delta_ = 1.125;

double Param::InflatorDelta()
{
   return inflator_delta_;
}

double Param::InflatorDelta(const double& val)
{
   double old = inflator_delta_;
   inflator_delta_ = val;
   return old;
}

double Param::inflator_chi_ = 1.0e-12;

double Param::InflatorChi()
{
   return inflator_chi_;
}

double Param::InflatorChi(const double& val)
{
   double old = inflator_chi_;
   inflator_chi_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

double Param::bc3_wpeel_ = 2.0;  // 2 percent of the width of an interval

double Param::BC3PeelWidth()
{
   return bc3_wpeel_;
}

double Param::BC3PeelWidth(const double& val)
{
   double old = bc3_wpeel_;
   bc3_wpeel_ = val;
   return old;
}

int Param::bc3_ms_ = 20;

int Param::BC3MaxSteps()
{
   return bc3_ms_;
}

int Param::BC3MaxSteps(const int& val)
{
   int old = bc3_ms_;
   bc3_ms_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

Improvement Param::propag_imp_( 10.0 );

Improvement Param::PropagatorImprovement()
{
   return propag_imp_;
}

Improvement Param::PropagatorImprovement(const Improvement& val)
{
   Improvement old = propag_imp_;
   propag_imp_ = val;
   return old;
}

int Param::propag_ms_ = 50;

int Param::PropagatorMaxSteps()
{
   return propag_ms_;
}

int Param::PropagatorMaxSteps(const int& val)
{
   int old = propag_ms_;
   propag_ms_ = val;
   return old;
}

} // namespace
