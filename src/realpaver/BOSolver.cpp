///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/BOContractor.hpp"
#include "realpaver/BOPresolver.hpp"
#include "realpaver/BOSolver.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Selector.hpp"

namespace realpaver {

BOSolver::BOSolver(Problem& problem)
      : problem_(problem),
        preprob_(),
        solprob_(),
        model_(nullptr),
        localSolver_(nullptr),
        split_(nullptr),
        contractor_(nullptr),
        pool_(nullptr),
        init_(nullptr),
        status_(OptimizationStatus::Other),
        sol_(problem.nbVars()),
        objval_(Interval::universe()),
        vmap21_(),
        vmap31_(),
        ptimer_(),
        stimer_(),
        timelimit_(Param::getDblParam("TIME_LIMIT")),
        nodelimit_(Param::getIntParam("NODE_LIMIT")),
        splitobj_(Param::getStrParam("SPLIT_OBJ") == "YES")
{
   THROW_IF(!problem.isBOP(), "BO solver applied to a problem" <<
                              "that is not a BO problem");
}

BOSolver::~BOSolver()
{
   if (contractor_ != nullptr)  delete contractor_;
   if (pool_ != nullptr)        delete pool_;
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

size_t BOSolver::getNodeLimit() const
{
   return nodelimit_;
}

void BOSolver::setNodeLimit(size_t n)
{
   ASSERT(n > 0, "Bad node limit in a BO solver");

   nodelimit_ = n;
}

bool BOSolver::isSplitableObj() const
{
   return splitobj_;
}

void BOSolver::setSplitableObj(bool split)
{
   splitobj_ = split;
}

void BOSolver::makeLocalSolver()
{
   // default local solver
   localSolver_ = new BOLocalSolver();
}

void BOSolver::makeSplit()
{
   Selector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   const Scope& S = isSplitableObj() ? model_->getFullScope() :
                                       model_->getObjScope();

   std::string sel = Param::getStrParam("SPLIT_SELECTOR");
   if (sel == "MaxDom") selector = new SelectorMaxDom(S);
   if (sel == "MaxSmear") selector = new SelectorMaxSmear(model_, S);

   std::string sli = Param::getStrParam("SPLIT_SLICER");
   if (sli == "Bisection") slicer = new IntervalBisecter();
   if (sli == "Peeling")
   {
      double f = Param::getDblParam("SPLIT_PEEL_FACTOR");
      slicer = new IntervalPeeler(f);
   }
   if (sli == "Partition")
   {
      size_t n = Param::getIntParam("SPLIT_NB_SLICES");
      slicer = new IntervalPartitioner(n);
   }

   THROW_IF(selector == nullptr || slicer == nullptr,
            "Unable to make the split object in a BO solver");

   split_ = new BOSplit(selector, slicer);
}

void BOSolver::makeHC4()
{
DEBUG("MAKE HC4");

   ContractorVector* vpool = new ContractorVector();

   Dag* dag = model_->getDag();
   size_t i = 0;

   // contractors associated with df / dv = 0
   for (Variable v : model_->getObjScope())
   {

DEBUG("     hc4 for var " << v.getName());

      SharedContractor op = std::make_shared<HC4Contractor>(dag, i);

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

      i = i+1;
   }

   // contractor associated with the objective constraint
   SharedContractor op = std::make_shared<HC4Contractor>(dag, dag->nbFun()-1);
   vpool->push(op);

DEBUG("     hc4 for fun " << dag->nbFun()-1);

   pool_ = vpool;
   contractor_ = new Propagator(pool_);
}

void BOSolver::makeContractor()
{
   init_ = std::make_shared<IntervalVector>(model_->getInitRegion());

   std::string algo = Param::getStrParam("PROPAGATOR_ALGORITHM");

   if (algo == "HC4") makeHC4();

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
   // TODO
}

void BOSolver::calculateUpper(SharedBONode& node)
{
   IntervalVector* X = node->getRegion();
   RealVector m = X->midpoint();
   RealVector p(X->size());

   // domain of the objective variable after propagation
   Interval z(X->operator[](model_->getObjVar()));
   node->setUpper(z.right());

   // local optimization
   OptimizationStatus status = localSolver_->minimize(*model_, *X, m, p);

   if (status == OptimizationStatus::Optimal)
   {
      // safe interval evaluation at the final point
      Interval e = model_->ifunEvalPoint(p);

      if (!e.isEmpty())
      {
         double u = e.right();
         if (u < z.right())
         {
            node->setUpper(u);
            z.setRight(u);
            X->set(model_->getObjVar(), z);

      DEBUG("   UPPER BOUND " << u);
         }
      }
   }
}

bool BOSolver::bbStep(BOSpace& space, BOSpace& sol)
{
   // stops the search if the space is empty
   if (space.isEmpty()) return false;

   SharedBONode node = space.extractNode();


DEBUG("NODE : " << *node->getRegion());

   // splits the node
   split_->apply(node);

   if (split_->getNbNodes() == 1)
   {
DEBUG("   sol NODE !");


      sol.insertNode(node);
      return false;
   }
   else
   {
      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         SharedBONode subnode = *it;
         IntervalVector* X = subnode->getRegion();

DEBUG("sub NODE : " << *X);


         Proof proof = contractor_->contract(*X);

DEBUG("proof : " << proof);

         if (proof != Proof::Empty)
         {
            calculateLower(subnode);
            calculateUpper(subnode);

            THROW_IF(subnode->getLower() > subnode->getUpper(),
                     "Lower bound greater than upper bound in a BO node");



   // TODO, simplifier les espaces
   // TODO, calculer un encadrement de l'optimum


            space.insertNode(subnode);
         }
      }
      return true;
   }
}

void BOSolver::findInitialBounds(SharedBONode& node)
{
   IntervalVector* region = node->getRegion();

   Interval val = model_->ifunEval(*region);

   if (val.isEmpty())
   {
      status_ = OptimizationStatus::Infeasible;
      return;
   }

   node->setLower(val.left());
   node->setUpper(val.right());

   region->set(model_->getObjVar(), val);

   DEBUG("Node bounds : " << node->getLower() << ", " << node->getUpper());

   // TODO, local optimization


   // TODO, first relaxation
   
}

void BOSolver::branchAndBound()
{
DEBUG("-- branchAndBound");

   // creates the algorithmic components
   makeLocalSolver();
   makeSplit();
   makeContractor();

   // creates the initial node
   SharedBONode node;
 
   if (isSplitableObj())
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

   if (status_ == OptimizationStatus::Infeasible) return;

   // creates the space of nodes to be processed
   BOSpace space;
   space.insertNode(node);

   // creates the space of solution nodes, i.e. nodes that cannot be split
   BOSpace sol;

   bool iter = true;

   do
   {
      iter = bbStep(space, sol);

      if (iter &&
          ptimer_.elapsedTime() + stimer_.elapsedTime() > getTimeLimit())
      {
         iter = false;
         status_ = OptimizationStatus::StopOnTimeLimit;
      }

      if (iter && space.getNbNodes() > nodelimit_)
      {
         iter = false;
         status_ = OptimizationStatus::StopOnNodeLimit;
      }
   }
   while (iter);

   // assigns the enclosure of the optimum
   // objval_, TODO
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

void BOSolver::setTimeLimit(double t)
{
   ASSERT(t > 0.0, "Bad time limit for a BO solver");

   timelimit_ = t;
}

double BOSolver::getTimeLimit() const
{
   return timelimit_;
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
