///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOLocalSolver.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

BOLocalSolver::BOLocalSolver()
      : time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT"))
{}

BOLocalSolver::~BOLocalSolver()
{}

double BOLocalSolver::getTimeLimit() const
{
   return time_limit_;
}

void BOLocalSolver::setTimeLimit(double val)
{
   time_limit_ = val;
}

OptimizationStatus BOLocalSolver::minimize(RealFunction& f,
                                           const IntervalRegion& reg,
                                           const RealPoint& src,
                                           RealPoint& dest)
{
   dest.setAll(src);

   double u = f.rfunEval(src);

   return (Double::isInf(u) || Double::isNan(u)) ? OptimizationStatus::Other :
                                                   OptimizationStatus::Optimal;
}

} // namespace
