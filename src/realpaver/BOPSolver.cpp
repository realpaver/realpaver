///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOPSolver.hpp"
#include "realpaver/Preprocessor.hpp"

namespace realpaver {

BOPSolver::BOPSolver(Problem& problem)
      : problem_(problem),
        preprob_(),
        solprob_(),
        status_(OptimizationStatus::Other),
        sol_(problem.nbVars()),
        objval_(Interval::universe())
{
   THROW_IF(!problem.isBOP(), "BOP Solver applied to a problem" <<
                              "that is not a BOP");
}

bool BOPSolver::optimize()
{
   DEBUG("BOPSolver optimize");
DEBUG("Input problem\n" << problem_);

   Preprocessor preproc;
   bool valid = preproc.apply(problem_, preprob_);

   DEBUG("after preprocessing");

   if (!valid)
   {
DEBUG("infeasible");
      
      status_ = OptimizationStatus::Infeasible;
      return false;
   }

   if (preproc.areAllVarFixed())
   {
DEBUG("all var fixed");

      status_ = OptimizationStatus::Optimal;
      IntervalVector dom(problem_.nbVars());

      for (auto v : problem_.scope())
      {
         Interval x = preproc.getFixedDomain(v);
         dom.set(v.getId(), x);
         sol_.set(v.getId(), x.midpoint());
      }

      Term t = problem_.getObjective().getTerm();
      objval_ = t.eval(dom);

      return true;
   }

DEBUG("Simplified problem\n" << preprob_);


   DEBUG("TODO... return false");
   return false;
   // TODO
}

OptimizationStatus BOPSolver::getStatus() const
{
   return status_;
}

void BOPSolver::setMaxSeconds(double s)
{
   // TODO
}

double BOPSolver::getMaxSeconds() const
{
   // TODO
}

Interval BOPSolver::getObjEnclosure() const
{
   return objval_;
}

RealVector BOPSolver::getBestSolution() const
{
   return sol_;
}

} // namespace
