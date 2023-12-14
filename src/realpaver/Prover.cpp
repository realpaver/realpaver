///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Prover.hpp"
#include "realpaver/ThickFunction.hpp"

namespace realpaver {

Prover::Prover(const Problem& p)
      : v_(),
        dag_(nullptr),
        mnewton_(nullptr),
        unewton_(nullptr),
        delta_(Param::GetDblParam("INFLATION_DELTA")),
        chi_(Param::GetDblParam("INFLATION_CHI")),
        maxiter_(Param::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT")),
        dtol_(Param::GetTolParam("NEWTON_CERTIFY_DTOL"))
{
   dag_ = std::make_shared<Dag>();

   for (size_t i=0; i<p.nbCtrs(); ++i)
   {
      Constraint c = p.ctrAt(i);
      bool eq = c.isEquation();

      Item it = {c, eq, Proof::Maybe};
      v_.push_back(it);

      if (eq)
         dag_->insert(c);
   }

   if (dag_->nbFuns() > 0)
   {
      IntervalFunctionVector F(dag_);

      if (F.isSquare())
      {
         if (F.nbVars() == 1)
         {
            unewton_ = new UniIntervalNewton();
            unewton_->setMaxIter(maxiter_);
            unewton_->setLocalDTol(dtol_);
            unewton_->getInflator().setDelta(delta_);
            unewton_->getInflator().setChi(chi_);
         }
         else
         {
            mnewton_ = new IntervalNewton(F);
            mnewton_->setInflationDelta(delta_);
            mnewton_->setInflationChi(chi_);
            mnewton_->setCertifyMaxIter(maxiter_);
            mnewton_->setCertifyDTol(dtol_);
         }
      }
   }
}

Prover::~Prover()
{
   if (mnewton_ != nullptr) delete mnewton_;
   if (unewton_ != nullptr) delete unewton_;
}

Proof Prover::certify(IntervalBox& box)
{
   bool inner = true;     // Iner certificate for the problem
   bool innerbis = true;  // Inner certificate for the problem but the equations

   LOG_INTER("Certification of the box " << box);

   for (auto& it : v_)
   {
      it.proof = it.ctr.isSatisfied(box);

      if (it.proof == Proof::Empty)
         return Proof::Empty;

      if (it.proof != Proof::Inner)
      {
         inner = false;
         
         if (!it.iseq)
            innerbis = false;
      }
   }

   if (inner)
   {
      LOG_INTER(" -> inner box");
      return Proof::Inner;
   }

   if (!innerbis)
   {
      LOG_INTER(" -> no proof since one consraint is not validated");
      return Proof::Maybe;
   }

   // now only the equations are not certainly satisfied
   Proof proof = Proof::Maybe;

   // applies the multivariate Newton operator for systems
   if (mnewton_ != nullptr)
   {
      LOG_INTER("Certification by the multivariate interval Newton");

      proof = mnewton_->certify(box);

      LOG_INTER(" -> " << box);
   }

   // applies the univariate Newton operator for one equation
   if (unewton_ != nullptr)
   {
      LOG_INTER("Certification by the univariate interval Newton");

      Variable v = dag_->fun(0)->scope().var(0);
      Interval x = box.get(v);
      ThickFunction f(dag_, 0, v);
      proof = unewton_->localSearch(f, x);
      box.set(v, x);

      LOG_INTER(" -> " << box);
   }

   LOG_INTER(" -> " << proof);
   return proof;
}

double Prover::getInflationDelta() const
{
   return delta_;
}

void Prover::setInflationDelta(const double& val)
{
   ASSERT(val > 1.0, "Bad parameter delta of inflation: " << val);
   delta_ = val;

   if (mnewton_ != nullptr)
      mnewton_->setInflationDelta(val);

   if (unewton_ != nullptr)
      unewton_->getInflator().setDelta(val);
}

double Prover::getInflationChi() const
{
   return chi_;
}

void Prover::setInflationChi(const double& val)
{
   ASSERT(val > 0.0, "Bad parameter chi of inflation: " << val);
   chi_ = val;

   if (mnewton_ != nullptr)
      mnewton_->setInflationChi(val);

   if (unewton_ != nullptr)
      unewton_->getInflator().setChi(val);
}

void Prover::setMaxIter(size_t n)
{
   maxiter_ = n;

   if (mnewton_ != nullptr)
      mnewton_->setCertifyMaxIter(n);

   if (unewton_ != nullptr)
      unewton_->setMaxIter(n);
}

size_t Prover::getMaxIter() const
{
   return maxiter_;
}

Tolerance Prover::getDTol() const
{
   return dtol_;
}

void Prover::setDTol(const Tolerance& tol)
{
   dtol_ = tol;

   if (mnewton_ != nullptr)
      mnewton_->setCertifyDTol(tol);

   if (unewton_ != nullptr)
      unewton_->setLocalDTol(tol);
}

} // namespace
