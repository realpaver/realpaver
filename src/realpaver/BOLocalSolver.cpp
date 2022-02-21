///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOLocalSolver.hpp"

namespace realpaver {

BOLocalSolver::BOLocalSolver()
      : initObjVal_(Double::neginf()),
        finalObjVal_(Double::neginf())
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

OptimizationStatus BOLocalSolver::minimize(RealFunction& f,
                                           IntervalVector& region,
                                           const RealVector& initialPoint,
                                           RealVector& finalPoint)
{
   finalPoint = initialPoint;
   initObjVal_ = finalObjVal_ = f.realEval(initialPoint);

   return (Double::isInf(initObjVal_) ||
           Double::isNan(initObjVal_)) ? OptimizationStatus::Other :
                                         OptimizationStatus::Optimal;
}

} // namespace
