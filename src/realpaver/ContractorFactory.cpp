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
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/Linearizer.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ContractorFactory::ContractorFactory(const Problem &pbm, std::shared_ptr<Env> env)
    : env_(env)
    , dag_(nullptr)
    , ve_()
    , vi_()
    , vc_()
    , se_()
    , si_()
    , sc_()
{
   // environment
   if (env == nullptr)
      env_ = std::make_shared<Env>();

   // creates the shared DAG
   dag_ = std::make_shared<Dag>();

   // separates the constraints in three parts
   for (size_t i = 0; i < pbm.nbCtrs(); ++i)
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
      catch (Exception &e)
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

IntervalFunctionVector ContractorFactory::makeIntervalFunctionVector()
{
   IntervalFunctionVector F;
   for (size_t i : ve_)
      F.addFun(IntervalFunction(dag_, i));
   for (size_t i : vi_)
      F.addFun(IntervalFunction(dag_, i));
   return F;
}

SharedContractorHC4 ContractorFactory::makeHC4(double tol)
{
   // constraints from the dag
   SharedContractorHC4 hc4 = std::make_shared<ContractorHC4>(dag_);

   // other constraints
   for (const Constraint &c : vc_)
   {
      std::shared_ptr<ContractorConstraint> op =
          std::make_shared<ContractorConstraint>(c);
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
   double tol = env_->getParams()->getDblParam("PROPAGATION_TOL");
   return makeHC4(tol);
}

SharedContractorBC4 ContractorFactory::makeBC4()
{
   // constraints from the dag
   SharedContractorBC4 bc4 = std::make_shared<ContractorBC4>(dag_);

   // other constraints
   for (const Constraint &c : vc_)
   {
      std::shared_ptr<ContractorConstraint> op =
          std::make_shared<ContractorConstraint>(c);
      bc4->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      bc4->push(dop);

   // tuning of propagation
   double tol = env_->getParams()->getDblParam("PROPAGATION_TOL");
   bc4->setTol(tol);

   // tuning of BC4Revise operators
   double f = env_->getParams()->getDblParam("BC3_PEEL_FACTOR");
   bc4->setBC4RevisePeelFactor(f);

   int niter = env_->getParams()->getIntParam("BC3_ITER_LIMIT");
   bc4->setBC4ReviseMaxIter(niter);

   return bc4;
}

SharedContractorAffine ContractorFactory::makeAffine(double tol)
{
   // constraints from the dag
   SharedContractorAffine ctc = std::make_shared<ContractorAffine>(dag_);

   // other constraints
   for (const Constraint &c : vc_)
   {
      std::shared_ptr<ContractorConstraint> op =
          std::make_shared<ContractorConstraint>(c);
      ctc->push(op);
   }

   // variables with disconnected domains
   std::shared_ptr<ContractorDomain> dop = makeContractorDomain();

   if (dop->nbVars() > 0)
      ctc->push(dop);

   // tuning of propagation
   ctc->setTol(tol);

   return ctc;
}

SharedContractorAffine ContractorFactory::makeAffine()
{
   double tol = env_->getParams()->getDblParam("PROPAGATION_TOL");
   return makeAffine(tol);
}

std::shared_ptr<IntervalNewton> ContractorFactory::makeNewton()
{
   size_t ne = ve_.size(), nv = se_.size();

   if ((ne < 2) || (ne != nv))
      return nullptr;

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

      double tol = env_->getParams()->getDblParam("NEWTON_TOL");
      newton->setTol(tol);

      double wl = env_->getParams()->getDblParam("NEWTON_WIDTH_LIMIT");
      newton->setWidthLimit(wl);

      int niter = env_->getParams()->getIntParam("NEWTON_ITER_LIMIT");
      newton->setMaxIter(niter);

      double delta = env_->getParams()->getDblParam("INFLATION_DELTA");
      newton->setInflationDelta(delta);

      double chi = env_->getParams()->getDblParam("INFLATION_CHI");
      newton->setInflationChi(chi);

      tol = env_->getParams()->getDblParam("GAUSS_SEIDEL_TOL");
      newton->getGaussSeidel()->setTol(tol);

      niter = env_->getParams()->getIntParam("GAUSS_SEIDEL_ITER_LIMIT");
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

SharedContractorACID ContractorFactory::makeACID()
{
   if (dag_->isEmpty() || (!dag_->scope().contains(sc_)))
   {
      LOG_LOW("Unable to create an ACID contractor");
      return nullptr;
   }

   IntervalFunctionVector F = makeIntervalFunctionVector();
   std::unique_ptr<SelectorSSR> ssr = std::make_unique<SelectorSSR>(F);

   double tol = env_->getParams()->getDblParam("ACID_HC4_TOL");
   SharedContractorHC4 hc4 = makeHC4(tol);

   int ns3B = env_->getParams()->getIntParam("NB_SLICE_3B");
   int nsCID = env_->getParams()->getIntParam("NB_SLICE_CID");
   int learnLength = env_->getParams()->getIntParam("ACID_LEARN_LENGTH");
   int cycleLength = env_->getParams()->getIntParam("ACID_CYCLE_LENGTH");
   double ctRatio = env_->getParams()->getDblParam("ACID_CT_RATIO");
   double varMinWidth = env_->getParams()->getDblParam("VAR3BCID_MIN_WIDTH");

   return std::make_shared<ContractorACID>(std::move(ssr), hc4, ns3B, nsCID, learnLength,
                                           cycleLength, ctRatio, varMinWidth);
}

SharedContractorPolytope ContractorFactory::makePolytope()
{
   if (dag_->isEmpty())
      return nullptr;

   SharedContractorPolytope op = nullptr;

   bool ok = env_->getParams()->getStrParam("PROPAGATION_WITH_POLYTOPE_HULL") == "YES";

   if (ok)
   {
      std::string relaxation = env_->getParams()->getStrParam("POLYTOPE_HULL_RELAXATION");

      if (relaxation == "TAYLOR")
      {
         bool hansen =
             env_->getParams()->getStrParam("POLYTOPE_HULL_TAYLOR_HANSEN") == "YES";

         std::string corner =
             env_->getParams()->getStrParam("POLYTOPE_HULL_TAYLOR_CORNER");

         CornerStyle style;
         if (corner == "RANDOM_OPPOSITE")
            style = CornerStyle::RandomOpposite;
         else if (corner == "RANDOM")
            style = CornerStyle::Random;
         else if (corner == "LEFT")
            style = CornerStyle::Left;
         else if (corner == "RIGHT")
            style = CornerStyle::Right;

         unsigned seed =
             (unsigned)env_->getParams()->getIntParam("POLYTOPE_HULL_TAYLOR_SEED");

         std::unique_ptr<Linearizer> lzr =
             std::make_unique<LinearizerTaylor>(dag_, hansen, style, seed);

         op = std::make_shared<ContractorPolytope>(std::move(lzr));
      }
      else if (relaxation == "AFFINE")
      {
         bool minrange =
             env_->getParams()->getStrParam("POLYTOPE_HULL_AFFINE_APPROX") == "MINRANGE";

         std::unique_ptr<Linearizer> lzr =
             std::make_unique<LinearizerAffine>(dag_, minrange);

         op = std::make_shared<ContractorPolytope>(std::move(lzr));
      }
      else if (relaxation == "AFFINE_TAYLOR")
      {
         bool hansen =
             env_->getParams()->getStrParam("POLYTOPE_HULL_TAYLOR_HANSEN") == "YES";

         std::string corner =
             env_->getParams()->getStrParam("POLYTOPE_HULL_TAYLOR_CORNER");

         CornerStyle style;
         if (corner == "RANDOM_OPPOSITE")
            style = CornerStyle::RandomOpposite;
         else if (corner == "RANDOM")
            style = CornerStyle::Random;
         else if (corner == "LEFT")
            style = CornerStyle::Left;
         else if (corner == "RIGHT")
            style = CornerStyle::Right;

         unsigned seed =
             (unsigned)env_->getParams()->getIntParam("POLYTOPE_HULL_TAYLOR_SEED");

         std::unique_ptr<LinearizerTaylor> taylor =
             std::make_unique<LinearizerTaylor>(dag_, hansen, style, seed);

         ASSERT(taylor != nullptr, "taylor null");

         bool minrange =
             env_->getParams()->getStrParam("POLYTOPE_HULL_AFFINE_APPROX") == "MINRANGE";

         std::unique_ptr<LinearizerAffine> affine =
             std::make_unique<LinearizerAffine>(dag_, minrange);

         ASSERT(affine != nullptr, "affine null");

         std::unique_ptr<Linearizer> lzr = std::make_unique<LinearizerAffineTaylor>(
             dag_, std::move(affine), std::move(taylor));

         op = std::make_shared<ContractorPolytope>(std::move(lzr));
      }
   }

   if (op != nullptr)
   {
      double feas_tol = env_->getParams()->getDblParam("LP_FEAS_TOL");
      op->setFeasTol(feas_tol);

      double relax_tol = env_->getParams()->getDblParam("RELAXATION_EQ_TOL");
      op->setRelaxTol(relax_tol);

      int max_iter = env_->getParams()->getIntParam("LP_ITER_LIMIT");
      op->setMaxIter(max_iter);

      double max_time = env_->getParams()->getDblParam("LP_TIME_LIMIT");
      op->setMaxSeconds(max_time);

      bool loop = (env_->getParams()->getStrParam("POLYTOPE_HULL_LOOP") == "YES");
      double looptol = env_->getParams()->getDblParam("POLYTOPE_HULL_LOOP_TOL");
      op->enforceLoop(loop, looptol);
   }

   return op;
}

} // namespace realpaver
