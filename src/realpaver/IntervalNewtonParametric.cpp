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
 * @file   IntervalNewtonParametric.cpp
 * @brief  Parametric interval Newton method
 * @author Laurent Granvilliers
 * @date   13 May 2026
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/IntervalNewtonParametric.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalNewtonParametric::IntervalNewtonParametric(IntervalFunctionVector F)
    : F_(F)
    , gs_(nullptr)
    , delta_(Params::GetDblParam("INFLATION_DELTA"))
    , chi_(Params::GetDblParam("INFLATION_CHI"))
    , cmaxiter_(Params::GetIntParam("NEWTON_CERTIFY_ITER_LIMIT"))
{

   REALPAVER_DEBUG("nv : " << F.nbVars() << "  nf: " << F.nbFuns());

   THROW_IF(F.nbFuns() >= F.nbVars(), "Bad system");

   gs_ = std::make_unique<IntervalGaussSeidel>();
}

bool IntervalNewtonParametric::makeScopes(IntervalBox &B, Scope &mainScop,
                                          Scope &paramScop)
{
   IntervalMatrix J(F_.nbFuns(), F_.nbVars());
   F_.diff(B, J);

   REALPAVER_DEBUG("---- Jacobian ----\n" << J);

   IntervalMatrix::IndexList ind(F_.nbVars());
   bool ok = J.selectMostSensitive(ind);

   if (ok)
   {
      const Scope &scop = F_.scope();

      for (size_t i = 0; i < F_.nbFuns(); ++i)
      {
         mainScop.insert(scop.var(ind[i]));
      }

      for (size_t i = F_.nbFuns(); i < F_.nbVars(); ++i)
      {
         paramScop.insert(scop.var(ind[i]));
      }
   }
   return ok;
}

Proof IntervalNewtonParametric::certify(IntervalBox &B)
{
   const Scope &fullScop = F_.scope();

   Scope mainScop, paramScop;
   bool ok = makeScopes(B, mainScop, paramScop);

   if (!ok)
      return Proof::Maybe;

   REALPAVER_DEBUG("main scope : " << mainScop);
   REALPAVER_DEBUG("param scope : " << paramScop);

   size_t n = mainScop.size();

   bool iter = true;
   Proof proof = Proof::Maybe;
   size_t nb_steps = 0;

   IntervalBox X(B);       // inflated box
   IntervalBox c(B);       // midpoint
   IntervalMatrix H(n, n); // Hansen's matrix

   // given X(k-2), X(k-1), Xk three boxes from the sequence calculated
   // by this method, dprev is the distance between X(k-2) and X(k-1), dcurr
   // is the distance between X(k-1) and Xk ; the method diverges if we have
   // dcurr > dprev
   double dprev = Double::inf(), dcurr;

   LOG_INTER("Interval Newton certification on " << X);

   REALPAVER_DEBUG("X : " << X);

   do
   {
      ++nb_steps;

      IntervalBox prev(X);

      // inflation on the main scope
      X.inflateOnScope(mainScop, delta_, chi_);

      REALPAVER_DEBUG("inflate X : " << X);

      LOG_LOW("Inflated box -> " << X);

      IntervalVector fmid(n);
      makeC(X, mainScop, c); // c := midpoint of X on the main scope
      F_.eval(c, fmid);

      REALPAVER_DEBUG("mid : " << c);
      REALPAVER_DEBUG("fmid : " << fmid);

      if (fmid.isEmpty())
      {
         iter = false;
         LOG_INTER("Stops on the midpoint evaluation -> empty");
         continue;
      }

      IntervalVector y(n);
      makeY(X, c, mainScop, y); // y := X - c on the main scope

      REALPAVER_DEBUG("y : " << y);

      IntervalVector b = -fmid;

      REALPAVER_DEBUG("b : " << b);

      // calculates the Hansen's matrix
      F_.diffHansen(X, c, mainScop, H);

      REALPAVER_DEBUG("H\n" << H);

      Proof certif = gs_->certifyPrecond(H, y, b);

      REALPAVER_DEBUG("certif : " << certif);

      if (certif == Proof::Empty)
      {
         proof = Proof::Empty;
         iter = false;
         LOG_INTER("Stops on Gauss-Seidel -> empty");
         continue;
      }

      // X := y + c
      certif = certifyX(c, y, mainScop, X);
      dcurr = X.distanceOnScope(prev, mainScop);

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
         LOG_INTER("Stops on a maximum number of iterations: " << cmaxiter_);
      }

      LOG_LOW("Inner step of interval Newton certification  -> " << X);

   } while (iter);

   if (proof == Proof::Feasible)
      B.setOnScope(X, fullScop);

   LOG_INTER("End of interval Newton cerfification -> " << proof);
   LOG_INTER("New box -> " << B);

   return proof;
}

void IntervalNewtonParametric::makeC(const IntervalBox &X, const Scope &mainScop,
                                     IntervalBox &c)
{
   for (const auto &v : mainScop)
      c.set(v, X.get(v).midpoint());
}

void IntervalNewtonParametric::makeY(const IntervalBox &X, const IntervalBox &c,
                                     const Scope &mainScop, IntervalVector &y)
{
   // y := X - c
   int i = 0;
   for (const auto &v : mainScop)
   {
      y.set(i, X.get(v) - c.get(v));
      i = i + 1;
   }
}

Proof IntervalNewtonParametric::certifyX(const IntervalBox &c, const IntervalVector &y,
                                         const Scope &mainScope, IntervalBox &X)
{
   // X := y + c
   int i = 0;
   Proof proof = Proof::Feasible;

   for (const auto &v : mainScope)
   {
      Interval dom = X.get(v), z = y.get(i) + c.get(v);

      if (!dom.strictlyContains(z))
         proof = Proof::Maybe;

      X.set(v, z);
      i = i + 1;
   }
   return proof;
}

double IntervalNewtonParametric::getInflationDelta() const
{
   return delta_;
}

void IntervalNewtonParametric::setInflationDelta(const double &val)
{
   REALPAVER_ASSERT(val > 1.0, "Bad parameter delta of inflation: " << val);
   delta_ = val;
}

double IntervalNewtonParametric::getInflationChi() const
{
   return chi_;
}

void IntervalNewtonParametric::setInflationChi(const double &val)
{
   REALPAVER_ASSERT(val > 0.0, "Bad parameter chi of inflation: " << val);
   chi_ = val;
}

void IntervalNewtonParametric::setCertifyMaxIter(size_t n)
{
   cmaxiter_ = n;
}

size_t IntervalNewtonParametric::getCertifyMaxIter() const
{
   return cmaxiter_;
}

} // namespace realpaver
