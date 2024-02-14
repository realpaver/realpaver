///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorBC4Revise.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/ContractorHC4.hpp"
#include "realpaver/ContractorList.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ContractorFactory::ContractorFactory(const Problem& pbm,
                                     std::shared_ptr<Env> env)
      : env_(env),
        dag_(nullptr),
        ve_(),
        vi_(),
        vc_(),
        se_(),
        si_(),
        sc_()
{
   // environment
   if (env == nullptr)
      env_ = std::make_shared<Env>();

   // creates the shared DAG
   dag_ = std::make_shared<Dag>();

   // separates the constraints in three parts
   for (size_t i=0; i<pbm.nbCtrs(); ++i)
   {
      Constraint c = pbm.ctrAt(i);
      try
      {
         size_t j = dag_->insert(c);
         if (c.isEquation())
         {
            ve_.push_back(j);
            se_.insert(c.scope());
         }
         else
         {
            vi_.push_back(j);
            si_.insert(c.scope());
         }
      }
      catch(Exception& e)
      {
         vc_.push_back(c);
         sc_.insert(c.scope());
      }
   }

   // inserts the scopes in the bank
   se_ = ScopeBank::getInstance()->insertScope(se_);
   si_ = ScopeBank::getInstance()->insertScope(si_);
   sc_ = ScopeBank::getInstance()->insertScope(sc_);
}

std::shared_ptr<Env> ContractorFactory::getEnv() const
{
   return env_;
}

SharedDag ContractorFactory::ContractorFactory::getDag() const
{
   return dag_;
}

std::shared_ptr<IntervalSmearSumRel> ContractorFactory::makeSSR()
{
   if (dag_->isEmpty()) return nullptr;

   std::shared_ptr<IntervalSmearSumRel> ssr;

   if (vc_.empty())
   {
      IntervalFunctionVector F(dag_);
      ssr = std::make_shared<IntervalSmearSumRel>(F);
   }
   else
   {
      IntervalFunctionVector F;
      for (size_t i : ve_)
         F.addFun(IntervalFunction(dag_, i));

      for (size_t i : vi_)
         F.addFun(IntervalFunction(dag_, i));

      ssr = std::make_shared<IntervalSmearSumRel>(F);
   }

   return ssr;
}

SharedContractorPropag ContractorFactory::makeHC4()
{
   SharedContractorVector pool = std::make_shared<ContractorVector>();

   // equations
   for (size_t i : ve_)
   {
      std::shared_ptr<ContractorHC4>
         op = std::make_shared<ContractorHC4>(dag_, i);
      pool->push(op);
   }

   // inequality constraints
   for (size_t i : vi_)
   {
      std::shared_ptr<ContractorHC4>
         op = std::make_shared<ContractorHC4>(dag_, i);
      pool->push(op);
   }

   // constraints
   for (const Constraint& c : vc_)
   {
      std::shared_ptr<ContractorConstraint>
         op = std::make_shared<ContractorConstraint>(c);
      pool->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      pool->push(dop);

   SharedContractorPropag hc4 = std::make_shared<ContractorPropag>(pool);

   double rtol = env_->getParam()->getDblParam("PROPAGATION_REL_TOL");
   hc4->setTol(Tolerance(rtol, 0.0));

   int niter = env_->getParam()->getIntParam("PROPAGATION_ITER_LIMIT");
   hc4->setMaxIter(niter);

   return hc4;
}

SharedContractorPropag ContractorFactory::makeBC4()
{
   SharedContractorVector pool = std::make_shared<ContractorVector>();

   // equations
   for (size_t i : ve_)
   {
      std::shared_ptr<ContractorBC4Revise>
         op = std::make_shared<ContractorBC4Revise>(dag_, i);
      pool->push(op);
   }

   // inequality constraints
   for (size_t i : vi_)
   {
      std::shared_ptr<ContractorBC4Revise>
         op = std::make_shared<ContractorBC4Revise>(dag_, i);
      pool->push(op);
   }

   // constraints
   for (const Constraint& c : vc_)
   {
      std::shared_ptr<ContractorConstraint>
         op = std::make_shared<ContractorConstraint>(c);
      pool->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      pool->push(dop);

   SharedContractorPropag bc4 = std::make_shared<ContractorPropag>(pool);

   double rtol = env_->getParam()->getDblParam("PROPAGATION_REL_TOL");
   double atol = env_->getParam()->getDblParam("PROPAGATION_ABS_TOL");
   bc4->setTol(Tolerance(rtol, atol));

   int niter = env_->getParam()->getIntParam("PROPAGATION_ITER_LIMIT");
   bc4->setMaxIter(niter);

   return bc4;
}

std::shared_ptr<IntervalNewton> ContractorFactory::makeIntervalNewton()
{
   size_t ne = ve_.size(),
          nv = se_.size();
   
   if ((ne < 2) || (ne != nv)) return nullptr;

   std::shared_ptr<IntervalNewton> newton;

   if (ne == dag_->nbFuns())
   {
      IntervalFunctionVector F(dag_);
      newton = std::make_shared<IntervalNewton>(F);
   }
   else
   {
      IntervalFunctionVector F;
      for (size_t i : ve_)
      {
         IntervalFunction g(dag_, i);
         F.addFun(g);
      }
      newton = std::make_shared<IntervalNewton>(F);
   }

   if (newton != nullptr)
   {
      double rtol = env_->getParam()->getDblParam("NEWTON_REL_TOL");
      newton->setTol(Tolerance(rtol, 0.0));

      int niter = env_->getParam()->getIntParam("NEWTON_ITER_LIMIT");
      newton->setMaxIter(niter);

      double delta = env_->getParam()->getDblParam("INFLATION_DELTA");
      newton->setInflationDelta(delta);

      double chi = env_->getParam()->getDblParam("INFLATION_CHI");
      newton->setInflationChi(chi);

      rtol  = env_->getParam()->getDblParam("GAUSS_SEIDEL_REL_TOL");
      newton->getGaussSeidel()->setTol(Tolerance(rtol, 0.0));

      niter = env_->getParam()->getIntParam("GAUSS_SEIDEL_ITER_LIMIT");
      newton->getGaussSeidel()->setMaxIter(niter);
   }

   return newton;
}

std::shared_ptr<ContractorDomain> ContractorFactory::makeContractorDomain()
{
   std::shared_ptr<ContractorDomain> op = std::make_shared<ContractorDomain>();

   for (Variable v : se_)
      if (!v.getDomain()->isConnected())
         op->insertVar(v);

   for (Variable v : si_)
      if (!v.getDomain()->isConnected())
         op->insertVar(v);

   for (Variable v : sc_)
      if (!v.getDomain()->isConnected())
         op->insertVar(v);

   return op;
}

SharedContractor ContractorFactory::makeHC4Newton()
{
   SharedContractorPropag hc4 = makeHC4();
   std::shared_ptr<IntervalNewton> newton = makeIntervalNewton();

   if (newton != nullptr)
   {
      SharedContractorVector pool = std::make_shared<ContractorVector>();
      pool->push(hc4);
      pool->push(newton);
      return std::make_shared<ContractorList>(pool);
   }
   else
   {
      return hc4;
   }
}

SharedContractorACID ContractorFactory::makeACID()
{
   if (dag_->isEmpty()) return nullptr;

   std::shared_ptr<IntervalSmearSumRel> ssr = makeSSR();

   SharedContractorPropag hc4 = makeHC4();

   int ns3B = env_->getParam()->getIntParam("NB_SLICE_3B");
   int nsCID = env_->getParam()->getIntParam("NB_SLICE_CID");
   int learnLength = env_->getParam()->getIntParam("ACID_LEARN_LENGTH");
   int cycleLength = env_->getParam()->getIntParam("ACID_CYCLE_LENGTH");
   double ctRatio = env_->getParam()->getDblParam("ACID_CT_RATIO");

   return std::make_shared<ContractorACID>(ssr, hc4, ns3B, nsCID, learnLength,
                                           cycleLength, ctRatio);
}

SharedContractorPolytope ContractorFactory::makePolytope()
{
   if (dag_->isEmpty()) return nullptr;

   SharedContractorPolytope op = nullptr;
   std::string
      creator = env_->getParam()->getStrParam("PROPAGATION_WITH_POLYTOPE");

   if (creator == "TAYLOR")
      op = std::make_shared<ContractorPolytope>(dag_,
                                                PolytopeStyle::Taylor);

   else if (creator == "RLT")
      op = std::make_shared<ContractorPolytope>(dag_,
                                                PolytopeStyle::RLT);

   if (op != nullptr)
   {
      double tol = env_->getParam()->getDblParam("RELAXATION_EQ_TOL");
      op->setRelaxEqTol(tol);
   }

   return op;
}
 
} // namespace
