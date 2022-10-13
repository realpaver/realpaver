///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BC4Contractor.hpp"
#include "realpaver/ConstraintContractor.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/IntContractor.hpp"
#include "realpaver/ListContractor.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/MaxCIDContractor.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/NcspSpaceDFS.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Preprocessor.hpp"
#include "realpaver/Propagator.hpp"

namespace realpaver {

NcspSolver::NcspSolver(const Problem& problem)
      : problem_(problem),
        preprob_(),
        param_(nullptr),
        space_(nullptr),
        contractor_(nullptr),
        ptimer_(),
        stimer_()
{
   THROW_IF(!problem.isCSP(), "Ncsp solver applied to a problem that is " <<
                              "not a constraint satisfaction problem");

   param_ = new Param();
}

NcspSolver::~NcspSolver()
{
   if (param_ != nullptr) delete param_;
   if (space_ != nullptr) delete space_;
}

void NcspSolver::setParam(const Param& prm)
{
   if (param_ != nullptr) delete param_;
   param_ = new Param(prm);
}

Param* NcspSolver::getParam() const
{
   return param_;
}

bool NcspSolver::solve()
{
   LOG_MAIN("Input problem\n" << problem_);

   ptimer_.start();

   // first phase: preprocessing
   bool feasible = preprocess();
   if (!feasible) return false;
   
   
   // TODO : solved ?



   return branchAndPrune();
}

bool NcspSolver::preprocess()
{
   Preprocessor proc;

   ptimer_.start();
   bool feasible = proc.apply(problem_, preprob_);
   ptimer_.stop();

   return feasible;
}

void NcspSolver::setContractor(const SharedContractor& contractor)
{
   ASSERT(contractor != nullptr, "Null contractor assigned in a Ncsp solver");
   contractor_ = contractor;
}

void NcspSolver::makeSpace()
{
   // gets the strategy from the parameters
   std::string s = param_->getStrParam("BP_NODE_SELECTION");
   if (s == "DFS") space_ = new NcspSpaceDFS();

   THROW_IF(space_ == nullptr,
            "Unable to make the space object in a Ncsp solver");

   // creates and inserts the root node
   SharedNcspNode node =
      std::make_shared<NcspNode>(preprob_.scope(), preprob_.getDomains());

   space_->insertPendingNode(node);
}
   
void NcspSolver::makeContractor()
{
   // creates an empty dag
   dag_ = std::make_shared<Dag>();

   // main pool of contractors, the pool of this's contractor
   SharedContractorVector mainpool = std::make_shared<ContractorVector>();

   std::string base = param_->getStrParam("PROPAGATION_BASE");

   // creates a propagator with one default contractor per constraint
   SharedContractorVector pool = std::make_shared<ContractorVector>();
   for (size_t i=0; i<preprob_.nbCtrs(); ++i)
   {
      Constraint c = preprob_.ctrAt(i);

      try {
         size_t j = dag_->insert(c);
         std::shared_ptr<Contractor> op;
         
         if (base == "HC4")
         {
            op = std::make_shared<HC4Contractor>(dag_, j);
         }
         else if (base == "BC4")
         {
            op = std::make_shared<BC4Contractor>(dag_, j);
         }
         else
         {
            THROW("bad assignment of parameter PROPAGATION_BASE");
         }
         
         pool->push(op);
      }
      catch(Exception e)
      {
         std::shared_ptr<ConstraintContractor> op =
            std::make_shared<ConstraintContractor>(c);
         pool->push(op);         
      }
   }   
   SharedPropagator propagator = std::make_shared<Propagator>(pool);

   // propagator or propagator + max CID ?
   std::string with_max_cid = param_->getStrParam("PROPAGATION_WITH_MAX_CID");
   if (with_max_cid == "YES")
   {
      std::unique_ptr<VariableSelector> selector =
         std::make_unique<MaxDomSelector>(preprob_.scope());

      int nb = param_->getIntParam("SPLIT_NB_SLICES");
      std::unique_ptr<IntervalSlicer> slicer =
         std::make_unique<IntervalPartitionMaker>(nb);

      SharedContractor op =
         std::make_shared<MaxCIDContractor>(propagator, std::move(selector),
                                            std::move(slicer));

      mainpool->push(op);
   }
   else
   {
      mainpool->push(propagator);
   }

   // polytope hull contractor ?
   std::string with_polytope = param_->getStrParam("PROPAGATION_WITH_POLYTOPE");
   if (with_polytope == "YES")
   {
      SharedContractor op =
         std::make_shared<PolytopeHullContractor>(dag_, preprob_.scope());

      mainpool->push(op);
   }

   // integer variables
   std::shared_ptr<IntContractor> iop = std::make_shared<IntContractor>();
   for (Variable v : preprob_.scope())
   {
      if (v.isDiscrete()) iop->insertVar(v);
   }
   if (iop->nbVars() > 0)
   {
      mainpool->push(iop);
   }

   // creates the contractor of this solver applying the contractors of
   // the main pool in sequence
   contractor_ = std::make_shared<ListContractor>(mainpool);
}

bool NcspSolver::branchAndPrune()
{
   makeSpace();
   makeContractor();

   stimer_.start();
   

   stimer_.stop();

   return space_->nbSolutionNodes() > 0;
}

} // namespace
