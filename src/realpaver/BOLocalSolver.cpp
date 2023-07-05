///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOLocalSolver.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

BOLocalSolver::BOLocalSolver()
      : time_limit_(Param::GetDblParam("BO_LOCAL_SOLVER_TIME_LIMIT"))
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

} // namespace
