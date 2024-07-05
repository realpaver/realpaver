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
 * @file   ContractorFactory.cpp
 * @brief  Factory of interval contractors
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorBC4Revise.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/ContractorList.hpp"
#include "realpaver/IntervalSmearSumRel.hpp"
#include "realpaver/Logger.hpp"
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

SharedContractorHC4 ContractorFactory::makeHC4(Tolerance tol)
{
   // constraints from the dag
   SharedContractorHC4 hc4 = std::make_shared<ContractorHC4>(dag_);

   // other constraints
   for (const Constraint& c : vc_)
   {
      std::shared_ptr<ContractorConstraint>
         op = std::make_shared<ContractorConstraint>(c);
      hc4->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      hc4->push(dop);

   // tuning of propagation
   hc4->setTol(tol);

   return hc4;
}

SharedContractorHC4 ContractorFactory::makeHC4()
{
   double rtol = env_->getParam()->getDblParam("PROPAGATION_REL_TOL");
   return makeHC4(Tolerance(rtol, 0.0));
}

SharedContractorBC4 ContractorFactory::makeBC4()
{
   // constraints from the dag
   SharedContractorBC4 bc4 = std::make_shared<ContractorBC4>(dag_);

   // other constraints
   for (const Constraint& c : vc_)
   {
      std::shared_ptr<ContractorConstraint>
         op = std::make_shared<ContractorConstraint>(c);
      bc4->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      bc4->push(dop);

   // tuning of propagation
   double val = env_->getParam()->getDblParam("PROPAGATION_REL_TOL");
   bc4->setTol(Tolerance(val, 0.0));

   // tuning of BC4Revise operators
   val = env_->getParam()->getDblParam("BC3_PEEL_FACTOR");
   bc4->setBC4RevisePeelFactor(val);

   int niter = env_->getParam()->getIntParam("BC3_ITER_LIMIT");
   bc4->setBC4ReviseMaxIter(niter);   

   return bc4;
}

std::shared_ptr<IntervalNewton> ContractorFactory::makeNewton()
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
      LOG_LOW("Newton operator built by the factory");
      
      double rtol = env_->getParam()->getDblParam("NEWTON_REL_TOL");
      newton->setTol(Tolerance(rtol, 0.0));

      double wl = env_->getParam()->getDblParam("NEWTON_WIDTH_LIMIT");
      newton->setWidthLimit(wl);

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
   else
   {
      LOG_LOW("Newton operator NOT built by the factory");
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
   SharedContractorHC4 hc4 = makeHC4();
   std::shared_ptr<IntervalNewton> newton = makeNewton();

   if (newton != nullptr)
   {
      SharedContractorPool pool = std::make_shared<ContractorPool>();
      pool->push(hc4);
      pool->push(newton);
      return std::make_shared<ContractorList>(pool);
   }
   else
   {
      return hc4;
   }
}

SharedContractor ContractorFactory::makeACID()
{
   if (dag_->isEmpty() || (!dag_->scope().contains(sc_)))
   {
      LOG_LOW("Unable to create an ACID contractor -> HC4 instead");
      return makeHC4();
   }

   std::shared_ptr<IntervalSmearSumRel> ssr = makeSSR();

   double rtol = env_->getParam()->getDblParam("ACID_HC4_REL_TOL");
   SharedContractorHC4 hc4 = makeHC4(Tolerance(rtol, 0.0));

   int ns3B = env_->getParam()->getIntParam("NB_SLICE_3B");
   int nsCID = env_->getParam()->getIntParam("NB_SLICE_CID");
   int learnLength = env_->getParam()->getIntParam("ACID_LEARN_LENGTH");
   int cycleLength = env_->getParam()->getIntParam("ACID_CYCLE_LENGTH");
   double ctRatio = env_->getParam()->getDblParam("ACID_CT_RATIO");
   double varMinWidth = env_->getParam()->getDblParam("VAR3BCID_MIN_WIDTH");

   return std::make_shared<ContractorACID>(ssr, hc4, ns3B, nsCID, learnLength,
                                           cycleLength, ctRatio, varMinWidth);
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
