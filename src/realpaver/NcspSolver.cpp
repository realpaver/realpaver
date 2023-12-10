///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/BC4Contractor.hpp"
#include "realpaver/ConstraintContractor.hpp"
#include "realpaver/DomainContractor.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/ListContractor.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/MaxCIDContractor.hpp"
#include "realpaver/NcspSolver.hpp"
#include "realpaver/NcspSpaceBFS.hpp"
#include "realpaver/NcspSpaceDFS.hpp"
#include "realpaver/NcspSpaceDMDFS.hpp"
#include "realpaver/NcspSpaceHybridDFS.hpp"
#include "realpaver/PolytopeHullContractor.hpp"
#include "realpaver/Propagator.hpp"
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

NcspSolver::NcspSolver(const Problem& problem)
      : problem_(nullptr),
        preprob_(nullptr),
        preproc_(nullptr),
        env_(nullptr),
        space_(nullptr),
        contractor_(nullptr),
        split_(nullptr),
        prover_(nullptr),
        stimer_(),
        nbnodes_(0),
        withPreprocessing_(true)
{
   THROW_IF(!problem.isCSP(), "Ncsp solver applied to a problem that is " <<
                              "not a constraint satisfaction problem");

   env_ = new NcspEnv();
   preproc_ = new Preprocessor();
   problem_ = new Problem(problem);
}

NcspSolver::~NcspSolver()
{
   if (env_ != nullptr) delete env_;
   if (preproc_ != nullptr) delete preproc_;
   if (space_ != nullptr) delete space_;
   if (split_ != nullptr) delete split_;
   if (prover_ != nullptr) delete prover_;
   if (preprob_ != nullptr) delete preprob_;
   if (problem_ != nullptr) delete problem_;
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
   LOG_MAIN("Input problem\n" << (*problem_));

   std::string pre = env_->getParam()->getStrParam("PREPROCESSING");
   if (pre == "YES")
   {
      // preprocessing + branch-and-prune
      withPreprocessing_ = true;
      preprob_ = new Problem();
      preproc_->apply(*problem_, *preprob_);
   
      if (!preproc_->isSolved()) branchAndPrune();
   }
   else
   {
      // only branch-and-prune
      withPreprocessing_ = false;
      preprob_ = problem_;
      problem_ = nullptr;
      branchAndPrune();
   }
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
   if (s == "DFS")
      space_ = new NcspSpaceDFS();

   else if (s == "BFS")
      space_ = new NcspSpaceBFS();

   else if (s == "DMDFS")
      space_ = new NcspSpaceDMDFS();

   else if (s == "IDFS")
      space_ = new NcspSpaceHybridDFS(HybridDFSStyle::Depth);

   else if (s == "PDFS")
      space_ = new NcspSpaceHybridDFS(HybridDFSStyle::Perimeter);

   else if (s == "GPDFS")
      space_ = new NcspSpaceHybridDFS(HybridDFSStyle::GridPerimeter);

   THROW_IF(space_ == nullptr,
            "Unable to make the space object in a Ncsp solver");

   // creates and inserts the root node
   IntervalBox box(preprob_->scope());

   SharedNcspNode node = std::make_shared<NcspNode>(preprob_->scope(), box);
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
   for (size_t i=0; i<preprob_->nbCtrs(); ++i)
   {
      Constraint c = preprob_->ctrAt(i);
      std::shared_ptr<Contractor> op = nullptr;

      try
      {
         size_t j = dag_->insert(c);

         if (base == "HC4")
            op = std::make_shared<HC4Contractor>(dag_, j);

         else if (base == "BC4")
            op = std::make_shared<BC4Contractor>(dag_, j);

         else
         {
            LOG_INTER("Bad assignment of PROPAGATION_BASE");
            THROW("-");  // exception catched below
         }
      }
      catch(Exception& e)
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
         std::make_unique<MaxDomSelector>(preprob_->scope());

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

   if (with_polytope != "NO" && !dag_->isEmpty())
   {
      PolytopeCreatorStyle style = PolytopeCreatorStyle::RLT;
      if (with_polytope == "TAYLOR")
         style = PolytopeCreatorStyle::Taylor;

      std::shared_ptr<PolytopeHullContractor> op =
         std::make_shared<PolytopeHullContractor>(dag_, style);
      op->setRelaxEqTol(env_->getParam()->getDblParam("RELAXATION_EQ_TOL"));

      mainpool->push(op);
   }

   // interval Newton method for a square system of equations
   std::string with_newton =
      env_->getParam()->getStrParam("PROPAGATION_WITH_NEWTON");

   if (with_newton != "NO")
   {
      std::vector<size_t> eq; // indexes of equations in the DAG

      for (size_t i=0; i<dag_->nbFuns(); ++i)
      {
         if (dag_->fun(i)->getImage().isZero())
            eq.push_back(i);
      }

      if (eq.size() > 1)
      {
         std::shared_ptr<IntervalNewton> newton = nullptr;

         if (eq.size() == dag_->nbFuns())
         {
            IntervalFunctionVector F(dag_);
            if (F.nbVars() == F.nbFuns())
            {
               newton = std::make_shared<IntervalNewton>(F);
            }
         }
         else
         {
            IntervalFunctionVector F;
            for (size_t i : eq)
            {
               IntervalFunction g(dag_, i);
               F.addFun(g);
            }
            if (F.nbVars() == F.nbFuns())
            {
               newton = std::make_shared<IntervalNewton>(F);
            }
         }

         if (newton != nullptr)
         {
            Tolerance tol = env_->getParam()->getTolParam("NEWTON_XTOL");
            newton->setXTol(tol);
            
            tol = env_->getParam()->getTolParam("NEWTON_DTOL");
            newton->setDTol(tol);

            int niter = env_->getParam()->getIntParam("NEWTON_ITER_LIMIT");
            newton->setMaxIter(niter);

            double delta = env_->getParam()->getDblParam("INFLATION_DELTA");
            newton->setInflationDelta(delta);

            double chi = env_->getParam()->getDblParam("INFLATION_CHI");
            newton->setInflationChi(chi);

            tol = env_->getParam()->getTolParam("GAUSS_SEIDEL_XTOL");
            newton->getGaussSeidel()->setXTol(tol);

            tol = env_->getParam()->getTolParam("GAUSS_SEIDEL_DTOL");
            newton->getGaussSeidel()->setDTol(tol);

            niter = env_->getParam()->getIntParam("GAUSS_SEIDEL_ITER_LIMIT");
            newton->getGaussSeidel()->setMaxIter(niter);

            mainpool->push(newton);
         }
      }
   }

   // variables with disconnected domains
   std::shared_ptr<DomainContractor> dop = std::make_shared<DomainContractor>();
   for (Variable v : preprob_->scope())
      if (!v.getDomain()->isConnected())
         dop->insertVar(v);

   if (dop->nbVars() > 0)
      mainpool->push(dop);

   // creates the contractor of this solver, which applies the contractors of
   // the main pool in sequence
   contractor_ = std::make_shared<ListContractor>(mainpool);
}

VariableSelector* NcspSolver::makeMaxSmearStrategy()
{
   IntervalFunctionVector F;

   for (size_t i=0; i<preprob_->nbCtrs(); ++i)
   {
      Constraint c = preprob_->ctrAt(i);

      if (c.isEquation() || c.isInequality())
      {
         Constraint::SharedRep rep = c.rep();
         ArithCtrBinary* eq = static_cast<ArithCtrBinary*>(rep.get());
         Term t = eq->left() - eq->right();

         F.addFun(IntervalFunction(t));
      }
   }

   if (preprob_->nbVars() == F.nbVars())
   {
      return new MaxSmearSelector(F, preprob_->scope());
   }
   else
   {
      LOG_INTER("Unable to create a max-smear variable selection strategy");
      int n = env_->getParam()->getIntParam("SPLIT_DOM_ROBIN");
      return new HybridDomRobinSelector(preprob_->scope(), n);
   }
}

void NcspSolver::makeSplit()
{
   VariableSelector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   Scope sco = preprob_->scope();

   std::string sel = env_->getParam()->getStrParam("SPLIT_SELECTOR");
   if (sel == "MAX_DOM") selector = new MaxDomSelector(sco);
   if (sel == "ROUND_ROBIN") selector = new RoundRobinSelector(sco);
   if (sel == "HYBRID_DOM_ROBIN")
   {
      int n = env_->getParam()->getIntParam("SPLIT_DOM_ROBIN");
      selector = new HybridDomRobinSelector(sco, n);
   }
   if (sel == "MAX_SMEAR")
   {
      selector = makeMaxSmearStrategy();   
   }

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
            "Unable to make the split object in a Ncsp solver");

   std::unique_ptr<VariableSelector> pselector(selector);
   std::unique_ptr<IntervalSlicer> pslicer(slicer);

   split_ = new NcspSplit(std::move(pselector), std::move(pslicer));
}

bool NcspSolver::isAnInnerRegion(const IntervalBox& box) const
{
   for (size_t i=0; i<preprob_->nbCtrs(); ++i)
   {
      Constraint c = preprob_->ctrAt(i);
      if (c.isSatisfied(box) != Proof::Inner)
         return false;
   }

   return true;
}

void NcspSolver::bpStep(int depthlimit)
{
   SharedNcspNode node = space_->nextPendingNode();
   IntervalBox* box = node->region();

   LOG_INTER("Extracts node " << node->index() << " (depth "
                              << node->depth() << ")");
   LOG_LOW("Box: " << *box);

   node->setProof(Proof::Maybe);

   // contracts the box
   Proof proof = contractor_->contract(*box);

   LOG_INTER("Contraction -> " << proof);
   LOG_INTER("Contracted box: " << *box);

   if (proof == Proof::Empty)
   {
      node->setProof(Proof::Empty);
      return;
   }

   if (isAnInnerRegion(*box))
   {
      node->setProof(Proof::Inner);

      std::string str = env_->getParam()->getStrParam("SPLIT_INNER");
      if (str == "NO")
      {
         LOG_INTER("Solution node (inner box)");
         space_->pushSolNode(node);

         return;
      }
      else
      {
         LOG_INTER("Inner box detected but split required");        
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
      }

      space_->insertPendingNodes(split_->begin(), split_->end());
   }
}

void NcspSolver::branchAndPrune()
{
   LOG_MAIN("**************************************************");
   LOG_MAIN("Branch-and-prune algorithm on the following problem\n" << (*preprob_));
   LOG_INTER("Parameters\n" << *env_->getParam());

   stimer_.start();

   makeSpace();
   makeContractor();
   makeSplit();

   // prover that derives proof certificates of the solutions
   prover_ = new Prover(*preprob_);

   int niter = env_->getParam()->getIntParam("NEWTON_CERTIFY_ITER_LIMIT");
   prover_->setMaxIter(niter);

   double delta = env_->getParam()->getDblParam("INFLATION_DELTA");
   prover_->setInflationDelta(delta);

   double chi = env_->getParam()->getDblParam("INFLATION_CHI");
   prover_->setInflationChi(chi);

   Tolerance tol = env_->getParam()->getTolParam("NEWTON_CERTIFY_DTOL");
   prover_->setDTol(tol);

   // parameters
   double timelimit = env_->getParam()->getDblParam("TIME_LIMIT");
   env_->setTimeLimit(false);

   int nodelimit = env_->getParam()->getIntParam("NODE_LIMIT");
   env_->setNodeLimit(false);

   int sollimit = env_->getParam()->getIntParam("SOLUTION_LIMIT");
   env_->setSolutionLimit(false);

   int depthlimit = env_->getParam()->getIntParam("DEPTH_LIMIT");

   bool trace = env_->getParam()->getStrParam("TRACE") == "YES";

   bool iter = true;

   size_t nsol = 0;
   size_t nnodes = 0, tnodes = 0;

   do
   {
      ++nnodes;
      ++tnodes;
      bpStep(depthlimit);
      size_t soln = space_->nbSolNodes();

      if (soln > nsol)
      {
         nsol = soln;
         if (trace)
         {
            std::cout << "\tnb sol: " << "\033[34m" << nsol << "\033[39m"
                      << "\t\ttime: " << "\033[32m" << stimer_.elapsedTime()
                      << "s" << "\033[39m"
                      << "\t\tspace size: " << "\033[31m" << space_->nbPendingNodes()
                      << "\033[39m" << std::endl;
         }
         
         nnodes = 0;
      }
      else
      {

         if (trace && nnodes % 1000 == 0)
         {
            std::cout << "\tnb nod: " << "\033[34m" << tnodes << "\033[39m"
                      << "\t\ttime: " << "\033[32m" << stimer_.elapsedTime()
                      << "s" << "\033[39m"
                      << "\t\tspace size: " << "\033[31m" << space_->nbPendingNodes()
                      << "\033[39m" << std::endl;
         }
      }

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

   double gap = env_->getParam()->getDblParam("SOLUTION_CLUSTER_GAP");
   space_->makeSolClusters(gap);

   certifySolutions();

   stimer_.stop();
}

void NcspSolver::certifySolutions()
{
   std::list<SharedNcspNode> lsol;
   while (space_->nbSolNodes() > 0)
   {
      SharedNcspNode node = space_->popSolNode();
      Proof proof = prover_->certify(*node->region());
      
      if (proof != Proof::Empty)
      {
         node->setProof(proof);
         lsol.push_back(node);
      }
   }

   while (!lsol.empty())
   {
      space_->pushSolNode(lsol.front());
      lsol.pop_front();
   }
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

size_t NcspSolver::getNbSolutions() const
{
   if (preproc_->isSolved())
      return preproc_->isUnfeasible() ? 0 : 1;

   else
      return space_->nbSolNodes();
}

std::pair<IntervalBox, Proof> NcspSolver::getSolution(size_t i) const
{
   ASSERT(i < getNbSolutions(), "Bad access to a solution in a Ncsp solver");

   if (withPreprocessing_)
   {
      IntervalBox box(problem_->scope());
      Proof proof = Proof::Inner;

      // assigns the values of the fixed variables
      for (size_t i=0; i<preproc_->nbFixedVars(); ++i)
      {
         Variable v = preproc_->getFixedVar(i);
         box.set(v, preproc_->getFixedDomain(v));
      }

      // assigns the values of the unfixed variables
      if (!preproc_->allVarsFixed())
      {
         SharedNcspNode node = space_->getSolNode(i);
         proof = node->getProof();

         IntervalBox* aux = node->region();

         for (size_t i=0; i<preproc_->nbUnfixedVars(); ++i)
         {
            Variable v = preproc_->getUnfixedVar(i);
            Variable w = preproc_->srcToDestVar(v);
            box.set(v, aux->get(w));
         }
      }

      return std::make_pair(box, proof);
   }
   else
   {
      SharedNcspNode node = space_->getSolNode(i);
      Proof proof = node->getProof();
      IntervalBox* aux = node->region();
      return std::make_pair(*aux, proof);
   }
}

} // namespace
