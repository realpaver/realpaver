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
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorConstraint.hpp"
#include "realpaver/ContractorDomain.hpp"
#include "realpaver/ContractorFactory.hpp"
#include "realpaver/ContractorHC4Revise.hpp"

#include "realpaver/Exception.hpp"
#include "realpaver/LinearizerAffine.hpp"
#include "realpaver/LinearizerAffineTaylor.hpp"
#include "realpaver/LinearizerTaylor.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ContractorFactory::ContractorFactory(Problem &pbm, std::shared_ptr<Env> env)
    : env_(env)
    , edag_(nullptr)
    , cse_()
    , csi_()
    , cso_()
    , cs_()
{
   // environment
   if (env == nullptr)
      env_ = std::make_shared<Env>();

   // creates the shared DAG
   edag_ = std::make_shared<DagExtended>(pbm);

   // separates the constraints in three parts
   for (size_t i = 0; i < pbm.nbCtrs(); ++i)
   {
      // inserts c in the different constraint sets
      Constraint c = pbm.ctrAt(i);
      cs_.insert(c);
      if (c.isEquation())
      {
         cse_.insert(c);
      }
      else if (c.isInequality())
      {
         csi_.insert(c);
      }
      else
      {
         cso_.insert(c);
      }
   }
}

std::shared_ptr<Env> ContractorFactory::getEnv() const
{
   return env_;
}

SharedDagExtended ContractorFactory::ContractorFactory::extDag() const
{
   return edag_;
}

SharedContractorHC4 ContractorFactory::makeHC4(double tol)
{
   // constraints from the dag
   SharedContractorHC4 hc4 = std::make_shared<ContractorHC4>();

   // constraints
   for (const Constraint &c : cs_)
   {
      std::shared_ptr<ContractorHC4Revise> op = std::make_shared<ContractorHC4Revise>(c);
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

SharedContractorAffine ContractorFactory::makeAffine(double tol)
{
   bool minrange = env_->getParams()->getStrParam("AFFINE_APPROXIMATION") == "MINRANGE";

   // constraints from the dag
   SharedContractorAffine ctc = std::make_shared<ContractorAffine>(edag_, minrange);

   // other constraints
   for (size_t i = 0; i < edag_->nbBadCtrs(); ++i)
   {
      std::shared_ptr<ContractorConstraint> op =
          std::make_shared<ContractorConstraint>(edag_->badCtr(i));
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
   IntervalFunctionVector F;
   for (size_t i = 0; i < edag_->size(); ++i)
   {
      DagFunExtEquationFinder finder;
      edag_->fun(i)->acceptVisitor(finder);
      if (finder.isAnEquation())
      {
         IntervalFunction g(edag_->dag()->fun(finder.index()));
         F.addFun(g);
      }
   }

   std::shared_ptr<IntervalNewton> newton;

   try
   {
      newton = std::make_shared<IntervalNewton>(F);

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
      newton->getGaussSeidel().setTol(tol);

      niter = env_->getParams()->getIntParam("GAUSS_SEIDEL_ITER_LIMIT");
      newton->getGaussSeidel().setMaxIter(niter);
   }
   catch (Exception &e)
   {
      LOG_LOW("Newton operator NOT built by the factory");
      newton = nullptr;
   }
   return newton;
}

std::shared_ptr<ContractorDomain> ContractorFactory::makeContractorDomain()
{
   std::shared_ptr<ContractorDomain> op = std::make_shared<ContractorDomain>();

   for (const Variable &v : cs_.scope())
      if (!v.getDomain()->isConnected())
         op->insertVar(v);

   return op;
}

SharedContractorACID ContractorFactory::makeACID()
{
   if (edag_->isEmpty())
   {
      LOG_LOW("Unable to create an ACID contractor");
      return nullptr;
   }

   IntervalFunctionVector F(edag_);
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
   if (edag_->isEmpty())
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
             std::make_unique<LinearizerTaylor>(edag_, hansen, style, seed);

         op = std::make_shared<ContractorPolytope>(std::move(lzr));
      }
      else if (relaxation == "AFFINE")
      {
         bool minrange =
             env_->getParams()->getStrParam("AFFINE_APPROXIMATION") == "MINRANGE";

         std::unique_ptr<LinearizerAffine> lzr =
             std::make_unique<LinearizerAffine>(edag_, minrange);

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
         else if (corner == "LEFT_RIGHT")
            style = CornerStyle::LeftRight;

         unsigned seed =
             (unsigned)env_->getParams()->getIntParam("POLYTOPE_HULL_TAYLOR_SEED");

         std::unique_ptr<LinearizerTaylor> taylor =
             std::make_unique<LinearizerTaylor>(edag_, hansen, style, seed);

         REALPAVER_ASSERT(taylor != nullptr, "taylor null");

         bool minrange =
             env_->getParams()->getStrParam("AFFINE_APPROXIMATION") == "MINRANGE";

         std::unique_ptr<LinearizerAffine> affine =
             std::make_unique<LinearizerAffine>(edag_, minrange);

         REALPAVER_ASSERT(affine != nullptr, "affine null");

         std::unique_ptr<Linearizer> lzr = std::make_unique<LinearizerAffineTaylor>(
             std::move(affine), std::move(taylor));

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

/*----------------------------------------------------------------------------*/

DagFunExtEquationFinder::DagFunExtEquationFinder()
    : DagFunExtVisitor()
    , idx_(-1)
{
}

void DagFunExtEquationFinder::apply(const DagFunArith *f)
{
   SharedDag dag = f->dag();
   int i = f->index();
   if (dag->fun(i)->getImage().isSingleton())
   {
      idx_ = i;
   }
}

void DagFunExtEquationFinder::apply(const DagFunCond *f)
{
   // nothing to do
}

void DagFunExtEquationFinder::apply(const DagFunPiecewise *f)
{
   // nothing to do
}

bool DagFunExtEquationFinder::isAnEquation() const
{
   return idx_ >= 0;
}

int DagFunExtEquationFinder::index() const
{
   THROW_IF(!isAnEquation(), "Bad access, not an equation");
   return idx_;
}

} // namespace realpaver
