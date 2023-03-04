///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalNewton.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalNewton::IntervalNewton(IntervalFunctionVector F)
      : Contractor(),
        F_(F),
        jac_(F.nbFuns(), F.nbVars()),
        val_(F.nbFuns()),
        y_(F.nbVars()),
        b_(F.nbFuns()),
        c_(F.scope()),
        gs_(nullptr),
        maxiter_(Param::GetIntParam("NEWTON_ITER_LIMIT")),
        xtol_(Param::GetTolParam("NEWTON_XTOL")),
        dtol_(Param::GetTolParam("NEWTON_DTOL")),
        delta_(Param::GetDblParam("INFLATION_DELTA")),
        chi_(Param::GetDblParam("INFLATION_CHI")),
        cmaxiter_(Param::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT")),
        cdtol_(Param::GetTolParam("NEWTON_CERTIFY_DTOL"))
{
   ASSERT(F.nbVars() == F.nbFuns(),
          "Interval Newton defined with a non-square system");

   gs_ = new IntervalGaussSeidel();
}

IntervalNewton::IntervalNewton(const IntervalNewton& N)
      : Contractor(N),
        F_(N.F_),
        jac_(N.jac_),
        val_(N.val_),
        y_(N.y_),
        b_(N.b_),
        c_(N.c_),
        gs_(nullptr),
        maxiter_(N.maxiter_),
        xtol_(N.xtol_),
        dtol_(N.dtol_)
{
   gs_ = new IntervalGaussSeidel(*N.gs_);
}

IntervalNewton::~IntervalNewton()
{
   delete gs_;
}

Scope IntervalNewton::scope() const
{
   return F_.scope();
}

void IntervalNewton::setMaxIter(size_t n)
{
   maxiter_ = n;
}

size_t IntervalNewton::getMaxIter() const
{
   return maxiter_;
}

Tolerance IntervalNewton::getXTol() const
{
   return xtol_;
}

void IntervalNewton::setXTol(const Tolerance& tol)
{
   xtol_ = tol;
}

Tolerance IntervalNewton::getDTol() const
{
   return dtol_;
}

void IntervalNewton::setDTol(const Tolerance& tol)
{
   dtol_ = tol;
}

IntervalGaussSeidel* IntervalNewton::getGaussSeidel() const
{
   return gs_;
}

Proof IntervalNewton::contract(IntervalRegion& X)
{
   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   do
   {
      ++ nb_steps;

      F_.evalDiff(X, val_, jac_);

      if (!val_.containsZero())
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      X.midpointOnScope(scope(), c_);
      F_.pointEval(c_, val_);

      if (val_.isEmpty())
      {
         iter = false;
         continue;
      }

      makeY(X);      // y := X - c
      b_ = -val_;    // b := -F(c)

      Proof certif = gs_->contractPrecond(jac_, y_, b_);
            
      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      // X := X inter (y + c)
      bool hasxtol, hasdtol;
      certif = reduceX(X, hasxtol, hasdtol);

      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      if (certif == Proof::Feasible)
         proof = Proof::Feasible;

      // checks the stopping criteria
      if (nb_steps > maxiter_ || hasxtol || hasdtol)
         iter = false;
   }
   while (iter);

   return proof;
}

void IntervalNewton::makeY(IntervalRegion& X)
{
   // y := X - c
   int i = 0;
   for (const auto& v : scope())
   {
      y_.set(i, X.get(v) - c_.get(v));
      i = i+1;
   }
}

Proof IntervalNewton::reduceX(IntervalRegion& X, bool& hasxtol, bool& hasdtol)
{
   int i = 0;
   Proof proof = Proof::Feasible;

   hasxtol = true;
   hasdtol = true;

   for (const auto& v : scope())
   {
      Interval dom = X.get(v),
               z = y_.get(i) + c_.get(v);

      if (dom.isDisjoint(z))
         return Proof::Empty;

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

      Interval reduced = dom & z;

      if (!dtol_.haveDistTolerance(reduced, dom))
         hasdtol = false;

      if (!xtol_.hasTolerance(reduced))
         hasxtol = false;

      X.set(v, reduced);
      i = i+1;
   }
   return proof;
}

Proof IntervalNewton::certify(IntervalRegion& reg)
{
   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   IntervalRegion X(scope());
   X.setOnScope(reg, scope());

   do
   {
      ++ nb_steps;

      // inflation
      X.inflate(delta_, chi_);

      F_.evalDiff(X, val_, jac_);

      if (!val_.containsZero())
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      X.midpointOnScope(scope(), c_);
      F_.pointEval(c_, val_);

      if (val_.isEmpty())
      {
         iter = false;
         continue;
      }

      makeY(X);      // y := X - c
      b_ = -val_;    // b := -F(c)

      Proof certif = gs_->contractPrecond(jac_, y_, b_);
            
      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         continue;
      }

      // X := y + c
      bool hasdtol;
      certif = certifyX(X, hasdtol);

      if (certif == Proof::Feasible)
      {
         proof = Proof::Feasible;
         iter = false;
         continue;
      }

      // checks the stopping criteria
      if (nb_steps > cmaxiter_ || (!hasdtol))
         iter = false;

   }
   while (iter);

   if (proof == Proof::Feasible)
      reg.setOnScope(X, scope());

   return proof;
}

Proof IntervalNewton::certifyX(IntervalRegion& X, bool& hasdtol)
{
   // X := y + c
   int i = 0;
   Proof proof = Proof::Feasible;
   hasdtol = true;

   for (const auto& v : scope())
   {
      Interval dom = X.get(v),
               z = y_.get(i) + c_.get(v);

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

      if (!dtol_.haveDistTolerance(z, dom))
         hasdtol = false;

      X.set(v, z);
      i = i+1;
   }
   return proof;
}

double IntervalNewton::getInflationDelta() const
{
   return delta_;
}

void IntervalNewton::setInflationDelta(const double& val)
{
   ASSERT(val > 1.0, "Bad parameter delta of inflation: " << val);
   delta_ = val;
}

double IntervalNewton::getInflationChi() const
{
   return chi_;
}

void IntervalNewton::setInflationChi(const double& val)
{
   ASSERT(val > 0.0, "Bad parameter chi of inflation: " << val);
   chi_ = val;
}

void IntervalNewton::setCertifyMaxIter(size_t n)
{
   cmaxiter_ = n;
}

size_t IntervalNewton::getCertifyMaxIter() const
{
   return cmaxiter_;
}

Tolerance IntervalNewton::getCertifyDTol() const
{
   return cdtol_;
}

void IntervalNewton::setCertifyDTol(const Tolerance& tol)
{
   cdtol_ = tol;
}

void IntervalNewton::print(std::ostream& os) const
{
   os << "Interval Newton";
}

} // namespace
