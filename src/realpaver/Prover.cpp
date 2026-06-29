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
 * @file   Prover.cpp
 * @brief  Certification method
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "Common.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/ConstraintRep.hpp"
#include "realpaver/IntervalFunctionUni.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/IntervalNewtonParametric.hpp"
#include "realpaver/IntervalNewtonUni.hpp"
#include "realpaver/IntervalThickFunction.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Prover.hpp"

namespace realpaver {

Prover::Prover(SharedDagExtended edag)
    : edag_(edag)
    , initBox_(nullptr)
    , delta_(Params::GetDblParam("INFLATION_DELTA"))
    , chi_(Params::GetDblParam("INFLATION_CHI"))
    , maxiter_(Params::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT"))
    , callNumber_(0)
    , successNumber_(0)
{
}

Prover::~Prover()
{
   LOG_MAIN("Number of prover calls: " << callNumber_);
   LOG_MAIN("Number of prover success: " << successNumber_);
}

void Prover::setInitBox(const IntervalBox &B)
{
   initBox_ = std::make_unique<IntervalBox>(B);
}

Proof Prover::certify(IntervalBox &B)
{
   Bitset bt;
   return certify(B, bt);
}

Proof Prover::certify(IntervalBox &B, const Bitset &active)
{
   callNumber_++;
   // all the constraints inactive ?
   if (active.size() > 0 && active.areAllZeros())
   {
      successNumber_++;
      return Proof::Inner;
   }

   // checks the constraints in the DAG
   IntervalFunctionVector Fe; // vector of equations
   IntervalFunctionVector Fi; // vector of inequalities

   for (size_t i = 0; i < edag_->size(); ++i)
   {
      Constraint c = edag_->fun(i)->ctr();

      // only the initial constraints are considered
      if (c.status() != ConstraintStatus::Initial)
         continue;

      // only the active constraints are considered
      if (active.size() > 0 && active.get(c.id()) == 0)
         continue;

      SharedDagFun f = edag_->getActiveFun(i, B, Bitset());

      if (f == nullptr)
      {
         // f is e.g. a conditional constraint and the guard is not verified
         return Proof::Maybe;
      }
      else if (f->isAnEquation())
      {
         Fe.addFun(IntervalFunction(f));
      }
      else
      {
         Fi.addFun(IntervalFunction(f));
      }
   }

   // handles the equations using an interval Newton operator
   int nve = Fe.nbVars(), nfe = Fe.nbFuns();
   if (nve > 0)
   {
      Proof proof = Proof::Maybe;
      if (nve == nfe)
      {
         // Newton for square systems
         if (nfe == 1)
         {
            // univariate Newton
            Variable v = Fe.fun(0).scope().var(0);
            Interval x = B.get(v);
            IntervalFunctionUniv g(Fe.fun(0));
            IntervalNewtonUni nwt;
            nwt.setMaxIter(maxiter_);
            nwt.getInflator().setChi(chi_);
            nwt.getInflator().setDelta(delta_);
            proof = nwt.localSearch(g, x);
            B.set(v, x);
         }
         else
         {
            // multivariate Newton
            IntervalNewton nwt(Fe);
            nwt.setInflationChi(chi_);
            nwt.setInflationDelta(delta_);
            nwt.setCertifyMaxIter(maxiter_);
            proof = nwt.certify(B);
         }
      }
      else
      {
         // parametric Newton
         IntervalNewtonParametric nwt(Fe);
         nwt.setInflationChi(chi_);
         nwt.setInflationDelta(delta_);
         nwt.setCertifyMaxIter(maxiter_);
         proof = nwt.certify(B);
      }

      if (proof != Proof::Feasible)
         return proof;
   }

   // handles the inequalities
   int nfi = Fi.nbFuns();
   for (size_t i = 0; i < nfi; ++i)
   {
      IntervalFunction f = Fi.fun(i);
      Interval e = f.eval(B);
      Interval img = f.getImage();

      if (!img.contains(e))
         return Proof::Maybe;
   }

   // checks the constraints not in the DAG
   for (size_t i = 0; i < edag_->nbOutsideCtrs(); ++i)
   {
      ConstraintProver prover(B);
      Constraint c = edag_->outsideCtr(i);

      if (active.size() > 0 && active.get(c.id()))
      {
         c.acceptVisitor(prover);
         if (!prover.isFeasible())
            return Proof::Maybe;
      }
   }

   Proof proof = (nfe == 0) ? Proof::Inner : Proof::Feasible;

   if (initBox_ == nullptr)
   {
      if (proof == Proof::Feasible || proof == Proof::Inner)
         successNumber_++;
      return proof;
   }
   else
   {
      if (initBox_->contains(B) && (proof == Proof::Feasible || proof == Proof::Inner))
         successNumber_++;
      return initBox_->contains(B) ? proof : Proof::Maybe;
   }
}

double Prover::getInflationDelta() const
{
   return delta_;
}

void Prover::setInflationDelta(const double &val)
{
   REALPAVER_ASSERT(val > 1.0, "Bad parameter delta of inflation: " << val);
   delta_ = val;
}

double Prover::getInflationChi() const
{
   return chi_;
}

void Prover::setInflationChi(const double &val)
{
   REALPAVER_ASSERT(val > 0.0, "Bad parameter chi of inflation: " << val);
   chi_ = val;
}

void Prover::setMaxIter(size_t n)
{
   maxiter_ = n;
}

size_t Prover::getMaxIter() const
{
   return maxiter_;
}

bool Prover::checkSatisfaction(const IntervalBox &B) const
{
   for (size_t i = 0; i < edag_->size(); ++i)
   {
      Constraint c = edag_->fun(i)->ctr();

      if (c.status() != ConstraintStatus::Initial)
         continue;

      double v = c.violation(B);
      if (v > 0.0)
         return false;
   }
   return true;
}

} // namespace realpaver
