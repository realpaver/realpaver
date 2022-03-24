///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOContractor.hpp"
#include "realpaver/BOLocalGradient.hpp"
#include "realpaver/BOPresolver.hpp"
#include "realpaver/BOSolver.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/IntContractor.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/ListContractor.hpp"
#include "realpaver/MaxCIDContractor.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

BOSolver::BOSolver(Problem& problem)
      : problem_(problem),
        preprob_(),
        solprob_(),
        param_(),
        model_(nullptr),
        localSolver_(nullptr),
        split_(nullptr),
        contractor_(nullptr),
        init_(nullptr),
        status_(OptimizationStatus::Other),
        sol_(problem.scope()),
        objval_(Interval::universe()),
        upper_(Double::inf()),
        nbnodes_(0),
        nbpending_(0),
        vmap21_(),
        vmap31_(),
        ptimer_(),
        stimer_()
{
   THROW_IF(!problem.isBOP(), "BO solver applied to a problem" <<
                              "that is not a BO problem");
}

BOSolver::~BOSolver()
{
   if (split_ != nullptr)       delete split_;
   if (localSolver_ != nullptr) delete localSolver_;
   if (model_ != nullptr)       delete model_;
}

double BOSolver::getPreprocessingTime() const
{
   return ptimer_.elapsedTime();
}

double BOSolver::getSolvingTime() const
{
   return stimer_.elapsedTime();
}

void BOSolver::loadParam(const std::string& filename)
{
   param_.loadParam(filename);
}

int BOSolver::getIntParam(const string& name)
{
   return param_.getIntParam(name);
}

void BOSolver::setIntParam(const string& name, int val)
{
   param_.setIntParam(name, val);
}

double BOSolver::getDblParam(const string& name)
{
   return param_.getDblParam(name);
}

void BOSolver::setDblParam(const string& name, double val)
{
   param_.setDblParam(name, val);
}

Tolerance BOSolver::getTolParam(const string& name)
{
   return param_.getTolParam(name);
}

void BOSolver::setTolParam(const string& name, const Tolerance& val)
{
   param_.setTolParam(name, val);
}

std::string BOSolver::getStrParam(const string& name)
{
   return param_.getStrParam(name);
}

void BOSolver::setStrParam(const string& name, const std::string& val)
{
   param_.setStrParam(name, val);
}

void BOSolver::printParam(std::ostream& os)
{
   param_.printParam(os);
}

int BOSolver::getNbNodes() const
{
   return nbnodes_;
}

int BOSolver::getNbPendingNodes() const
{
   return nbpending_;
}

void BOSolver::makeLocalSolver()
{
   std::string stra = param_.getStrParam("LOCAL_SOLVER_ALGORITHM");

   if (stra == "GRADIENT")
   {
      BOLocalGradient* solver = new BOLocalGradient();
      solver->setIterLimit(param_.getIntParam("LINE_SEARCH_ITER_LIMIT"));
      solver->setArmijoCoefficient(param_.getDblParam("LINE_SEARCH_ARMIJO"));
      solver->setStepTol(param_.getDblParam("LINE_SEARCH_STEP_TOL"));
      localSolver_ = solver;
   }

   if (stra == "EVALUATION" || localSolver_ == nullptr)
   {
      localSolver_ = new BOLocalSolver();
   }

   localSolver_->setTimeLimit(param_.getDblParam("LOCAL_SOLVER_TIME_LIMIT"));
}

void BOSolver::makeSplit()
{
   Selector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   bool osplit = (param_.getStrParam("SPLIT_OBJECTIVE") == "YES");
   Scope S = osplit ? model_->getFullScope() : model_->getObjScope();

   std::string sel = param_.getStrParam("SPLIT_SELECTOR");
   if (sel == "MAX_DOM") selector = new SelectorMaxDom(S);
   if (sel == "MAX_SMEAR") selector = new SelectorMaxSmear(model_, S);
   if (sel == "ROUND_ROBIN") selector = new SelectorRoundRobin(S);

   std::string sli = param_.getStrParam("SPLIT_SLICER");
   if (sli == "BISECTION") slicer = new IntervalBisecter();
   if (sli == "PEELING")
   {
      double f = param_.getDblParam("SPLIT_PEEL_FACTOR");
      slicer = new IntervalPeeler(f);
   }
   if (sli == "PARTITION")
   {
      size_t n = param_.getIntParam("SPLIT_NB_SLICES");
      slicer = new IntervalPartitioner(n);
   }

   THROW_IF(selector == nullptr || slicer == nullptr,
            "Unable to make the split object in a BO solver");

   split_ = new BOSplit(selector, slicer);
}

void BOSolver::makeHC4()
{
   SharedContractorVector vpool = std::make_shared<ContractorVector>();

   Dag* dag = model_->getDag();
   size_t i = 0;

   SharedIntContractor ic = std::make_shared<IntContractor>();

   // contractors associated with df / dv = 0
   for (Variable v : model_->getObjScope())
   {
      DagFun* dv = model_->getDerivative(i);
      SharedContractor op = std::make_shared<HC4Contractor>(dag, dv->index());

      if (model_->isBoundaryVar(v))
      {
         SharedContractor bop =
            std::make_shared<BOContractor>(dag, i, v, op, init_);

         vpool->push(bop);
      }
      else
      {
         vpool->push(op);
      }

      if (v.isDiscrete()) ic->insertVar(v);

      i = i+1;
   }

   // contractor associated with the objective constraint
   DagFun* of = model_->getObjConstraint();
   SharedContractor op = std::make_shared<HC4Contractor>(dag, of->index());
   vpool->push(op);

   SharedPropagator propagator = std::make_shared<Propagator>(vpool);
   propagator->setDistTol(param_.getTolParam("PROPAGATION_DTOL"));
   propagator->setMaxIter(param_.getIntParam("PROPAGATION_ITER_LIMIT"));

   if (ic->nbVars() > 0)
   {
      SharedContractorVector ipool = std::make_shared<ContractorVector>();
      ipool->push(propagator);
      ipool->push(ic);

      SharedListContractor lc = std::make_shared<ListContractor>(ipool);
      contractor_ = lc;
   }
   else
   {
      contractor_ = propagator;
   }
}

void BOSolver::makeMaxCIDHC4()
{
   makeHC4();

   std::unique_ptr<Selector> selector =
      std::make_unique<SelectorMaxDom>(model_->getObjScope());

   int nb = param_.getIntParam("SPLIT_NB_SLICES");
   std::unique_ptr<IntervalSlicer> slicer =
      std::make_unique<IntervalPartitioner>(nb);

   SharedContractor op =
      std::make_shared<MaxCIDContractor>(contractor_, std::move(selector),
                                         std::move(slicer));

   contractor_ = op;
}

void BOSolver::makeContractor()
{
   init_ = std::make_shared<IntervalRegion>(model_->getInitRegion());

   std::string algo = param_.getStrParam("PROPAGATION_ALGORITHM");

   if (algo == "HC4") makeHC4();
   if (algo == "MAX_CID_HC4") makeMaxCIDHC4();

   THROW_IF(contractor_ == nullptr, "No contractor in a BO solver");
}

bool BOSolver::preprocess()
{
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
   model_ = new BOModel(preprob_, false);

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

   // preprocessing after presolving
   Preprocessor preproc;
   bool pfeasible = preproc.apply(preprob_, solprob_);

   if (!pfeasible)
   {
      status_ = OptimizationStatus::Infeasible;
      return false;
   }

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
         }
      }
   }

   // initial problem solved if all the variables are fixed
   if (solprob_.nbVars() == 0)
   {
      Term t = preprob_.getObjective().getTerm();
      objval_ = t.eval(sol_);

      status_ = objval_.isEmpty() ? OptimizationStatus::Infeasible :
                                    OptimizationStatus::Optimal;
   }

   return true;
}

void BOSolver::calculateLower(SharedBONode& node)
{
   IntervalRegion* reg = node->getRegion();

   // domain of the objective variable after propagation
   Interval z = reg->get(model_->getObjVar());
   if (z.left() > node->getLower()) node->setLower(z.left());

   // TODO
}

void BOSolver::saveIncumbent(const RealPoint& pt)
{
   for (auto it = vmap31_.begin(); it != vmap31_.end(); ++it)
   {
      Variable sv = it->first;
      Variable v = it->second;

      double val = pt.get(sv);
      sol_.set(v, Interval(val));
   }
}

void BOSolver::calculateUpper(SharedBONode& node)
{
DEBUG("\ncalculateUpper, current upper bound " << upper_);

   IntervalRegion* reg = node->getRegion();
   RealPoint src = reg->midpointOnScope(model_->getObjScope());
   RealPoint dest(src);

   // local optimization
   OptimizationStatus status = localSolver_->minimize(*model_, *reg, src, dest);

DEBUG("   src = " << src);
DEBUG("   local optim -> " << status);

   if (status == OptimizationStatus::Optimal)
   {
DEBUG("   dest = " << dest);


      // safe interval evaluation at the final point
      Interval e = model_->ifunEvalPoint(dest);

DEBUG("   e = " << e);

      if (!e.isEmpty())
      {
         double u = e.right();
         node->setUpper(u);

      DEBUG("   UPPER BOUND " << u);

         // new upper bound of the global minimum?
         if (u < upper_)
         {
            upper_ = u;
            saveIncumbent(dest);
         }
      }
   }
}

bool BOSolver::bbStep(BOSpace& space, BOSpace& sol)
{
   // stops the search if the space is empty
   if (space.isEmpty()) return false;

   SharedBONode node = space.extractNode();

DEBUG("\n#########\nNODE : " << *node->getRegion() << " l: "
                             << node->getLower()
                             << " u: " << node->getUpper());

   // splits the node
   split_->apply(node);

   if (split_->getNbNodes() == 1)
   {
DEBUG("   sol NODE !");

      sol.insertNode(node);
      return true;
   }
   else
   {
      Variable v = model_->getObjVar();

      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         ++nbnodes_;

         SharedBONode subnode = *it;
         IntervalRegion* reg = subnode->getRegion();

         // BB theorem
         Interval z(reg->get(v));
         if (z.left() > upper_) continue;

         // assigns the upper bound before propagation
         if (z.right() > upper_)
         {
            z.setRight(upper_);
            reg->set(v, z);
         }
DEBUG("sub NODE : " << *reg);

         Proof proof = contractor_->contract(*reg);

DEBUG("proof : " << proof << "  -> " << *reg);

         if (proof != Proof::Empty)
         {
            calculateLower(subnode);
            calculateUpper(subnode);

DEBUG("    ... l : " << subnode->getLower());
DEBUG("    ... u : " << subnode->getUpper());

            THROW_IF(subnode->getLower() > subnode->getUpper(),
                     "Lower bound greater than upper bound in a BO node");

            space.insertNode(subnode);
         }
      }

      space.simplify(upper_);
      sol.simplify(upper_);

      return true;
   }
}

void BOSolver::findInitialBounds(SharedBONode& node)
{
   // upper bound of the global minimum
   upper_ = Double::inf();

   calculateLower(node);
   calculateUpper(node);

   Interval z(node->getLower(), node->getUpper());

   if (z.isEmpty())
   {
      status_ = OptimizationStatus::Infeasible;
      return;
   }

   node->getRegion()->set(model_->getObjVar(), z);

   DEBUG("Node bounds : " << z);

   // TODO, local optimization
   // restart, ...   
}

void BOSolver::branchAndBound()
{
DEBUG("-- branchAndBound with sol_ : " << sol_);

   // creates the algorithmic components
   makeLocalSolver();
   makeSplit();
   makeContractor();

   // creates the initial node
   SharedBONode node;
 
   bool osplit = (param_.getStrParam("SPLIT_OBJECTIVE") == "YES");

   if (osplit)
   {
      node = std::make_shared<BONode>(model_->getFullScope(),
                                      model_->getObjVar(),
                                      model_->getInitRegion());
   }
   else
   {   
      node = std::make_shared<BONode>(model_->getObjScope(),
                                      model_->getObjVar(),
                                      model_->getInitRegion());
   }

   // finds bounds of the objective in the initial node
   findInitialBounds(node);

   if (status_ == OptimizationStatus::Infeasible)
   {
      nbnodes_ = 1;
      return;
   }

   // creates the space of nodes to be processed
   BOSpace space;
   space.insertNode(node);
   space.setFrequency(param_.getIntParam("BB_SPACE_FREQUENCY"));

   // creates the space of solution nodes, i.e. nodes that cannot be split
   BOSpace sol;

   // parameters
   double timelimit = param_.getDblParam("TIME_LIMIT");
   int nodelimit = param_.getIntParam("NODE_LIMIT");
   Tolerance otol = param_.getTolParam("OBJ_TOL");

   bool iter = true;
   do
   {
      iter = bbStep(space, sol);

      double L = std::min(space.getLowestLowerBound(), sol.getLowestLowerBound());
      objval_ = Interval(L, upper_);

DEBUG("OBJ ENCLOSURE : " << objval_);

      if (iter && otol.hasTolerance(objval_))
      {
         iter = false;
         status_ = OptimizationStatus::Optimal;         
      }

      if (iter &&
          ptimer_.elapsedTime() + stimer_.elapsedTime() > timelimit)
      {
         iter = false;
         status_ = OptimizationStatus::StopOnTimeLimit;
      }

      if (iter && nbnodes_ > nodelimit)
      {
         iter = false;
         status_ = OptimizationStatus::StopOnNodeLimit;
      }
   }
   while (iter);

   nbpending_ = space.getNbNodes();
}

void BOSolver::solve()
{
   delete model_;

   // creates the solving model
   model_ = new BOModel(solprob_, true);

   // manages the status of every variable: interior or boundary
   for (auto it = vmap31_.begin(); it != vmap31_.end(); ++it)
   {   
      Variable sv = it->first;
      Variable v = it->second;

      Interval dsv = solprob_.getDomain(sv);
      Interval dv = problem_.getDomain(v);

      if (dv.strictlyContains(dsv))
      {
         model_->setInteriorVar(sv);
      }
      else
      {
         model_->setBoundaryVar(sv);
      }
   }

   // search
   branchAndBound();
}

bool BOSolver::optimize()
{
   DEBUG("---------- Input problem\n" << problem_);

   status_ = OptimizationStatus::Other;
   ptimer_.start();

   // first phase: preprocessing
   bool pfeasible = preprocess();

DEBUG("\n---------- Simplified problem\n" << preprob_);

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      ptimer_.stop();
      return pfeasible;
   }

   // second phase: presolving
   bool sfeasible = presolve();

DEBUG("\n---------- Presolved problem\n" << solprob_);

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      ptimer_.stop();
      return sfeasible;
   }

   ptimer_.stop();
   stimer_.start();

   // third phase: solving
   solve();

   stimer_.stop();

DEBUG("\nEND OF OPTIMIZATION");
DEBUG("sol = " << sol_ << "\n\n");

   return true;
}

OptimizationStatus BOSolver::getStatus() const
{
   return status_;
}

Interval BOSolver::getObjEnclosure() const
{
   return problem_.getObjective().isMinimization() ? objval_ : (-objval_);
}

RealPoint BOSolver::getBestSolution() const
{
   return sol_.midpoint();
}

} // namespace
