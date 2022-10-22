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
#include "realpaver/NcspSpaceBFS.hpp"
#include "realpaver/NcspSpaceDFS.hpp"
#include "realpaver/NcspSpaceDMDFS.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Propagator.hpp"
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

NcspSolver::NcspSolver(const Problem& problem)
      : problem_(problem),
        preprob_(),
        preproc_(nullptr),
        env_(nullptr),
        space_(nullptr),
        contractor_(nullptr),
        split_(nullptr),
        stimer_(),
        nbnodes_(0)
{
   THROW_IF(!problem.isCSP(), "Ncsp solver applied to a problem that is " <<
                              "not a constraint satisfaction problem");

   env_ = new NcspEnv();
   preproc_ = new Preprocessor();
}

NcspSolver::~NcspSolver()
{
   if (env_ != nullptr) delete env_;
   if (preproc_ != nullptr) delete preproc_;
   if (space_ != nullptr) delete space_;
   if (split_ != nullptr) delete split_;
}

double NcspSolver::getSolvingTime() const
{
   return stimer_.elapsedTime();   
}

int NcspSolver::getTotalNodes() const
{
   return nbnodes_;
}

void NcspSolver::solve()
{
   LOG_MAIN("Input problem\n" << problem_);

   // first phase: preprocessing
   preproc_->apply(problem_, preprob_);
   if (preproc_->isSolved()) return;

   return branchAndPrune();
}

void NcspSolver::setContractor(SharedContractor contractor)
{
   ASSERT(contractor != nullptr, "Null contractor assigned in a Ncsp solver");
   contractor_ = contractor;
}

void NcspSolver::makeSpace()
{
   // gets the strategy from the parameters
   std::string s = env_->getParam()->getStrParam("BP_NODE_SELECTION");
   if (s == "DFS") space_ = new NcspSpaceDFS();
   if (s == "BFS") space_ = new NcspSpaceBFS();
   if (s == "DMDFS") space_ = new NcspSpaceDMDFS();

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

   std::string base = env_->getParam()->getStrParam("PROPAGATION_BASE");

   // creates a propagator with one default contractor per constraint
   SharedContractorVector pool = std::make_shared<ContractorVector>();
   for (size_t i=0; i<preprob_.nbCtrs(); ++i)
   {
      Constraint c = preprob_.ctrAt(i);
      std::shared_ptr<Contractor> op;

      try
      {
         size_t j = dag_->insert(c);
         std::shared_ptr<Contractor> op;
         
         if (base == "HC4")
            op = std::make_shared<HC4Contractor>(dag_, j);

         else if (base == "BC4")
            op = std::make_shared<BC4Contractor>(dag_, j);

         else
            THROW("bad assignment of parameter PROPAGATION_BASE");

      }
      catch(Exception e)
      {
         op = std::make_shared<ConstraintContractor>(c);
      }

      pool->push(op);
   }

   SharedPropagator propagator = std::make_shared<Propagator>(pool);
   Tolerance dtol = env_->getParam()->getTolParam("PROPAGATION_DTOL");
   propagator->setDistTol(dtol);

   int niter = env_->getParam()->getIntParam("PROPAGATION_ITER_LIMIT");
   propagator->setMaxIter(niter);

   // propagator or propagator + max CID ?
   std::string with_max_cid =
      env_->getParam()->getStrParam("PROPAGATION_WITH_MAX_CID");
   if (with_max_cid == "YES")
   {
      std::unique_ptr<VariableSelector> selector =
         std::make_unique<MaxDomSelector>(preprob_.scope());

      int nb = env_->getParam()->getIntParam("SPLIT_NB_SLICES");
      std::unique_ptr<IntervalSlicer> slicer =
         std::make_unique<IntervalPartitionMaker>(nb);

      SharedContractor op =
         std::make_shared<MaxCIDContractor>(propagator, std::move(selector),
                                            std::move(slicer));

      mainpool->push(op);
   }

   else
      mainpool->push(propagator);

   // polytope hull contractor and non empty dag ?
   std::string with_polytope =
      env_->getParam()->getStrParam("PROPAGATION_WITH_POLYTOPE");
   if (with_polytope == "YES" && !dag_->isEmpty())
   {
      SharedContractor op =
         std::make_shared<PolytopeHullContractor>(dag_, preprob_.scope());

      mainpool->push(op);
   }

   // integer variables
   std::shared_ptr<IntContractor> iop = std::make_shared<IntContractor>();
   for (Variable v : preprob_.scope())
      if (v.isDiscrete()) iop->insertVar(v);

   if (iop->nbVars() > 0)
      mainpool->push(iop);

   // creates the contractor of this solver applying the contractors of
   // the main pool in sequence
   contractor_ = std::make_shared<ListContractor>(mainpool);
}

void NcspSolver::makeSplit()
{
   VariableSelector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   Scope sco = preprob_.scope();

   std::string sel = env_->getParam()->getStrParam("SPLIT_SELECTOR");
   if (sel == "MAX_DOM") selector = new MaxDomSelector(sco);
   if (sel == "ROUND_ROBIN") selector = new RoundRobinSelector(sco);
   // TODO: max smear strategy not handled

   std::string sli = env_->getParam()->getStrParam("SPLIT_SLICER");
   if (sli == "BISECTION") slicer = new IntervalBisecter();
   if (sli == "PEELING")
   {
      double f = env_->getParam()->getDblParam("SPLIT_PEEL_FACTOR");
      slicer = new IntervalPeeler(f);
   }
   if (sli == "PARTITION")
   {
      size_t n = env_->getParam()->getIntParam("SPLIT_NB_SLICES");
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
   SharedNcspNode node = space_->nextPendingNode();
   IntervalRegion* reg = node->region();

   LOG_INTER("Extracts node " << node->index() << " (depth "
                              << node->depth() << ")");
   LOG_LOW("Region: " << *reg);

   // contracts the region
   Proof proof = contractor_->contract(*reg);
   
   LOG_INTER("Contraction -> " << proof);
   LOG_INTER("Contracted region: " << *reg);

   if (proof == Proof::Empty)
   {
      node->setProof(proof);
      return;
   }

   if (isAnInnerRegion(*reg))
   {
      string str = env_->getParam()->getStrParam("SPLIT_INNER");
      if (str == "NO")
      {
         LOG_INTER("Solution node (inner region)");
         node->setProof(Proof::Inner);
         space_->pushSolNode(node);
         return;
      }
      else
      {
         LOG_INTER("Inner region detected but split required");        
      }
   }

   // node depth
   int depth = node->depth() + 1;
   if (depth >= depthlimit)
   {
      LOG_INTER("Node fathomed due to the depth limit");
      env_->setDepthLimit(true);
      return;
   }

   // splits the node
   split_->apply(node);

   if (split_->getNbNodes() <= 1)
   {
      LOG_INTER("Solution node (small enough)");
      node->setProof(Proof::Maybe);
      space_->pushSolNode(node);
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

void NcspSolver::branchAndPrune()
{
   LOG_MAIN("Branch-and-prune algorithm on problem\n" << preprob_);
   LOG_INTER("Parameters\n" << *env_->getParam());

   stimer_.start();

   makeSpace();
   makeContractor();
   makeSplit();

std::cout << "apres les makers" << std::endl;

   // parameters
   double timelimit = env_->getParam()->getDblParam("TIME_LIMIT");
   env_->setTimeLimit(false);

   int nodelimit = env_->getParam()->getIntParam("NODE_LIMIT");
   env_->setNodeLimit(false);

   int sollimit = env_->getParam()->getIntParam("SOLUTION_LIMIT");
   env_->setSolutionLimit(false);

   int depthlimit = env_->getParam()->getIntParam("DEPTH_LIMIT");

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
          preproc_->elapsedTime() + stimer_.elapsedTime() > timelimit)
      {
         LOG_MAIN("Stops on time limit (" << timelimit << "s)");
         env_->setTimeLimit(true);
         iter = false;
      }

      if (iter && nbnodes_ > nodelimit)
      {
         LOG_MAIN("Stops on node limit (" << nodelimit << ")");
         env_->setNodeLimit(true);
         iter = false;
      }

      if (iter && (int)space_->nbSolNodes() >= sollimit)
      {
         LOG_MAIN("Stops on solution limit (" << sollimit << ")");
         env_->setSolutionLimit(true);
         iter = false;         
      }
   }
   while (iter);

   stimer_.stop();
}

NcspEnv* NcspSolver::getEnv() const
{
   return env_;
}

NcspSpace* NcspSolver::getSpace() const
{
   return space_;
}

Preprocessor* NcspSolver::getPreprocessor() const
{
   return preproc_;
}


} // namespace
