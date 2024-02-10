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
#include "realpaver/Logger.hpp"
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
        tol_(Param::GetDblParam("NEWTON_REL_TOL"), 0.0),
        delta_(Param::GetDblParam("INFLATION_DELTA")),
        chi_(Param::GetDblParam("INFLATION_CHI")),
        cmaxiter_(Param::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT"))
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
        tol_(N.tol_),
        delta_(N.delta_),
        chi_(N.chi_),
        cmaxiter_(N.cmaxiter_)
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

Tolerance IntervalNewton::getTol() const
{
   return tol_;
}

void IntervalNewton::setTol(const Tolerance& tol)
{
   tol_ = tol;
}

IntervalGaussSeidel* IntervalNewton::getGaussSeidel() const
{
   return gs_;
}

Proof IntervalNewton::contract(IntervalBox& X)
{
   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   LOG_INTER("Interval Newton contractor on " << X);

   do
   {
      ++ nb_steps;

      F_.evalDiff(X, val_, jac_);

      if (!val_.containsZero())
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on the evaluation test -> 0 not in F(X)");
         continue;
      }

      X.midpointOnScope(scope(), c_);
      F_.pointEval(c_, val_);

      if (val_.isEmpty())
      {
         iter = false;
         LOG_INTER("Stops on the midpoint evaluation -> empty");
         continue;
      }

      makeY(X);      // y := X - c
      b_ = -val_;    // b := -F(c)

      Proof certif = gs_->contractPrecond(jac_, y_, b_);
            
      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on Gauss-Seidel -> empty");
         continue;
      }

      // X := X inter (y + c)
      bool improved;
      certif = reduceX(X, improved);

      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on the intersection with the previous box -> empty");
         continue;
      }

      if (certif == Proof::Feasible)
         proof = Proof::Feasible;

      // checks the stopping criteria
      if (nb_steps > maxiter_)
      {
         iter = false;
         LOG_INTER("Stops on a maximum number of iterations: " << maxiter_);
      }

      else if (!improved)
      {
         iter = false;
         LOG_INTER("Stops on the tolerance " << tol_);
      }
      
      LOG_LOW("Inner step of interval Newton  -> " << X);
   }
   while (iter);

   LOG_INTER("End of interval Newton -> " << proof);
   LOG_INTER("Reduced box -> " << X);

   return proof;
}

void IntervalNewton::makeY(IntervalBox& X)
{
   // y := X - c
   int i = 0;
   for (const auto& v : scope())
   {
      y_.set(i, X.get(v) - c_.get(v));
      i = i+1;
   }
}

Proof IntervalNewton::reduceX(IntervalBox& X, bool& improved)
{
   int i = 0;
   Proof proof = Proof::Feasible;

   improved = false;

   for (const auto& v : scope())
   {
      Interval dom = X.get(v),
               z = y_.get(i) + c_.get(v);

      if (dom.isDisjoint(z))
         return Proof::Empty;

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

      Interval reduced = dom & z;

      if (tol_.isImproved(dom, reduced))
         improved = true;

      X.set(v, reduced);
      i = i+1;
   }
   return proof;
}

Proof IntervalNewton::certify(IntervalBox& box)
{
   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   IntervalBox X(scope());
   X.setOnScope(box, scope());

   // given X(k-2), X(k-1), Xk three boxes from the sequence calculated
   // by this method, dprev is the distance between X(k-2) and X(k-1), dcurr
   // is the distance between X(k-1) and Xk ; the method diverges if we have
   // dcurr > dprev
   double dprev = Double::inf(), dcurr;

   LOG_INTER("Interval Newton certification on " << X);

   do
   {
      ++ nb_steps;

      IntervalBox prev(X);

      // inflation
      X.inflate(delta_, chi_);

      LOG_LOW("Inflated box -> " << X);

      F_.evalDiff(X, val_, jac_);

      if (!val_.containsZero())
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on the evaluation test -> 0 not in F(X)");
         continue;
      }

      X.midpointOnScope(scope(), c_);
      F_.pointEval(c_, val_);

      if (val_.isEmpty())
      {
         iter = false;
         LOG_INTER("Stops on the midpoint evaluation -> empty");
         continue;
      }

      makeY(X);      // y := X - c
      b_ = -val_;    // b := -F(c)

      Proof certif = gs_->contractPrecond(jac_, y_, b_);
            
      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on Gauss-Seidel -> empty");
         continue;
      }

      // X := y + c
      certif = certifyX(X);
      dcurr = X.distance(prev);

      if (certif == Proof::Feasible)
      {
         proof = Proof::Feasible;
         iter = false;
         LOG_INTER("Feasibility proved");
      }
      else if (dcurr > dprev)
      {
         iter = false;
         LOG_INTER("Stops since the method diverges");
      }
      else if (nb_steps > cmaxiter_)
      {
         iter = false;
         LOG_INTER("Stops on a maximum number of iterations: " << maxiter_);
      }

      LOG_LOW("Inner step of interval Newton certification  -> " << X);
   }
   while (iter);

   if (proof == Proof::Feasible)
      box.setOnScope(X, scope());

   LOG_INTER("End of interval Newton cerfification -> " << proof);
   LOG_INTER("New box -> " << box);

   return proof;
}

Proof IntervalNewton::certifyX(IntervalBox& X)
{
   // X := y + c
   int i = 0;
   Proof proof = Proof::Feasible;

   for (const auto& v : scope())
   {
      Interval dom = X.get(v),
               z = y_.get(i) + c_.get(v);

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

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

void IntervalNewton::print(std::ostream& os) const
{
   os << "Interval Newton";
}

} // namespace
