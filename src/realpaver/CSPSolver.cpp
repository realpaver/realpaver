/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   CSPSolver.cpp
 * @brief  CSP solver
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include <list>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/CSPSolver.hpp"
#include "realpaver/CSPSpaceBFS.hpp"
#include "realpaver/CSPSpaceDFS.hpp"
#include "realpaver/CSPSpaceDMDFS.hpp"
#include "realpaver/CSPSpaceHybridDFS.hpp"

namespace realpaver {

CSPSolver::CSPSolver(const Problem& problem)
      : problem_(nullptr),
        preprob_(nullptr),
        preproc_(nullptr),
        context_(nullptr),
        env_(nullptr),
        space_(nullptr),
        propagator_(nullptr),
        split_(nullptr),
        factory_(nullptr),
        prover_(nullptr),
        stimer_(),
        nbnodes_(0),
        withPreprocessing_(true)
{
   THROW_IF(!problem.isCSP(), "CSP solver applied to a problem that is " <<
                              "not a constraint satisfaction problem");

   env_ = std::make_shared<CSPEnv>();
   preproc_ = new Preprocessor();
   problem_ = new Problem(problem);
}

CSPSolver::~CSPSolver()
{
   if (context_ != nullptr) delete context_;
   if (preproc_ != nullptr) delete preproc_;
   if (space_ != nullptr) delete space_;
   if (factory_ != nullptr) delete factory_;
   if (split_ != nullptr) delete split_;
   if (propagator_ != nullptr) delete propagator_;
   if (prover_ != nullptr) delete prover_;
   if (preprob_ != nullptr) delete preprob_;
   if (problem_ != nullptr) delete problem_;
}

double CSPSolver::getSolvingTime() const
{
   return stimer_.elapsedTime();
}

int CSPSolver::getTotalNodes() const
{
   return nbnodes_;
}

void CSPSolver::solve()
{
   LOG_MAIN("Input problem\n" << (*problem_));

   int fp = env_->getParam()->getIntParam("FLOAT_PRECISION");
   Logger::getInstance()->setFloatPrecision(fp);

   std::string pre = env_->getParam()->getStrParam("PREPROCESSING");
   if (pre == "YES")
   {
      // preprocessing + branch-and-prune
      withPreprocessing_ = true;
      preprob_ = new Problem();
      preproc_->apply(*problem_, *preprob_);

      if (!preproc_->isSolved())
         branchAndPrune();
   }
   else
   {
      // only branch-and-prune
      LOG_MAIN("No preprocessing");

      withPreprocessing_ = false;
      preprob_ = problem_;
      problem_ = nullptr;
      branchAndPrune();
   }
}

void CSPSolver::makeSpace()
{
   LOG_LOW("Makes the space in the CSP solver");

   // gets the strategy from the parameters
   std::string s = env_->getParam()->getStrParam("BP_NODE_SELECTION");
   if (s == "DFS")
      space_ = new CSPSpaceDFS();

   else if (s == "BFS")
      space_ = new CSPSpaceBFS();

   else if (s == "DMDFS")
      space_ = new CSPSpaceDMDFS();

   else if (s == "IDFS")
      space_ = new CSPSpaceHybridDFS(HybridDFSStyle::Depth);

   else if (s == "PDFS")
      space_ = new CSPSpaceHybridDFS(HybridDFSStyle::Perimeter);

   else if (s == "GPDFS")
      space_ = new CSPSpaceHybridDFS(HybridDFSStyle::GridPerimeter);

   THROW_IF(space_ == nullptr,
            "Unable to make the space object in a CSP solver");

   // creates and inserts the root node
   SharedCSPNode node = std::make_shared<CSPNode>(preprob_->scope());
   node->setIndex(0);

   space_->insertPendingNode(node);
   ++nbnodes_;
}

void CSPSolver::makePropagator()
{
   LOG_LOW("Makes the propagator in the CSP solver");

   // Propagation: HC4 or BC4 or ACID
   std::string propag = env_->getParam()->getStrParam("PROPAGATION_BASE");
   bool hc4 = (propag == "HC4"),
        bc4 = (propag == "BC4"),
        acid = (propag == "ACID");

   THROW_IF(!(hc4 || bc4 || acid),
             "Bad parameter value for the propagation algorithm");

   // Newton: YES or NO
   std::string with_newton =
      env_->getParam()->getStrParam("PROPAGATION_WITH_NEWTON");
   bool newton = (with_newton == "YES");

   if (newton == false)
   {
      if (hc4)
         propagator_ = new CSPPropagatorHC4(*factory_);

      else if (bc4)
         propagator_ = new CSPPropagatorBC4(*factory_);

      else
         propagator_ = new CSPPropagatorACID(*factory_);
   }
   else
   {
      if (hc4)
         propagator_ = new CSPPropagatorHC4Newton(*factory_);

      else if (bc4)
         propagator_ = new CSPPropagatorBC4Newton(*factory_);

      else
         propagator_ = new CSPPropagatorACIDNewton(*factory_);
   }
}

void CSPSolver::makeSplit()
{
   LOG_LOW("Makes the split object in the CSP solver");

   Scope scop = preprob_->scope();

   // makes the slicer
   std::string sli = env_->getParam()->getStrParam("SPLIT_SLICING");
   std::unique_ptr<DomainSlicerMap> smap = nullptr;

   if (sli == "BI")
   {
      double sip = env_->getParam()->getDblParam("SPLIT_INTERVAL_POINT");
      smap = DomainSlicerFactory::makeBiStrategy(sip);
   }

   THROW_IF(smap == nullptr,
            "Unable to make the split object in a CSP solver");

   // makes the spliting object acording the variable selection strategy
   std::string sel = env_->getParam()->getStrParam("SPLIT_SELECTION");

   if (sel == "RR")
      split_ = new CSPSplitRR(scop, std::move(smap));

   else if (sel == "LF")
      split_ = new CSPSplitLF(scop, std::move(smap));

   else if (sel == "SF")
      split_ = new CSPSplitSF(scop, std::move(smap));

   else if (sel == "SLF")
      split_ = new CSPSplitSLF(scop, std::move(smap));

   else if (sel == "SSR")
   {
      std::shared_ptr<IntervalSmearSumRel> ssr = factory_->makeSSR();

      if ((ssr != nullptr) && (preprob_->nbVars() == ssr->nbVars()))
      {
         split_ = new CSPSplitSSR(ssr, std::move(smap));
      }
      else
      {
         split_ = new CSPSplitRR(scop, std::move(smap));
         LOG_INTER("Unable to create a SmearSumRel variable selection " <<
                   "strategy -> use a round-robin strategy instead");
      }
   }

   THROW_IF(split_ == nullptr,
            "Unable to make the split object in a CSP solver");
}

bool CSPSolver::isInner(DomainBox* box) const
{
   IntervalBox B(*box);
   for (size_t i=0; i<preprob_->nbCtrs(); ++i)
   {
      Constraint c = preprob_->ctrAt(i);
      if (c.isSatisfied(B) != Proof::Inner)
         return false;
   }

   return true;
}

void CSPSolver::bpStep(int depthlimit)
{
#if LOG_ON
   static Timer timerStep;
   timerStep.start();
#endif

   // extracts a node from the space
    SharedCSPNode node = space_->nextPendingNode();

   // processes it
    bpStepAux(node, depthlimit);

   // removes the node informations
   context_->remove(node->index());

#if LOG_ON
   timerStep.stop();
   LOG_INTER("Total time steps : " << timerStep.elapsedTime() << "(s)");
#endif
}

void CSPSolver::bpStepAux(SharedCSPNode node, int depthlimit)
{
#if LOG_ON
   static Timer timerPropag, timerSplit;
#endif

   LOG_NL_INTER();
   LOG_INTER("Extracts node " << node->index() << " (depth "
                              << node->depth() << ")");
   LOG_LOW("Node: " << (*node->box()));

   node->setProof(Proof::Maybe);

#if LOG_ON
   timerPropag.start();
#endif

   // contracts the box
   Proof proof = propagator_->contract(*node, *context_);

#if LOG_ON
   timerPropag.stop();
   LOG_INTER("Total time contraction : " << timerPropag.elapsedTime() << "(s)");
#endif

   LOG_INTER("Contraction -> " << proof);

   if (proof == Proof::Empty)
   {
      node->setProof(Proof::Empty);
      return;
   }
   else
   {
      LOG_INTER("Contracted box: " << (*node->box()));
   }

   if (isInner(node->box()))
   {
      LOG_INTER("Node " << node->index() << " contains an inner box");

      node->setProof(Proof::Inner);

      std::string str = env_->getParam()->getStrParam("SPLIT_INNER_BOX");
      if (str == "NO")
      {
         space_->pushSolNode(node);
         return;
      }
   }

   // node depth limit
   int depth = node->depth() + 1;
   if (depth >= depthlimit)
   {
      LOG_INTER("Node fathomed due to the depth limit");
      env_->setDepthLimit(true);
      return;
   }

#if LOG_ON
   timerSplit.start();
#endif

   // splits the node
   split_->apply(node, *context_);

   if (split_->getNbNodes() <= 1)
   {
      LOG_INTER("Node " << node->index() << " is a solution");
      LOG_LOW(*node->box());

      space_->pushSolNode(node);
   }
   else
   {
      LOG_INTER("Node " << node->index() << " is split into "
                        << split_->getNbNodes() << " sub-nodes");

#if LOG_ON
      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         SharedCSPNode subnode = *it;
         LOG_INTER("Inserts node " << subnode->index() << " in the space");
         LOG_LOW(*subnode->box());
      }
#endif

      nbnodes_ += std::distance(split_->begin(), split_->end());
      space_->insertPendingNodes(split_->begin(), split_->end());
   }

#if LOG_ON
   timerSplit.stop();
   LOG_INTER("Total time split : " << timerSplit.elapsedTime() << "(s)");
#endif
}

void CSPSolver::branchAndPrune()
{
   LOG_MAIN("Branch-and-prune algorithm on the following problem\n" << (*preprob_));
   LOG_INTER("Parameters\n" << *env_->getParam());

   stimer_.start();

   context_ = new CSPContext();

   LOG_NL_LOW();
   LOG_LOW("Makes the factory in the CSP solver");
   factory_ = new ContractorFactory(*preprob_, env_);

   makeSpace();
   makePropagator();
   makeSplit();

   // prover that derives proof certificates of the solutions
   prover_ = new Prover(*preprob_);

   int niter = env_->getParam()->getIntParam("NEWTON_CERTIFY_ITER_LIMIT");
   prover_->setMaxIter(niter);

   double delta = env_->getParam()->getDblParam("INFLATION_DELTA");
   prover_->setInflationDelta(delta);

   double chi = env_->getParam()->getDblParam("INFLATION_CHI");
   prover_->setInflationChi(chi);

   // parameters
   double timelimit = env_->getParam()->getDblParam("TIME_LIMIT");
   env_->setTimeLimit(false);

   int nodelimit = env_->getParam()->getIntParam("NODE_LIMIT");
   env_->setNodeLimit(false);

   int sollimit = env_->getParam()->getIntParam("SOLUTION_LIMIT");
   env_->setSolutionLimit(false);

   int depthlimit = env_->getParam()->getIntParam("DEPTH_LIMIT");

   bool trace = env_->getParam()->getStrParam("TRACE") == "YES";
   int tracefreq = env_->getParam()->getIntParam("TRACE_FREQUENCY");

   bool iter = true;

   size_t last_nb_sol = 0;
   size_t nb_nodes_processed = 0;

#if LOG_ON
   static Timer timerBP;
   timerBP.start();
#endif

   do
   {
      ++nb_nodes_processed;
      bpStep(depthlimit);

      // trace every new solution
      size_t nb_sol = space_->nbSolNodes();
      if (nb_sol > last_nb_sol)
      {
         last_nb_sol = nb_sol;
         if (trace)
         {
            std::cout << "\tnb sol: " << "\033[34m" << nb_sol << "\033[39m"
                      << "\t\ttime: " << "\033[32m" << stimer_.elapsedTime()
                      << "s" << "\033[39m"
                      << "\t\tspace size: " << "\033[31m"
                      << space_->nbPendingNodes()
                      << "\033[39m" << std::endl;
         }
      }

      // trace every N nodes where N is the frequency
      if (trace && nb_nodes_processed % tracefreq == 0)
      {
         std::cout << "\tnb nod: " << "\033[34m" << nb_nodes_processed
                   << "\033[39m" << "\t\ttime: " << "\033[32m"
                   << stimer_.elapsedTime() << "s" << "\033[39m"
                   << "\t\tspace size: " << "\033[31m"
                   << space_->nbPendingNodes() << "\033[39m" << std::endl;
      }

      // tests the stopping criteria
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

#if LOG_ON
   LOG_INTER("Total time BP : " << timerBP.elapsedTime() << "(s)");
#endif
   }
   while (iter);

   double gap = env_->getParam()->getDblParam("SOLUTION_CLUSTER_GAP");
   space_->makeSolClusters(gap);

#if LOG_ON
   timerBP.stop();
   LOG_INTER("Total time BP : " << timerBP.elapsedTime() << "(s)");
#endif

   LOG_NL_MAIN();

   std::string sc = env_->getParam()->getStrParam("CERTIFICATION");
   if (sc == "YES")
   {
      LOG_INTER("Certification a posteriori");
      certifySolutions();
   }
   else
   {
      LOG_INTER("No Certification a posteriori");
   }

   stimer_.stop();
}

void CSPSolver::certifySolutions()
{
   std::list<SharedCSPNode> lsol;
   while (space_->nbSolNodes() > 0)
   {
      SharedCSPNode node = space_->popSolNode();
      Proof proof = node->getProof();

      DomainBox* dbox = node->box();
      IntervalBox B(*dbox);

      proof = prover_->certify(B);

      if (proof != Proof::Empty)
      {
         // B may be different from the hull of dbox, typically when
         // a Newton operator is applied by the prover; it is then necessary
         // to modify dbox
         for (const auto& v : B.scope())
         {
            Interval x = B.get(v),
                     y = dbox->get(v)->intervalHull();

            if (x.isSetNeq(y))
            {
               std::unique_ptr<IntervalDomain> dom(new IntervalDomain(x));
               dbox->set(v, std::move(dom));
            }
         }

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

std::shared_ptr<CSPEnv> CSPSolver::getEnv() const
{
   return env_;
}

CSPSpace* CSPSolver::getSpace() const
{
   return space_;
}

Preprocessor* CSPSolver::getPreprocessor() const
{
   return preproc_;
}

size_t CSPSolver::nbSolutions() const
{
   if (withPreprocessing_ && preproc_->isSolved())
      return preproc_->isUnfeasible() ? 0 : 1;

   else
      return space_->nbSolNodes();
}

std::pair<DomainBox, Proof> CSPSolver::getSolution(size_t i) const
{
   ASSERT(i < nbSolutions(),
          "Bad access to a solution in a CSP solver @ " << i);

   if (withPreprocessing_)
   {
      DomainBox box(problem_->scope());
      Proof proof = Proof::Inner;

      // assigns the values of the fixed variables
      for (size_t i=0; i<preproc_->nbFixedVars(); ++i)
      {
         Variable v = preproc_->getFixedVar(i);
         Interval x = preproc_->getFixedDomain(v);
         Domain* dom = box.get(v);
         dom->contract(x);
      }

      // assigns the values of the unfixed variables
      if (!preproc_->allVarsFixed())
      {
         SharedCSPNode node = space_->getSolNode(i);
         proof = node->getProof();

         DomainBox* aux = node->box();

         for (size_t i=0; i<preproc_->nbUnfixedVars(); ++i)
         {
            Variable v = preproc_->getUnfixedVar(i);
            Variable w = preproc_->srcToDestVar(v);
            std::unique_ptr<Domain> p(aux->get(w)->clone());
            box.set(v, std::move(p));
         }
      }

      return std::make_pair(box, proof);
   }
   else
   {
      SharedCSPNode node = space_->getSolNode(i);
      Proof proof = node->getProof();
      DomainBox aux(*node->box());
      return std::make_pair(aux, proof);
   }
}

size_t CSPSolver::nbPendingNodes() const
{
   if (preproc_->isSolved())
      return 0;

   else
      return space_->nbPendingNodes();
}

DomainBox CSPSolver::getPendingBox(size_t i) const
{
   ASSERT(i < nbPendingNodes(), "Bad access to a pending box in a CSP solver");

   if (withPreprocessing_)
   {
      DomainBox box(problem_->scope());

      // assigns the values of the fixed variables
      for (size_t i=0; i<preproc_->nbFixedVars(); ++i)
      {
         Variable v = preproc_->getFixedVar(i);
         Interval x = preproc_->getFixedDomain(v);
         Domain* dom = box.get(v);
         dom->contract(x);
      }

      // assigns the values of the unfixed variables
      if (!preproc_->allVarsFixed())
      {
         SharedCSPNode node = space_->getPendingNode(i);
         DomainBox* aux = node->box();

         for (size_t i=0; i<preproc_->nbUnfixedVars(); ++i)
         {
            Variable v = preproc_->getUnfixedVar(i);
            Variable w = preproc_->srcToDestVar(v);
            std::unique_ptr<Domain> p(aux->get(w)->clone());
            box.set(v, std::move(p));
         }
      }

      return box;
   }
   else
   {
      SharedCSPNode node = space_->getPendingNode(i);
      DomainBox aux(*node->box());
      return aux;
   }
}

} // namespace
