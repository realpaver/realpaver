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
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

NcspSolver::NcspSolver(const Problem& problem)
      : problem_(problem),
        preprob_(),
        param_(nullptr),
        space_(nullptr),
        contractor_(nullptr),
        split_(nullptr),
        ptimer_(),
        stimer_(),
        nbnodes_(0),
        isComplete_(true),
        tlim_(false),
        nlim_(false),
        slim_(false)
{
   THROW_IF(!problem.isCSP(), "Ncsp solver applied to a problem that is " <<
                              "not a constraint satisfaction problem");

   param_ = new Param();
}

NcspSolver::~NcspSolver()
{
   if (param_ != nullptr) delete param_;
   if (space_ != nullptr) delete space_;
   if (split_ != nullptr) delete split_;
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

double NcspSolver::getPreprocessingTime() const
{
   return ptimer_.elapsedTime();
}

double NcspSolver::getSolvingTime() const
{
   return stimer_.elapsedTime();   
}

int NcspSolver::getTotalNodes() const
{
   return nbnodes_;
}

int NcspSolver::getNbPendingNodes() const
{
   return space_->nbPendingNodes();
}

int NcspSolver::getNbSolutions() const
{
   return space_->nbSolutionNodes();
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

   node->setIndex(1);
   space_->insertPendingNode(node);
   ++nbnodes_;
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

      try
      {
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

void NcspSolver::makeSplit()
{
   VariableSelector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   Scope sco = preprob_.scope();

   std::string sel = param_->getStrParam("SPLIT_SELECTOR");
   if (sel == "MAX_DOM") selector = new MaxDomSelector(sco);
   if (sel == "ROUND_ROBIN") selector = new RoundRobinSelector(sco);
   // TODO: max smear strategy not handled

   std::string sli = param_->getStrParam("SPLIT_SLICER");
   if (sli == "BISECTION") slicer = new IntervalBisecter();
   if (sli == "PEELING")
   {
      double f = param_->getDblParam("SPLIT_PEEL_FACTOR");
      slicer = new IntervalPeeler(f);
   }
   if (sli == "PARTITION")
   {
      size_t n = param_->getIntParam("SPLIT_NB_SLICES");
      slicer = new IntervalPartitionMaker(n);
   }

   THROW_IF(selector == nullptr || slicer == nullptr,
            "Unable to make the split object in a BO solver");

   std::unique_ptr<VariableSelector> pselector(selector);
   std::unique_ptr<IntervalSlicer> pslicer(slicer);

   split_ = new NcspSplit(std::move(pselector), std::move(pslicer));
}

bool NcspSolver::isAnInnerRegion(const IntervalRegion& reg) const
{
   for (size_t i=0; i<preprob_.nbCtrs(); ++i)
   {
      Constraint c = preprob_.ctrAt(i);
      if (c.isSatisfied(reg) != Proof::Inner)
         return false;
   }
   return true;
}

void NcspSolver::bpStep(int depthlimit)
{
   SharedNcspNode node = space_->extractPendingNode();
   IntervalRegion* reg = node->region();

   LOG_INTER("Extracts node " << node->index() << " (depth "
                              << node->depth() << ")");
   LOG_LOW("Region: " << *reg);

   // contracts the region
   Proof proof = contractor_->contract(*reg);
   
   LOG_INTER("Contraction -> " << proof);
   LOG_LOW("Contracted region: " << *reg);

   if (proof == Proof::Empty)
   {
      node->setProof(proof);
      return;
   }

   if (isAnInnerRegion(*reg))
   {
      LOG_INTER("Solution node with an inner region");
      node->setProof(Proof::Inner);
      space_->pushSolutionNode(node);
      return;
   }

   // node depth
   int depth = node->depth() + 1;
   if (depth >= depthlimit)
   {
      LOG_INTER("Node fathomed due to the depth limit");
      isComplete_ = false;
      return;
   }

   // splits the node
   split_->apply(node);

   if (split_->getNbNodes() == 1)
   {
      LOG_INTER("Soliution node with a small enough region");
      node->setProof(Proof::Maybe);
      space_->pushSolutionNode(node);
   }
   else
   {
      LOG_INTER("Splits node " << node->index() << " > "
                               << split_->getNbNodes() << " sub-nodes");

      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         ++nbnodes_;

         SharedNcspNode subnode = *it;
         subnode->setIndex(nbnodes_);
         subnode->setDepth(depth);

         LOG_INTER("Inserts node " << subnode->index() << " in the space");
         space_->insertPendingNode(subnode);
      }
   }
}

bool NcspSolver::branchAndPrune()
{
   LOG_MAIN("Branch-and-prune algorithm");
   stimer_.start();

   makeSpace();
   makeContractor();
   makeSplit();

   // parameters
   double timelimit = param_->getDblParam("TIME_LIMIT");
   int nodelimit = param_->getIntParam("NODE_LIMIT");
   int depthlimit = param_->getIntParam("DEPTH_LIMIT");
   int sollimit = param_->getIntParam("BP_SOLUTION_LIMIT");

   LOG_INTER("Time limit:  " << timelimit << "s");
   LOG_INTER("Node limit:  " << nodelimit);
   LOG_INTER("Depth limit: " << depthlimit);
   LOG_INTER("Solution limit: " << sollimit);

   bool iter = true;
   
   do
   {
      bpStep(depthlimit);

      if (space_->nbPendingNodes() == 0)
      {
         LOG_MAIN("Stops since there is no more pending node");
         iter = false;
      }

      if (iter &&
          ptimer_.elapsedTime() + stimer_.elapsedTime() > timelimit)
      {
         LOG_MAIN("Stops on time limit (" << timelimit << "s)");
         isComplete_ = false;
         tlim_ = true;
         iter = false;
      }

      if (iter && nbnodes_ > nodelimit)
      {
         LOG_MAIN("Stops on node limit (" << nodelimit << ")");
         isComplete_ = false;
         nlim_ = true;
         iter = false;
      }

      if (iter && space_->nbSolutionNodes() >= sollimit)
      {
         LOG_MAIN("Stops on solution limit (" << sollimit << ")");
         isComplete_ = false;
         slim_ = true;
         iter = false;         
      }
   }
   while (iter);

   isComplete_ = isComplete_ && (space_->nbPendingNodes() == 0);

   stimer_.stop();
   return space_->nbSolutionNodes() > 0;
}

IntervalRegion NcspSolver::getSolution(size_t i) const
{
   ASSERT(i < space_->nbSolutionNodes(), "Bad access to a solution @ " << i);

   SharedNcspNode node = space_->getSolutionNode(i);
   IntervalRegion* reg = node->region();
   return *reg;
}

bool NcspSolver::proofComplete() const
{
   return isComplete_;
}

bool NcspSolver::proofFeasible() const
{
   return space_->proofFeasible();
}

bool NcspSolver::overTimeLimit() const
{
   return tlim_;
}

bool NcspSolver::overNodeLimit() const
{
   return nlim_;
}

bool NcspSolver::overSolutionLimit() const
{
   return slim_;
}

} // namespace
