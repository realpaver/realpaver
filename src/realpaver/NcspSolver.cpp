///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/Logger.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/Preprocessor.hpp"

namespace realpaver {

NcspSolver::NcspSolver(Problem& problem)
      : problem_(problem),
        preprob_(),
        space_(nullptr),
        ptimer_(),
        stimer_()
{
  THROW_IF(!problem.isCSP(), "Ncsp solver applied to a problem that is " <<
                             "not a constraint satisfaction problem");
}

NcspSolver::~NcspSolver()
{
   if (space_ != nullptr) delete space_;
}

bool NcspSolver::solve()
{
   LOG_MAIN("Input problem\n" << problem_);

   ptimer_.start();

   // first phase: preprocessing
   bool feasible = preprocess();
   if (!feasible) return false;



   // TODO
   
   
   
   return true;
}

bool NcspSolver::preprocess()
{
   Preprocessor proc;

   ptimer_.start();
   bool feasible = proc.apply(problem_, preprob_);
   ptimer_.stop();

   return feasible;
}
   
void NcspSolver::branchAndPrune()
{
   // TODO
}

} // namespace
