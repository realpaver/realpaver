///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOPresolver.hpp"
#include "realpaver/BOSolver.hpp"
#include "realpaver/Preprocessor.hpp"

namespace realpaver {

BOSolver::BOSolver(Problem& problem)
      : problem_(problem),
        preprob_(),
        solprob_(),
        model_(nullptr),
        status_(OptimizationStatus::Other),
        sol_(problem.nbVars()),
        objval_(Interval::universe()),
        vmap21_(),
        vmap31_()
{
   THROW_IF(!problem.isBOP(), "BO solver applied to a problem" <<
                              "that is not a BO problem");
}

BOSolver::~BOSolver()
{
   if (model_ != nullptr) delete model_;
}

bool BOSolver::preprocess()
{
   DEBUG("PREPROCESS...");

   Preprocessor preproc;
   bool feasible = preproc.apply(problem_, preprob_);

   if (!feasible)
   {
      status_ = OptimizationStatus::Infeasible;
      return false;
   }

   // assigns the fixed domains in the solution
   for (auto v : problem_.scope())
   {
      if (preproc.hasFixedDomain(v))
         sol_.set(v, preproc.getFixedDomain(v));

      else
      {
         Variable pv = preproc.srcToDestVar(v);
         vmap21_.insert(std::make_pair(pv, v));
   
DEBUG("vmap21 : new " << pv.getName() << "@" <<  pv.getId() << " -> init "
<< v.getName() << "@" << v.getId());
      }
   }

   if (preproc.areAllVarFixed())
   {
      Term t = problem_.getObjective().getTerm();
      objval_ = t.eval(sol_);

      status_ = objval_.isEmpty() ? OptimizationStatus::Infeasible :
                                    OptimizationStatus::Optimal;
   }

   return true;
}

bool BOSolver::presolve()
{
   DEBUG("PRESOLVE...");

   model_ = new BOModel(preprob_);
   BOPresolver presolver(*model_);

   // propagation phase -> contracted region
   bool sfeasible = presolver.presolve();

   if (!sfeasible)
   {
      status_ = OptimizationStatus::Infeasible;
      return false;
   }

   // assigns the contracted domains in the preprocessed problem
   preprob_.setDomains(presolver.getContractedRegion());

DEBUG("contracted : " << presolver.getContractedRegion());

   // preprocessing after presolving
   Preprocessor preproc;
   bool pfeasible = preproc.apply(preprob_, solprob_);

   if (!pfeasible)
   {
      status_ = OptimizationStatus::Infeasible;
      return false;
   }

DEBUG("after second preprocessing, scope : " << preprob_.scope());


  // assigns the fixed domains in the solution
   for (auto pv : preprob_.scope())
   {
      auto it = vmap21_.find(pv);

      if (it != vmap21_.end())
      {
         Variable v = it->second;

         if (preproc.hasFixedDomain(pv))
            sol_.set(v, preproc.getFixedDomain(pv));

         else
         {
            Variable sv = preproc.srcToDestVar(pv);
            vmap31_.insert(std::make_pair(sv, v));

DEBUG("vmap31 : new " << sv.getName() << "@" <<  sv.getId() << " -> init "
<< v.getName() << "@" << v.getId());
         }
      }
   }

   // initial problem solved if only the objective variable remains in
   // the presolved problem
   if (solprob_.nbVars() == 1)
   {
      Term t = problem_.getObjective().getTerm();
      objval_ = t.eval(sol_);

      status_ = objval_.isEmpty() ? OptimizationStatus::Infeasible :
                                    OptimizationStatus::Optimal;
   }

   return true;
}

void BOSolver::solve()
{
DEBUG("BOSolver solve");

   delete model_;

DEBUG("ici, solprob : " << solprob_);

   model_ = new BOModel(solprob_);

   for (auto it=vmap31_.begin(); it != vmap31_.end(); ++it)
   {
   DEBUG("ITER");
   
      Variable sv = it->first;
      Variable v = it->second;

DEBUG("solved  var : " << sv.getName() << "@" << sv.getId());
DEBUG("initial var : " << v.getName() << "@" << v.getId());


      Interval dsv = solprob_.getDomain(sv);
      Interval dv = problem_.getDomain(v);

      if (dv.strictlyContains(dsv))
         model_->setInteriorVar(sv);

      else
         model_->setBoundaryVar(sv);
   }

   
}

bool BOSolver::optimize()
{
   DEBUG("---------- Input problem\n" << problem_);

   status_ = OptimizationStatus::Other;

   // first phase: preprocessing
   bool pfeasible = preprocess();

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      return pfeasible;
   }

DEBUG("\n---------- Simplified problem\n" << preprob_);

   // second phase: presolving
   bool sfeasible = presolve();

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      return sfeasible;
   }

DEBUG("\n---------- Presolved problem\n" << solprob_);



   // third phase: solving
   solve();


DEBUG("\nEND OF OPTIMIZATION");
DEBUG("sol = " << sol_ << "\n\n");

   return true;
   // TODO
}

OptimizationStatus BOSolver::getStatus() const
{
   return status_;
}

void BOSolver::setMaxSeconds(double s)
{
   // TODO
}

double BOSolver::getMaxSeconds() const
{
   // TODO
}

Interval BOSolver::getObjEnclosure() const
{
   return objval_;
}

RealVector BOSolver::getBestSolution() const
{
   return sol_.midpoint();
}

} // namespace
