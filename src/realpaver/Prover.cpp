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
 * @brief  Prover based on interval Newton
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintProver.hpp"
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
    , delta_(Params::GetDblParam("INFLATION_DELTA"))
    , chi_(Params::GetDblParam("INFLATION_CHI"))
    , maxiter_(Params::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT"))
{
}

Proof Prover::certify(IntervalBox &B)
{
   // checks the constraints not in the DAG
   for (size_t i = 0; i < edag_->nbOutsideCtrs(); ++i)
   {
      ConstraintProver prover(B);
      edag_->outsideCtr(i).acceptVisitor(prover);
      if (!prover.isFeasible())
         return Proof::Maybe;
   }

   // checks the constraints in the DAG
   IntervalFunctionVector F;   // vector of equations
   Proof proof = Proof::Inner; // for inequalities

   for (size_t i = 0; i < edag_->size(); ++i)
   {
      SharedDagFun f = edag_->getActiveFun(i, B, Bitset());
      if (f == nullptr)
      {
         // f is e.g. a conditional constraint and the guard is not verified
         return Proof::Maybe;
      }
      else if (f->isAnEquation())
      {
         F.addFun(IntervalFunction(f));
      }
      else
      {
         Interval e = f->flatFunction()->iEval(B);
         Interval img = f->getImage();
         if (e.isDisjoint(img))
         {
            proof = Proof::Empty;
         }
         else if (!img.contains(e))
         {
            // handles f as an equation
            proof = Proof::Maybe;
            F.addFun(IntervalFunction(f));
         }
      }
   }

   // an inequality is violated?
   if (proof == Proof::Empty)
      return proof;

   int nv = F.nbVars(), nf = F.nbFuns();

   if (nv == 0)
   {
      // no equation
      return Proof::Inner;
   }
   else if (nv == nf)
   {
      // Newton for square systems
      if (nf == 1)
      {
         Variable v = F.fun(0).scope().var(0);
         Interval x = B.get(v);
         IntervalFunctionUniv g(F.fun(0));
         IntervalNewtonUni nwt;
         nwt.setMaxIter(maxiter_);
         nwt.getInflator().setChi(chi_);
         nwt.getInflator().setDelta(delta_);
         proof = nwt.localSearch(g, x);
         B.set(v, x);
         return proof;
      }
      else
      {
         IntervalNewton nwt(F);
         nwt.setInflationChi(chi_);
         nwt.setInflationDelta(delta_);
         nwt.setMaxIter(maxiter_);
         return nwt.certify(B);
      }
   }
   else
   {
      IntervalNewtonParametric nwt(F);
      nwt.setInflationChi(chi_);
      nwt.setInflationDelta(delta_);
      return nwt.certify(B);
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

} // namespace realpaver
