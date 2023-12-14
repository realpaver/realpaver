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
      : v_(),
        mnewton_(nullptr),
        delta_(Param::GetDblParam("INFLATION_DELTA")),
        chi_(Param::GetDblParam("INFLATION_CHI")),
        maxiter_(Param::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT")),
        dtol_(Param::GetTolParam("NEWTON_CERTIFY_DTOL"))
{
   IntervalFunctionVector F;

   for (size_t i=0; i<p.nbCtrs(); ++i)
   {
      Constraint c = p.ctrAt(i);
      bool eq = c.isEquation();

      Item it = {c, eq, Proof::Maybe};
      v_.push_back(it);

      if (eq)
      {
         Constraint::SharedRep rep = c.rep();
         ArithCtrEq* eq = static_cast<ArithCtrEq*>(rep.get());
         Term t = eq->left() - eq->right();
         F.addFun(IntervalFunction(t));
      }
   }

   if (F.nbFuns() > 0 && F.isSquare())
   {
      if (F.nbVars() == 1)
      {
         // TODO
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

Prover::~Prover()
{
   if (mnewton_ != nullptr)
      delete mnewton_;
}

Proof Prover::certify(IntervalBox& box)
{
   bool inner = true;     // Iner certificate for the problem
   bool innerbis = true;  // Inner certificate for the problem but the equations

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
      return Proof::Inner;

   if (!innerbis)
      return Proof::Maybe;

   // now only the equations are not certainly satisfied
   Proof proof = Proof::Maybe;

   // applies the multivariate Newton operator for systems
   if (mnewton_ != nullptr)
      proof = mnewton_->certify(box);

   // applies the univariate Newton operator for one equation
   // TODO

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
}

void Prover::setMaxIter(size_t n)
{
   maxiter_ = n;

   if (mnewton_ != nullptr)
      mnewton_->setCertifyMaxIter(n);
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
}

} // namespace
