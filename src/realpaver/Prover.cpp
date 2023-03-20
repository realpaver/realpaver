///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Prover.hpp"

namespace realpaver {

Prover::Prover(const Problem& p)
      : newton_(nullptr),
        ctr_(),
        canprove_(true),
        delta_(Param::GetDblParam("INFLATION_DELTA")),
        chi_(Param::GetDblParam("INFLATION_CHI")),
        maxiter_(Param::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT")),
        dtol_(Param::GetTolParam("NEWTON_CERTIFY_DTOL"))
{
   IntervalFunctionVector F;

   for (size_t i=0; i<p.nbCtrs(); ++i)
   {
      Constraint c = p.ctrAt(i);

      if (c.isEquation())
      {
         Constraint::SharedRep rep = c.rep();
         ArithCtrEq* eq = static_cast<ArithCtrEq*>(rep.get());
         Term t = eq->left() - eq->right();

         F.addFun(IntervalFunction(t));
      }
      else
         ctr_.push_back(c);
   }

   if (F.nbFuns() > 0)
   {
      if (F.isSquare() && F.nbVars() > 1)
      {
         newton_ = new IntervalNewton(F);
         newton_->setInflationDelta(delta_);
         newton_->setInflationChi(chi_);
         newton_->setCertifyMaxIter(maxiter_);
         newton_->setCertifyDTol(dtol_);
      }
   }

   else
      canprove_ = false;
}

Prover::~Prover()
{
   if (newton_ != nullptr)
      delete newton_;
}

Proof Prover::certify(IntervalRegion& reg)
{
   if (!canprove_) return Proof::Maybe;

   Proof proof = Proof::Inner;

   for (auto& c : ctr_)
   {
      Proof certif = c.isSatisfied(reg);
      proof = std::min(proof, certif);
   }

   if (newton_ != nullptr && proof != Proof::Empty)
   {
      proof = newton_->certify(reg);
   }

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
   
   if (newton_ != nullptr)
      newton_->setInflationDelta(val);
}

double Prover::getInflationChi() const
{
   return chi_;
}

void Prover::setInflationChi(const double& val)
{
   ASSERT(val > 0.0, "Bad parameter chi of inflation: " << val);
   chi_ = val;

   if (newton_ != nullptr)
      newton_->setInflationChi(val);
}

void Prover::setMaxIter(size_t n)
{
   maxiter_ = n;

   if (newton_ != nullptr)
      newton_->setCertifyMaxIter(n);
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

   if (newton_ != nullptr)
      newton_->setCertifyDTol(tol);
}

} // namespace
