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
      : initObjVal_(Double::neginf()),
        finalObjVal_(Double::neginf()),
        timelimit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT"))
{}

BOLocalSolver::~BOLocalSolver()
{}

double BOLocalSolver::getInitObjVal() const
{
   return initObjVal_;
}

double BOLocalSolver::getFinalObjVal() const
{
   return finalObjVal_;
}

void BOLocalSolver::setInitObjVal(double& val)
{
   initObjVal_ = val;
}

void BOLocalSolver::setFinalObjVal(double& val)
{
   finalObjVal_ = val;
}

double BOLocalSolver::getTimeLimit() const
{
   return timelimit_;
}

void BOLocalSolver::setTimeLimit(double val)
{
   timelimit_ = val;
}

OptimizationStatus BOLocalSolver::minimize(RealFunction& f,
                                           const IntervalRegion& reg,
                                           const RealPoint& src,
                                           RealPoint& dest)
{
   dest.setAll(src);
   initObjVal_ = finalObjVal_ = f.rfunEval(src);

   return (Double::isInf(initObjVal_) ||
           Double::isNan(initObjVal_)) ? OptimizationStatus::Other :
                                         OptimizationStatus::Optimal;
}

} // namespace
