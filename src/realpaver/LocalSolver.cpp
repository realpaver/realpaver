///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/LocalSolver.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

LocalSolver::LocalSolver(const RealFunction& obj, const RealFunctionVector& ctrs)
      : m_(ctrs.nbFuns()), s_(obj.scope() | ctrs.scope()),
      time_limit_(Param::GetDblParam("LOCAL_SOLVER_TIME_LIMIT"))
{
    obj_ = std::make_shared<RealFunction>(obj);
    ctrs_ = std::make_shared<RealFunctionVector>(ctrs);
    best_ = nullptr;
    n_ = s_.size();
}

LocalSolver::~LocalSolver()
{
}

double LocalSolver::getTimeLimit() const
{
   return time_limit_;
}

void LocalSolver::setTimeLimit(double val)
{
   time_limit_ = val;
}


} // namespace
