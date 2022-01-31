// This file is part of Realpaver. License: see COPYING file.

#include <limits>
#include "realpaver/param.hpp"

namespace realpaver {

Param::Param()
{
   rvar_prec_ = DefRealVarPrecision();
   newton_imp_ = DefNewtonImprovement();
   newton_ms_ = DefNewtonMaxSteps();
   newton_prec_ = DefNewtonPrecision();
   inflator_delta_ = DefInflatorDelta();
   inflator_chi_ = DefInflatorChi();
   bc3_wpeel_ = DefBC3PeelWidth();
   propag_ms_ = DefPropagatorMaxSteps();
   propag_imp_ = DefPropagatorImprovement();
}

///////////////////////////////////////////////////////////////////////////////

Tolerance Param::def_rvar_prec_ = Tolerance::makeRel(1.0e-8);

Tolerance Param::DefRealVarPrecision()
{
   return def_rvar_prec_;
}

Tolerance Param::DefRealVarPrecision(const Tolerance& val)
{
   Tolerance old = def_rvar_prec_;
   def_rvar_prec_ = val;
   return old;
}

Tolerance Param::realVarPrecision() const
{
   return rvar_prec_;
}

Tolerance Param::realVarPrecision(const Tolerance& val)
{
   Tolerance old = rvar_prec_;
   rvar_prec_ = val;
   return old;   
}

///////////////////////////////////////////////////////////////////////////////

Tolerance Param::def_obj_prec_ = Tolerance::makeRel(1.0e-6);

Tolerance Param::DefObjPrecision()
{
   return def_obj_prec_;
}

Tolerance Param::DefObjPrecision(const Tolerance& val)
{
   Tolerance old = def_obj_prec_;
   def_obj_prec_ = val;
   return old;
}

Tolerance Param::objPrecision() const
{
   return obj_prec_;
}

Tolerance Param::objPrecision(const Tolerance& val)
{
   Tolerance old = obj_prec_;
   obj_prec_ = val;
   return old;   
}

///////////////////////////////////////////////////////////////////////////////

IntervalImprovement Param::def_newton_imp_( 50.0 );

IntervalImprovement Param::DefNewtonImprovement()
{
   return def_newton_imp_;
}

IntervalImprovement Param::DefNewtonImprovement(const IntervalImprovement& val)
{
   IntervalImprovement old = def_newton_imp_;
   def_newton_imp_ = val;
   return old;
}

IntervalImprovement Param::newtonImprovement() const
{
   return newton_imp_;
}

IntervalImprovement Param::newtonImprovement(const IntervalImprovement& val)
{
   IntervalImprovement old = newton_imp_;
   newton_imp_ = val;
   return old;
}

int Param::def_newton_ms_ = 20;

int Param::DefNewtonMaxSteps()
{
   return def_newton_ms_;
}

int Param::DefNewtonMaxSteps(const int& val)
{
   int old = def_newton_ms_;
   def_newton_ms_ = val;
   return old;
}

int Param::newtonMaxSteps() const
{
   return newton_ms_;
}

int Param::newtonMaxSteps(const int& val)
{
   int old = newton_ms_;
   newton_ms_ = val;
   return old;
}

Tolerance Param::def_newton_prec_ = 1.0e-8;

Tolerance Param::DefNewtonPrecision()
{
   return def_newton_prec_;
}

Tolerance Param::DefNewtonPrecision(const Tolerance& val)
{
   Tolerance old = def_newton_prec_;
   def_newton_prec_ = val;
   return old;
}

Tolerance Param::newtonPrecision() const
{
   return newton_prec_;
}

Tolerance Param::newtonPrecision(const Tolerance& val)
{
   Tolerance old = newton_prec_;
   newton_prec_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

double Param::def_inflator_delta_ = 1.125;

double Param::DefInflatorDelta()
{
   return def_inflator_delta_;
}

double Param::DefInflatorDelta(const double& val)
{
   double old = def_inflator_delta_;
   def_inflator_delta_ = val;
   return old;
}

double Param::inflatorDelta() const
{
   return inflator_delta_;
}

double Param::inflatorDelta(const double& val)
{
   double old = inflator_delta_;
   inflator_delta_ = val;
   return old;
}

double Param::def_inflator_chi_ = 1.0e-12;

double Param::DefInflatorChi()
{
   return def_inflator_chi_;
}

double Param::DefInflatorChi(const double& val)
{
   double old = def_inflator_chi_;
   def_inflator_chi_ = val;
   return old;
}

double Param::inflatorChi() const
{
   return inflator_chi_;
}

double Param::inflatorChi(const double& val)
{
   double old = inflator_chi_;
   inflator_chi_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

double Param::def_bc3_wpeel_ = 2.0;  // 2 percent of the width of an interval

double Param::DefBC3PeelWidth()
{
   return def_bc3_wpeel_;
}

double Param::DefBC3PeelWidth(const double& val)
{
   double old = def_bc3_wpeel_;
   def_bc3_wpeel_ = val;
   return old;
}

double Param::bc3PeelWidth() const
{
   return bc3_wpeel_;
}

double Param::bc3PeelWidth(const double& val)
{
   double old = bc3_wpeel_;
   bc3_wpeel_ = val;
   return old;
}

int Param::def_bc3_ms_ = 20;

int Param::DefBC3MaxSteps()
{
   return def_bc3_ms_;
}

int Param::DefBC3MaxSteps(const int& val)
{
   int old = def_bc3_ms_;
   def_bc3_ms_ = val;
   return old;
}

int Param::bc3MaxSteps() const
{
   return bc3_ms_;
}

int Param::bc3MaxSteps(const int& val)
{
   int old = bc3_ms_;
   bc3_ms_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

IntervalImprovement Param::def_propag_imp_( 10.0 );

IntervalImprovement Param::DefPropagatorImprovement()
{
   return def_propag_imp_;
}

IntervalImprovement Param::DefPropagatorImprovement(const IntervalImprovement& val)
{
   IntervalImprovement old = def_propag_imp_;
   def_propag_imp_ = val;
   return old;
}

IntervalImprovement Param::propagatorImprovement() const
{
   return propag_imp_;
}

IntervalImprovement Param::propagatorImprovement(const IntervalImprovement& val)
{
   IntervalImprovement old = propag_imp_;
   propag_imp_ = val;
   return old;
}

int Param::def_propag_ms_ = 50;

int Param::DefPropagatorMaxSteps()
{
   return def_propag_ms_;
}

int Param::DefPropagatorMaxSteps(const int& val)
{
   int old = def_propag_ms_;
   def_propag_ms_ = val;
   return old;
}

int Param::propagatorMaxSteps() const
{
   return propag_ms_;
}

int Param::propagatorMaxSteps(const int& val)
{
   int old = propag_ms_;
   propag_ms_ = val;
   return old;
}

///////////////////////////////////////////////////////////////////////////////

size_t Param::def_timeout_ = std::numeric_limits<size_t>::max();

size_t Param::DefTimeout()
{
   return def_timeout_;
}

size_t Param::DefTimeout(const size_t& val)
{
   size_t old = def_timeout_;
   def_timeout_ = val;
   return old;
}

size_t Param::timeout() const
{
   return timeout_;
}

size_t Param::timeout(const size_t& val)
{
   size_t old = timeout_;
   timeout_ = val;
   return old;
}

} // namespace
