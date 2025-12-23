/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint Optimizer based on
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
 * @file   Optimizer.cpp
 * @brief  Branch-and-bound algorithm
 * @author Raphaël Chenouard, Laurent Granvilliers
 * @date   6 Oct 2025
 */

#include "DomainBox.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/BoxPicker.hpp"
#include "realpaver/Common.hpp"
#include "realpaver/DomainSlicerFactory.hpp"
#include "realpaver/Logger.hpp"
#include <ios>

#ifdef LOCAL_OPTIMIZER
#include "realpaver/NLPSolver.hpp"
#include "realpaver/ProspectorLocalSolver.hpp"
#endif

#include "realpaver/NodePropagatorList.hpp"
#include "realpaver/Optimizer.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/ProspectorSimple.hpp"
#include "realpaver/ProverOptim.hpp"
#include "realpaver/SpaceOptimFD.hpp"
#include "realpaver/SpaceOptimLU.hpp"
#include "realpaver/SplitLF.hpp"
#include "realpaver/SplitRR.hpp"
#include "realpaver/SplitSF.hpp"
#include "realpaver/SplitSLF.hpp"
#include "realpaver/SplitSSR.hpp"
#include "realpaver/SplitSSRLF.hpp"
#include <iomanip>
#include <memory>

namespace realpaver {

Optimizer::Optimizer(const Problem &problem)
    : problem_(nullptr)
    , pp_(nullptr)
    , context_(nullptr)
    , env_(nullptr)
    , space_(nullptr)
    , dag_(nullptr)
    , propagator_(nullptr)
    , factory_(nullptr)
    , prospector_(nullptr)
    , L_(Double::neginf())
    , U_(Double::inf())
    , z_()
    , obj_()
    , best_(nullptr)
    , nbnodes_(0)
    , withPreprocessing_(true)
    , status_(OptimizationStatus::Other)

{
   if (!problem.hasObjective())
      THROW("No objective function in this problem");

   // TODO: remove this if bound-constrained optimization problems can be handled
   if (problem.nbCtrs() == 0)
      THROW("No constraint in this problem");

   env_ = std::make_shared<EnvOptimizer>();
   problem_ = std::make_shared<Problem>(problem);
   pp_ = std::make_shared<Preprocessor>(problem_);
}

double Optimizer::getSolvingTime() const
{
   return stimer_.elapsedTime();
}

int Optimizer::getTotalNodes() const
{
   return nbnodes_;
}

std::shared_ptr<EnvOptimizer> Optimizer::getEnv() const
{
   return env_;
}

std::shared_ptr<SpaceOptim> Optimizer::getSpace() const
{
   return space_;
}

SharedPreprocessor Optimizer::getPreprocessor() const
{
   return pp_;
}

void Optimizer::optimize()
{
   LOG_MAIN("Input problem\n" << (*problem_));

   int fp = env_->getParams()->getIntParam("FLOAT_PRECISION");
   Logger::getInstance()->setFloatPrecision(fp);

   withPreprocessing_ = env_->getParams()->getStrParam("PREPROCESSING") == "YES";
   pp_->apply(withPreprocessing_);

   if (pp_->isUnfeasible())
      status_ = OptimizationStatus::Infeasible;

   else if (pp_->isSolvedOptimal())
      status_ = OptimizationStatus::Optimal;

   else
      branchAndBound();
}

void Optimizer::makeSpace()
{
   LOG_LOW("Makes the space in the optimizer");
   SharedProblem newprob = pp_->destProblem();

   // space type
   std::string str = env_->getParams()->getStrParam("BB_NODE_SELECTION");
   if (str == "LU")
   {
      std::shared_ptr<SpaceOptimLU> slu = std::make_shared<SpaceOptimLU>();
      double ratio = env_->getParams()->getDblParam("BB_LU_RATIO");
      slu->setRatio(ratio);
      space_ = slu;
   }
   else if (str == "FD")
   {
      space_ = std::make_shared<SpaceOptimFD>();
   }

   THROW_IF(space_ == nullptr, "Unable to create the optimization space");

   // tolerance
   double atol = env_->getParams()->getDblParam("OBJ_ABS_TOL");
   double rtol = env_->getParams()->getDblParam("OBJ_REL_TOL");
   Tolerance tol(rtol, atol);
   space_->setTol(tol);

   // creates and inserts the root node
   SharedNodeOptim node =
       std::make_shared<NodeOptim>(newprob->scope(), newprob->nbCtrs());
   node->setAllActive();
   node->setIndex(0);

   Objective o = newprob->getObjective();
   if (o.isMaximization())
   {
      // the domain of the objective variable after the preprocessing step corresponds to
      // a maximization problem; since the branch and bound solves a minimization problem
      // (max f replaced by min -f) then the opposite of this domain must be considered
      DomainBox *box = node->box();
      Interval dom = box->get(z_)->intervalHull();
      std::unique_ptr<IntervalDomain> mod = std::make_unique<IntervalDomain>(-dom);
      box->set(z_, std::move(mod));
   }

   space_->pushPendingNode(node);
   ++nbnodes_;
}

void Optimizer::makePropagator()
{
   LOG_LOW("Makes the propagator in the optimizer");

   std::unique_ptr<NodePropagatorList> aux = std::make_unique<NodePropagatorList>();

   // Constraint propagation algorithm: HC4, BC4, or AFFINE
   std::string base = env_->getParams()->getStrParam("PROPAGATION_BASE");

   if (base == "HC4")
      aux->pushBack(NodePropagAlgo::HC4, *factory_);
   else if (base == "BC4")
      aux->pushBack(NodePropagAlgo::BC4, *factory_);
   else if (base == "AFFINE")
      aux->pushBack(NodePropagAlgo::AFFINE, *factory_);
   else
      THROW("Bad parameter value for the propagation algorithm");

   // ACID contractor: YES or NO
   std::string with_acid = env_->getParams()->getStrParam("PROPAGATION_WITH_ACID");

   if (with_acid == "YES")
      aux->pushBack(NodePropagAlgo::ACID, *factory_);

   // Polytope hull contractor: YES or NO
   std::string with_polytope =
       env_->getParams()->getStrParam("PROPAGATION_WITH_POLYTOPE_HULL");

   if (with_polytope == "YES")
      aux->pushBack(NodePropagAlgo::Polytope, *factory_);

   if (aux->size() == 0)
      THROW("Unable to create the propagator in the optimizer");

   propagator_ = std::move(aux);
}

void Optimizer::makeSplit()
{
   LOG_LOW("Makes the split object in the optimizer");

   SharedProblem newprob = pp_->destProblem();
   Scope scop = newprob->scope();

   // makes the slicer
   std::string sli = env_->getParams()->getStrParam("SPLIT_SLICING");
   std::unique_ptr<DomainSlicerMap> smap = nullptr;

   if (sli == "BI")
   {
      double sip = env_->getParams()->getDblParam("SPLIT_INTERVAL_POINT");
      smap = DomainSlicerFactory::makeBiStrategy(sip);
   }

   THROW_IF(smap == nullptr, "Unable to make the split object in the optimizer");

   // makes the spliting object acording the variable selection strategy
   std::string sel = env_->getParams()->getStrParam("SPLIT_SELECTION");

   if (sel == "RR")
      split_ = std::make_unique<SplitRR>(scop, std::move(smap));

   else if (sel == "LF")
      split_ = std::make_unique<SplitLF>(scop, std::move(smap));

   else if (sel == "SF")
      split_ = std::make_unique<SplitSF>(scop, std::move(smap));

   else if (sel == "SLF")
      split_ = std::make_unique<SplitSLF>(scop, std::move(smap));

   else if (sel == "SSR")
   {
      if (dag_->isEmpty())
      {
         LOG_LOW("WARNING: Empty DAG, switching to Largest-First splitting strategy!");
         split_ = std::make_unique<SplitLF>(scop, std::move(smap));
      }
      else
      {
         IntervalFunctionVector F = factory_->makeIntervalFunctionVector();
         split_ = std::make_unique<SplitSSR>(scop, std::move(smap), F);
      }
   }

   else if (sel == "SSR_LF")
   {
      if (dag_->isEmpty())
      {
         LOG_LOW("WARNING: Empty DAG, switching to Largest-First splitting strategy!");
         split_ = std::make_unique<SplitLF>(scop, std::move(smap));
      }
      else
      {
         IntervalFunctionVector F = factory_->makeIntervalFunctionVector();
         std::unique_ptr<SplitSSRLF> hybrid =
             std::make_unique<SplitSSRLF>(scop, std::move(smap), F);
         double f = env_->getParams()->getDblParam("SPLIT_SSR_LF_FREQUENCY");
         hybrid->setFrequency(f);
         split_ = std::move(hybrid);
      }
   }

   THROW_IF(split_ == nullptr, "Unable to make the split object in the optimizer");

   // is the objective variable splitable?
   std::string splitobj = env_->getParams()->getStrParam("SPLIT_OBJECTIVE");
   if (splitobj == "NO" && newprob->hasObjective())
   {
      Variable z = newprob->getObjective().getVar();
      z.setSplitable(false);
   }
}

void Optimizer::makeProspector()
{
   LOG_LOW("Makes the explorer object in the optimizer");

   SharedProblem newprob = pp_->destProblem();
   Scope scop = newprob->scope();

   double tol = env_->getParams()->getDblParam("RELAXATION_EQ_TOL");
   std::unique_ptr<ProverOptim> prover = std::make_unique<ProverOptim>(*newprob, tol);

   std::string prospect = env_->getParams()->getStrParam("PROSPECTOR");
   if (prospect == "SIMPLE")
   {
      std::unique_ptr<BoxPicker> picker = std::make_unique<BoxPicker>();
      prospector_ =
          std::make_unique<ProspectorSimple>(std::move(picker), std::move(prover));
   }
#ifdef LOCAL_OPTIMIZER
   else if (prospect == "LOCALSOLVER")
   {
      std::unique_ptr<NLPModel> localSolver = std::make_unique<NLPSolver>(*newprob);
      localSolver->setAlgorithm(env_->getParams()->getStrParam("NLP_SOLVER_ALGORITHM"));
      std::unique_ptr<BoxPicker> picker = std::make_unique<BoxPicker>();
      prospector_ = std::make_unique<ProspectorLocalSolver>(
          std::move(picker), std::move(prover), std::move(localSolver));
   }
#endif

   THROW_IF(prospector_ == nullptr,
            "Unable to make the explorer object in the optimizer");
}

void Optimizer::bbStep(int depthlimit)
{
#if REALPAVER_LOG_ON
   static Timer timerStep;
   timerStep.start();
#endif

   // extracts a node from the space
   SharedNodeOptim node = space_->popPendingNode();

   // processes it
   bbStepAux(node, depthlimit);

   // removes the node informations
   context_->remove(node->index());

#if REALPAVER_LOG_ON
   timerStep.stop();
   LOG_INTER("Total time steps: " << timerStep.elapsedTime() << "(s)");
#endif
}

void Optimizer::bbStepAux(SharedNodeOptim node, int depthlimit)
{
   LOG_NL_INTER();
   LOG_INTER("Extracts node " << node->index() << " (depth " << node->depth() << ")");
   LOG_LOW("Node: " << (*node->box()));

   // splits the node
   split_->apply(node, *context_);

   if (split_->getNbNodes() <= 1)
   {
      LOG_INTER("Node " << node->index() << " is not splitable");
      LOG_LOW(*node->box());
      space_->pushSolNode(node);
   }
   else
   {
      LOG_INTER("Node " << node->index() << " is split into " << split_->getNbNodes()
                        << " sub-nodes");

      nbnodes_ += std::distance(split_->begin(), split_->end());

      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         // gets a node and contracts the box
         SharedNodeOptim subnode = std::static_pointer_cast<NodeOptim>(*it);

         LOG_INTER("Processing of subnode " << subnode->index());
         LOG_INTER("Box: " << (*subnode->box()));

         // Updates the upper bound to U_
         Domain *zdom = subnode->box()->get(z_);
         zdom->contract(Interval::lessThan(U_ - space_->getTol().getAbsTol()));
         if (zdom->isEmpty())
            continue;

         Proof proof = propagator_->contract(*subnode, *context_);

         LOG_INTER("   contraction -> " << proof);

         if (proof == Proof::Empty)
            continue;

         LOG_INTER("Contracted box: " << (*subnode->box()));

         // updates the active set
         proof = checkActivity(*subnode);

         // gets the lower bound
         double lo = subnode->box()->get(z_)->intervalHull().left();
         subnode->setLower(lo);

         // finds a feasible point
         bool found = prospector_->explore(*subnode, *context_);

         if (found)
         {
            // evaluates the objective function at the feasible point
            Interval e = obj_.eval(prospector_->feasiblePoint());
            double up = e.right();
            subnode->setUpper(up);

            LOG_INTER("Feasible point found: " << prospector_->feasiblePoint());

            // new upper bound ?
            if (up < U_)
            {
               best_ = std::make_unique<RealPoint>(prospector_->feasiblePoint());
               space_->simplify(up);
               U_ = up;
               subnode->box()->get(z_)->contract(Interval(lo, up));
               best_->set(z_, up);

               LOG_INTER("New upper bound: " << U_);
            }
         }

         if (space_->getTol().isTightOptimum(lo, U_))
         {
            // tolerance reached => solution node
            LOG_INTER("Tolerance reached => solution node");
            space_->pushSolNode(subnode);
            L_ = space_->getLowestLowerBound();
         }
         else
         {
            LOG_INTER("   => new pending node");

            // new pending node
            LOG_INTER("Tolerance no reached => new pending node");
            space_->pushPendingNode(subnode);
            L_ = space_->getLowestLowerBound();
         }
      }
   }
}

void Optimizer::traceOptimum()
{
   Interval optimum = getOptimum();
   Tolerance tol = getSpace()->getTol();
   double w = optimum.width();
   bool a = (w <= tol.getAbsTol()),
        r = (w <= tol.getRelTol() * Double::abs(optimum.right()));

   LOG_MAIN("Optimum: " << optimum);

   int flp = env_->getParams()->getIntParam("FLOAT_PRECISION");

   std::cout << std::endl
             << std::setprecision(flp) << "\033[39m"
             << "Optimum enclosure:   " << "\033[32m" << std::defaultfloat << optimum
             << "\033[39m";

   if (a)
   {
      std::cout << "\033[39m" << std::scientific << std::setprecision(3) << " (abs "
                << "\033[35m" << tol.getAbsTol() << "\033[39m"
                << ")" << "\033[32m";
   }
   if (r)
   {
      std::cout << "\033[39m" << std::scientific << std::setprecision(3) << " (rel "
                << "\033[35m" << tol.getRelTol() << "\033[39m"
                << ")";
   }
   std::cout << std::endl;

   std::cout << "\033[39m"
             << "Best feasible point: " << "\033[32m" << std::setprecision(flp)
             << std::defaultfloat << bestFeasible() << "\033[39m" << std::endl
             << std::endl;
}

void Optimizer::branchAndBound()
{
   SharedProblem newprob = pp_->destProblem();
   Objective o = newprob->getObjective();
   z_ = o.getVar();
   int sign_obj = 1; // used to trace the bounds
   if (o.isMinimization())
   {
      obj_ = o.getTerm();
   }
   else
   {
      obj_ = -o.getTerm();
      sign_obj = -1;
   }

   // assigns the tolerance of each variable to (0.0, 0.0)
   for (auto v : newprob->scope())
      v.setTolerance(Tolerance(0.0, 0.0));

   LOG_MAIN("Branch-and-bound algorithm on the following problem\n" << (*newprob));
   LOG_INTER("Parameters\n" << *env_->getParams());

   stimer_.start();
   context_ = std::make_unique<NodeContext>();

   LOG_NL_LOW();
   LOG_LOW("Makes the factory in the optimizer");
   factory_ = std::make_unique<ContractorFactory>(*newprob, env_);

   dag_ = factory_->getDag();
   makeSpace();
   makePropagator();
   makeSplit();
   makeProspector();

   // parameters
   bool trace = env_->getParams()->getStrParam("TRACE") == "YES";
   int tracefreq = env_->getParams()->getIntParam("TRACE_FREQUENCY");
   bool limits = (env_->getParams()->getStrParam("LIMITS_ENABLED") == "YES");
   bool incomplete = (env_->getParams()->getStrParam("BB_INCOMPLETE_SEARCH") == "YES");

   double time_limit;
   int node_limit, depth_limit;

   if (limits)
   {
      time_limit = env_->getParams()->getDblParam("TIME_LIMIT");
      node_limit = env_->getParams()->getIntParam("NODE_LIMIT");
      depth_limit = env_->getParams()->getIntParam("DEPTH_LIMIT");
   }
   else
   {
      time_limit = Double::inf();
      node_limit = std::numeric_limits<int>::max();
      depth_limit = std::numeric_limits<int>::max();
   }

   int flp = env_->getParams()->getIntParam("FLOAT_PRECISION");

   env_->setTimeLimit(false);
   env_->setNodeLimit(false);
   env_->setSolutionLimit(false);
   env_->setDepthLimit(false);

   bool iter = true;

#if REALPAVER_LOG_ON
   static Timer timerBB;
   timerBB.start();
#endif

   int tracediv = 0, traceaux = 0;

   do
   {
      double saveL_ = L_, saveU_ = U_;
      bbStep(depth_limit);

#if LOG_ON
      if (L_ != saveL_)
         LOG_INTER("New lower bound: " << L_);
#endif

      // trace every N nodes where N is the frequency
      if (trace && (traceaux = nbnodes_ / tracefreq) > tracediv)
      {
         std::cout << "Time: " << "\033[33m" << std::setw(8) << std::setprecision(3)
                   << std::fixed << stimer_.elapsedTime() << "s" << "\033[39m"
                   << "\t\tNodes: " << "\033[32m" << traceaux * tracefreq << "\033[39m"
                   << "\t\tSpace size: " << "\033[32m" << space_->nbPendingNodes()
                   << "\033[39m" << std::endl;
         tracediv = traceaux;
      }

      // trace the bounds
      if (trace && saveL_ != L_)
      {
         std::cout << "Time: " << "\033[33m" << std::setw(8) << std::setprecision(3)
                   << std::fixed << stimer_.elapsedTime() << "s";
         std::cout << "\033[39m" << std::setprecision(flp) << std::defaultfloat
                   << "\t\tL: " << "\033[35m" << L_ * sign_obj << "\033[39m" << std::endl;
      }
      if (trace && saveU_ != U_)
      {
         std::cout << "Time: " << "\033[33m" << std::setw(8) << std::setprecision(3)
                   << std::fixed << stimer_.elapsedTime() << "s";
         std::cout << "\033[39m" << std::setprecision(flp) << std::defaultfloat
                   << "\t\tU: " << "\033[36m" << U_ * sign_obj << "\033[39m" << std::endl;
      }

      if (iter && pp_->elapsedTime() + stimer_.elapsedTime() > time_limit)
      {
         LOG_MAIN("Stops on time limit (" << time_limit << "s)");
         env_->setTimeLimit(true);
         status_ = OptimizationStatus::StopOnTimeLimit;
         iter = false;
      }

      if (iter && nbnodes_ >= node_limit)
      {
         LOG_MAIN("Stops on node limit (" << node_limit << ")");
         env_->setNodeLimit(true);
         status_ = OptimizationStatus::StopOnNodeLimit;
         iter = false;
      }

      if (iter && incomplete && optimumFound())
      {
         traceOptimum();
         status_ = OptimizationStatus::Optimal;
         iter = false;
      }

      // tests the stopping criteria
      if (iter && space_->nbPendingNodes() == 0)
      {
         if (best_ != nullptr)
         {
            traceOptimum();
            status_ = OptimizationStatus::Optimal;
         }
         else
         {
            status_ = OptimizationStatus::Other;
         }

         LOG_MAIN("Stops since there is no more pending node");
         iter = false;
      }

#if REALPAVER_LOG_ON
      LOG_INTER("Total time BB : " << timerBB.elapsedTime() << "(s)");
#endif
   } while (iter);

#if REALPAVER_LOG_ON
   timerBB.stop();
   LOG_INTER("Total time BB : " << timerBB.elapsedTime() << "(s)");
#endif

   stimer_.stop();
}

bool Optimizer::optimumFound() const
{
   if (best_ == nullptr)
      return false;
   Tolerance tol = space_->getTol();
   return tol.isTightOptimum(L_, U_);
}

Interval Optimizer::getOptimum() const
{
   Objective o = pp_->destProblem()->getObjective();

   if (pp_->isSolved())
   {
      if (pp_->isUnfeasible())
         return Interval::emptyset();
      else
         return pp_->destRegion().get(o.getVar())->intervalHull();
   }
   else
   {
      Interval x(L_, U_);
      return o.isMinimization() ? x : -x;
   }
}

RealPoint Optimizer::bestFeasible() const
{
   RealPoint pt(problem_->scope());

   // assigns the values of the fixed variables
   for (size_t i = 0; i < pp_->nbFixedVars(); ++i)
   {
      Variable v = pp_->getFixedVar(i);
      Interval x = pp_->getFixedDomain(v);
      pt.set(v, x.left());
   }

   // assigns the values of the unfixed variables
   for (size_t i = 0; i < pp_->nbUnfixedVars(); ++i)
   {
      Variable v = pp_->getUnfixedVar(i);
      Variable w = pp_->srcToDestVar(v);
      pt.set(v, best_->get(w));
   }

   return pt;
}

// this method is called after a propagation step => it is not possible
// to detect an inconsistency
Proof Optimizer::checkActivity(NodeOptim &node)
{
   bool inactive = true;
   IntervalBox B(*node.box());
   SharedProblem newprob = pp_->destProblem();

   for (size_t i = 0; i < newprob->nbCtrs(); ++i)
   {
      Constraint c = newprob->ctrAt(i);
      int ic = c.id();

      if (node.isInactive(ic))
         continue;

      Proof proof = c.isSatisfied(B);
      if (proof == Proof::Inner)
      {
         node.setInactive(ic);
      }
      else
      {
         inactive = false;
      }
   }
   return inactive ? Proof::Inner : Proof::Maybe;
}

OptimizationStatus Optimizer::getStatus() const
{
   return status_;
}

} // namespace realpaver
