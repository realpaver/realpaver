///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include "realpaver/BOContractor.hpp"
#include "realpaver/BOLocalGradient.hpp"
#include "realpaver/BOPresolver.hpp"
#include "realpaver/BOSolver.hpp"
#include "realpaver/HC4Contractor.hpp"
#include "realpaver/IntContractor.hpp"
#include "realpaver/IntervalSlicer.hpp"
#include "realpaver/ListContractor.hpp"
#include "realpaver/Logger.hpp"
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
        lpsolver_(nullptr),
        init_(nullptr),
        status_(OptimizationStatus::Other),
        sol_(problem.scope()),
        objval_(Interval::universe()),
        upper_(Double::inf()),
        nbnodes_(0),
        nbpending_(0),
        otol_(),
        vmap21_(),
        vmap31_(),
        ptimer_(),
        stimer_(),
        trace_(Param::GetStrParam("TRACE") == "YES")
{
   THROW_IF(!problem.isBOP(), "BO solver applied to a problem" <<
                              "that is not a BO problem");
}

BOSolver::~BOSolver()
{
   if (split_ != nullptr)       delete split_;
   if (localSolver_ != nullptr) delete localSolver_;
   if (model_ != nullptr)       delete model_;
   if (lpsolver_ != nullptr)    delete lpsolver_;
}

double BOSolver::getPreprocessingTime() const
{
   return ptimer_.elapsedTime();
}

double BOSolver::getSolvingTime() const
{
   return stimer_.elapsedTime();
}

void BOSolver::setParam(const Param& prm)
{
   param_ = prm;
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

   THROW_IF(localSolver_ == nullptr,
            "Unable to make the local solver in a BO solver");

   localSolver_->setTimeLimit(param_.getDblParam("LOCAL_SOLVER_TIME_LIMIT"));
}

void BOSolver::makeSplit()
{
   Selector* selector = nullptr;
   IntervalSlicer* slicer = nullptr;

   bool osplit = (param_.getStrParam("SPLIT_OBJECTIVE") == "YES");
   Scope sco = osplit ? model_->getFullScope() : model_->getObjScope();

   std::string sel = param_.getStrParam("SPLIT_SELECTOR");
   if (sel == "MAX_DOM") selector = new SelectorMaxDom(sco);
   if (sel == "MAX_SMEAR") selector = new SelectorMaxSmear(model_, sco);
   if (sel == "ROUND_ROBIN") selector = new SelectorRoundRobin(sco);

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

   std::unique_ptr<Selector> pselector(selector);
   std::unique_ptr<IntervalSlicer> pslicer(slicer);

   split_ = new BOSplit(std::move(pselector), std::move(pslicer));
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

   // Recreates the solution and eliminates the fake variables
   Scope sco = preproc.trueScope(); 
   sol_ = IntervalRegion(sco);

   LOG_INTER("Scope of the solution: " << sco);

   // assigns the fixed domains in the solution
   for (auto v : sco)
   {
      if (preproc.hasFixedDomain(v))
         sol_.set(v, preproc.getFixedDomain(v));

      else
      {
         Variable pv = preproc.srcToDestVar(v);
         vmap21_.insert(std::make_pair(pv, v));
      }
   }

   if (preproc.allVarsRemoved())
   {
      Term t = problem_.getObjective().getTerm();
      objval_ = t.eval(sol_);

      LOG_MAIN("All the variables are fixed");

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

void BOSolver::calculateLowerInit(SharedBONode& node)
{
   IntervalRegion* reg = node->region();

   // first: interval evaluation
   Interval e = model_->intervalEval(*reg);
   node->setLower(e.left());

   // second: relaxation
   calculateLower(node);
}

void BOSolver::calculateLower(SharedBONode& node)
{
   IntervalRegion* reg = node->region();

   // domain of the objective variable after propagation
   Interval z = reg->get(model_->getObjVar());
   if (z.left() > node->getLower()) node->setLower(z.left());

   LOG_LOW("Lower bound for node " << node->index()
                                   << " after  propagation: "
                                   << node->getLower());

   // linear relaxation
   if (lpsolver_ != nullptr) delete lpsolver_;
   lpsolver_ = new LPSolver;

   model_->linearize(*reg, *lpsolver_);

   // solving
   bool optimal = lpsolver_->optimize();
   if (optimal)
   {
      double lb = lpsolver_->getObjVal();

      if (lb > node->getLower())
      {
         LOG_INTER("Lower bound improved for node " << node->index()
                                                    << ": " << lb);
         node->setLower(lb);
      }
      else
      {
         LOG_INTER("Lower bound not improved for node " << node->index()
                                                        << " (" << lb << ")");
      }
   }
   else
   {
      LOG_INTER("Lower bound not found for node " << node->index());
   }
}

Proof BOSolver::reducePolytope(SharedBONode& node)
{
   IntervalRegion* reg = node->region();
   Scope sco = model_->getObjScope();
   Dag* dag = model_->getDag();

   for (Variable v : sco)
   {
      if (model_->isInteriorVar(v, *reg))
      {
         DagNode* node = dag->findVarNode(v.id());

         size_t iv = node->indexLinVar();
         LinVar lv = lpsolver_->getLinVar(iv);

         // minimize v
         LinExpr e( {1.0}, {lv} );
         lpsolver_->setObj(e, true);

         lpsolver_->reoptimize();
         OptimizationStatus status = lpsolver_->getStatus();

         if (status == OptimizationStatus::Infeasible) return Proof::Empty;

         else if (status == OptimizationStatus::Optimal)
         {
            Interval x = Interval::moreThan(lpsolver_->getObjVal() - 1.0e-8);
            Interval y = reg->get(v) & x;

            if (y.isEmpty()) return Proof::Empty;
            else reg->set(v, y);
         }

         else return Proof::Maybe;

         // maximize v
         lpsolver_->setObj(e, false);
         lpsolver_->reoptimize();
         status = lpsolver_->getStatus();

         if (status == OptimizationStatus::Infeasible) return Proof::Empty;

         if (status == OptimizationStatus::Optimal)
         {
            Interval x = Interval::lessThan(lpsolver_->getObjVal() + 1.0e-8);
            Interval y = reg->get(v) & x;

            if (y.isEmpty()) return Proof::Empty;
            else reg->set(v, y);
         }
      }
   }
   return Proof::Maybe;
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

void BOSolver::calculateUpperInit(SharedBONode& node)
{
   calculateUpper(node);
}

void BOSolver::calculateUpper(SharedBONode& node)
{
   IntervalRegion* reg = node->region();
   RealPoint src = reg->midpointOnScope(model_->getObjScope());
   RealPoint dest(src);

   // local optimization
   OptimizationStatus status = localSolver_->minimize(*model_, *reg, src, dest);

   if (status == OptimizationStatus::Optimal)
   {
      // safe interval evaluation at the final point
      Interval e = model_->intervalPointEval(dest);

      if (!e.isEmpty())
      {
         double u = e.right();
         node->setUpper(u);

         // new solution found?
         if (u < upper_)
         {
            saveIncumbent(dest);
            objval_ = otol_.maxIntervalDn(u);
            upper_ = objval_.left();

            if (trace_)
            {
               std::cout << "\t\t\t\tup: " << u << std::endl;
            }

            LOG_INTER("New upper bound of the global optimum: " << u);
            LOG_INTER("Refined upper bound: " << upper_);
            LOG_INTER("New incumbent solution: " << getBestSolution());
         }
      }
   }
}

bool BOSolver::bbStep(BOSpace& space, BOSpace& sol)
{
   // stops the search if the space is empty
   if (space.isEmpty())
   {
      LOG_INTER("Empty space in branch-and-bound step");
      return false;
   }

   LOG_INTER("Current enclosure of the optimum: " << objval_);
   LOG_INTER("Current lowest lower bound: " << space.getLowestLowerBound());

   SharedBONode node = space.extractNode();

   LOG_INTER("Extracts node " << node->index());

   // splits the node
   split_->apply(node);

   if (split_->getNbNodes() == 1)
   {
      LOG_INTER("Node small enough saved in the solution space");

      sol.insertNode(node);
      return true;
   }
   else
   {
      Variable v = model_->getObjVar();

      LOG_INTER("Splits node " << node->index() << " > "
                               << split_->getNbNodes() << " sub-nodes");

      for (auto it = split_->begin(); it != split_->end(); ++it)
      {
         ++nbnodes_;

         SharedBONode subnode = *it;
         IntervalRegion* reg = subnode->region();

         subnode->setIndex(nbnodes_);

         LOG_INTER("Node " << subnode->index() << ": " << *reg);

         // BB theorem
         Interval z(reg->get(v));
         if (z.left() > upper_)
         {
            LOG_INTER("Node fathomed by BB theorem");
            continue;
         }

         // assigns the upper bound before propagation
         if (z.right() > upper_)
         {
            z.setRight(upper_);
            reg->set(v, z);
         }

         Proof proof = contractor_->contract(*reg);
         LOG_INTER("Contraction -> " << proof);

         if (proof != Proof::Empty)
         {
            LOG_INTER("New region: " << *reg);

            calculateLower(subnode);
            calculateUpper(subnode);

            THROW_IF(subnode->getLower() > subnode->getUpper(),
                     "Lower bound greater than upper bound in a BO node");

            // must be called after calculateLower
            proof = reducePolytope(subnode);
            LOG_INTER("Polytope reduction -> " << proof);

            if (proof != Proof::Empty)
            {
               LOG_INTER("New region:  " << *subnode->region());
               LOG_INTER("Inserts node " << subnode->index() << " in the space");
               space.insertNode(subnode);
            }
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

   calculateLowerInit(node);
   calculateUpperInit(node);

   LOG_INTER("Upper bound: " << node->getUpper());
   LOG_INTER("Lower bound: " << node->getLower());

   Interval z(node->getLower(), node->getUpper());

   if (z.isEmpty())
   {
      LOG_MAIN("Lower bound > upper bound");
      status_ = OptimizationStatus::Infeasible;
      return;
   }

   node->region()->set(model_->getObjVar(), z);
}

void BOSolver::branchAndBound()
{
   LOG_MAIN("Branch-and-bound algorithm");

   // creates the algorithmic components
   makeLocalSolver();
   makeSplit();
   makeContractor();

   // parameters
   double timelimit = param_.getDblParam("TIME_LIMIT");
   int nodelimit = param_.getIntParam("NODE_LIMIT");
   otol_ = param_.getTolParam("OBJ_TOL");
   trace_ = (param_.getStrParam("TRACE") == "YES");

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

   node->setIndex(0);

   LOG_INTER("Node " << node->index() << ": " << *node->region());

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

   LOG_INTER("Tolerance on the global optimum: " << otol_);
   LOG_INTER("Time limit: " << timelimit << "s");
   LOG_INTER("Node limit: " << nodelimit);

   bool iter = true;

   double Lprev = Double::neginf(),
          L = Double::neginf();

   do
   {
      iter = bbStep(space, sol);

      Lprev = L;
      L = std::min(space.getLowestLowerBound(), sol.getLowestLowerBound());

      if (trace_ && (L != Lprev) && (!space.isEmpty()))
      {
         std::cout << std::setprecision(12) << "\tlo: " << L << std::endl;
      }

      if (space.isEmpty())
      {
         LOG_MAIN("Stop since the space is empty");
         iter = false;
         status_ = OptimizationStatus::Optimal;         
      }

      if (iter && L >= objval_.left())
      {
         LOG_MAIN("Stop on global optimum at desired tolerance");
         iter = false;
         status_ = OptimizationStatus::Optimal;         
         objval_.setLeft(L);
      }

      if (iter &&
          ptimer_.elapsedTime() + stimer_.elapsedTime() > timelimit)
      {
         LOG_MAIN("Stop on time limit (" << timelimit << "s)");
         iter = false;
         status_ = OptimizationStatus::StopOnTimeLimit;
         objval_.setLeft(L);
      }

      if (iter && nbnodes_ > nodelimit)
      {
         LOG_MAIN("Stop on node limit (" << nodelimit << ")");
         iter = false;
         status_ = OptimizationStatus::StopOnNodeLimit;
         objval_.setLeft(L);
      }
   }
   while (iter);

   nbpending_ = space.getNbNodes();

   LOG_MAIN("Number of nodes processed: " << nbnodes_+1);
}

void BOSolver::solve()
{
   delete model_;

   LOG_INTER("Creates the solving model");

   // creates the solving model
   model_ = new BOModel(solprob_, true);

   LOG_LOW("DAG of presolved problem\n" << *model_->getDag());

   // manages the status of every variable: interior or boundary
   for (auto it = vmap31_.begin(); it != vmap31_.end(); ++it)
   {   
      Variable sv = it->first;
      Variable v = it->second;

      Interval dsv = solprob_.getDomain(sv);
      Interval dv = problem_.getDomain(v);

      if (dv.strictlyContains(dsv))
      {
         LOG_INTER("Interior variable: " << v.getName());
         model_->setInteriorVar(sv);
      }
      else
      {
         LOG_INTER("Boundary variable: " << v.getName());
         model_->setBoundaryVar(sv);
      }
   }

   // search
   branchAndBound();
}

bool BOSolver::optimize()
{
   LOG_MAIN("Input problem\n" << problem_);

   status_ = OptimizationStatus::Other;
   ptimer_.start();

   // first phase: preprocessing
   bool pfeasible = preprocess();

   LOG_MAIN("Simplified problem\n" << preprob_);

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      ptimer_.stop();

      LOG_MAIN("Problem solved after the preprocessing phase");
      LOG_MAIN("Preprocessing time: " << ptimer_.elapsedTime() << "s");

      return pfeasible;
   }

   // second phase: presolving
   bool sfeasible = presolve();

   LOG_MAIN("Presolved problem\n" << solprob_);

   if (status_ == OptimizationStatus::Infeasible ||
       status_ == OptimizationStatus::Optimal)
   {
      ptimer_.stop();

      LOG_MAIN("Problem solved after the presolving phase");
      LOG_MAIN("Preprocessing time: " << ptimer_.elapsedTime() << "s");

      return sfeasible;
   }

   ptimer_.stop();
   stimer_.start();

   // third phase: solving
   solve();

   stimer_.stop();

   LOG_MAIN("Preprocessing time: " << ptimer_.elapsedTime() << "s");
   LOG_MAIN("Solving time: " << stimer_.elapsedTime() << "s");
   LOG_MAIN("Optimization status: " << status_);

   if (status_ == OptimizationStatus::Optimal)
   {
      LOG_MAIN("Best point found: " << getBestSolution());
      LOG_MAIN("Global optimum: " << getObjEnclosure());
   }

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
