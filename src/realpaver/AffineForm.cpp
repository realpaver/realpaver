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
 * @file   AffineForm.cpp
 * @brief  Affine arithmetic
 * @author Laurent Granvilliers
 * @date   10 Mar 2025
 */

#include "realpaver/AffineForm.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Double.hpp"

namespace realpaver {

bool AffineForm::MINRANGE = true;

AffineForm::AffineForm()
    : c_(0.0)
    , l_()
    , e_(0.0)
{
}

AffineForm::AffineForm(const Interval &x)
    : c_(x)
    , l_()
    , e_(0.0)
{
   THROW_IF(x.isEmpty() || x.isInf(), "Affine form invalid");
}

AffineForm::AffineForm(const double &a)
    : c_(a)
    , l_()
    , e_(0.0)
{
   THROW_IF(Double::isNan(a) || Double::isInf(a), "Affine form invalid");
}

AffineForm::AffineForm(int var, const Interval &domain)
    : c_(0.0)
    , l_()
    , e_(0.0)
{
   THROW_IF(domain.isEmpty() || domain.isInf(), "Affine form invalid");
   std::pair<double, double> p = domain.midrad();
   c_ = Interval(p.first);
   pushBack(Interval(p.second), var);
}

AffineForm::AffineForm(const double &a0, const std::initializer_list<double> &A,
                       const std::initializer_list<int> &V, const Interval &e)
    : c_(a0)
    , l_()
    , e_(e)
{
   REALPAVER_ASSERT(A.size() == V.size(), "Bad initialization of an affine form");
   THROW_IF(Double::isNan(a0) || Double::isInf(a0), "Affine form invalid");
   THROW_IF(e.isEmpty() || e.isInf(), "Affine form invalid");

   auto ita = A.begin();
   auto itv = V.begin();

   while (ita != A.end())
   {
      THROW_IF(Double::isNan(*ita) || Double::isInf(*ita), "Affine form invalid");
      pushBack(*ita, *itv);
      ++ita;
      ++itv;
   }
}

AffineForm::AffineForm(const Interval &a0, const std::initializer_list<Interval> &A,
                       const std::initializer_list<int> &V, const Interval &e)
    : c_(a0)
    , l_()
    , e_(e)
{
   REALPAVER_ASSERT(A.size() == V.size(), "Bad initialization of an affine form");
   THROW_IF(a0.isEmpty() || a0.isInf(), "Affine form invalid");
   THROW_IF(e.isEmpty() || e.isInf(), "Affine form invalid");

   auto ita = A.begin();
   auto itv = V.begin();

   while (ita != A.end())
   {
      THROW_IF(ita->isEmpty() || ita->isInf(), "Affine form invalid");
      pushBack(*ita, *itv);
      ++ita;
      ++itv;
   }
}

bool AffineForm::equals(const AffineForm &f) const
{
   if (c_.isSetNeq(f.c_))
      return false;
   if (e_.isSetNeq(f.e_))
      return false;
   if (l_.size() != f.l_.size())
      return false;

   auto it = cbegin(), jt = f.cbegin();
   while (it != cend())
   {
      if (it->itv.isSetNeq(jt->itv))
         return false;
      if (it->var != jt->var)
         return false;
      ++it;
      ++jt;
   }
   return true;
}

size_t AffineForm::nbLinearTerms() const
{
   return l_.size();
}

bool AffineForm::usesMinrange()
{
   return MINRANGE;
}

void AffineForm::useMinrange(bool b)
{
   MINRANGE = b;
}

void AffineForm::useChebyshev(bool b)
{
   MINRANGE = !b;
}

const Interval &AffineForm::constantTerm() const
{
   return c_;
}

const Interval &AffineForm::errorTerm() const
{
   return e_;
}

AffineForm operator+(const AffineForm &f, const AffineForm &g)
{
   AffineForm res;

   // constants
   res.setConstant(f.c_ + g.c_);

   // error terms
   res.setError(f.e_ + g.e_);

   // linear terms
   auto it = f.cbegin();
   auto jt = g.cbegin();
   while ((it != f.cend()) && (jt != g.cend()))
   {
      if (it->var < jt->var)
      {
         res.pushBack(it->itv, it->var);
         ++it;
      }
      else if (it->var > jt->var)
      {
         res.pushBack(jt->itv, jt->var);
         ++jt;
      }
      else
      {
         res.pushBack(it->itv + jt->itv, it->var);
         ++it;
         ++jt;
      }
   }
   while (it != f.cend())
   {
      res.pushBack(it->itv, it->var);
      ++it;
   }
   while (jt != g.cend())
   {
      res.pushBack(jt->itv, jt->var);
      ++jt;
   }
   return res;
}

AffineForm operator-(const AffineForm &f, const AffineForm &g)
{
   AffineForm res;

   // constants
   res.setConstant(f.c_ - g.c_);

   // error terms (sum of errors)
   res.setError(f.e_ + g.e_);

   // linear terms
   auto it = f.cbegin();
   auto jt = g.cbegin();
   while ((it != f.cend()) && (jt != g.cend()))
   {
      if (it->var < jt->var)
      {
         res.pushBack(it->itv, it->var);
         ++it;
      }
      else if (it->var > jt->var)
      {
         res.pushBack(-jt->itv, jt->var);
         ++jt;
      }
      else
      {
         res.pushBack(it->itv - jt->itv, it->var);
         ++it;
         ++jt;
      }
   }
   while (it != f.cend())
   {
      res.pushBack(it->itv, it->var);
      ++it;
   }
   while (jt != g.cend())
   {
      res.pushBack(-jt->itv, jt->var);
      ++jt;
   }
   return res;
}

AffineForm operator-(const AffineForm &f)
{
   AffineForm res;

   // constant term
   res.setConstant(-f.c_);

   // error term (the same)
   res.setError(f.e_);

   // linear terms
   auto it = f.cbegin();
   while (it != f.cend())
   {
      res.pushBack(-it->itv, it->var);
      ++it;
   }
   return res;
}

AffineForm operator*(const AffineForm &f, const AffineForm &g)
{
   AffineForm res;

   Interval A = abs(f.e_), B = abs(g.e_);

   // constant term
   res.setConstant(f.c_ * g.c_);

   // linear terms
   auto it = f.cbegin();
   auto jt = g.cbegin();

   while (it != f.cend() && jt != g.cend())
   {
      if (it->var < jt->var)
      {
         res.pushBack(g.c_ * it->itv, it->var);
         A += abs(it->itv);
         ++it;
      }
      else if (it->var > jt->var)
      {
         res.pushBack(f.c_ * jt->itv, jt->var);
         B += abs(jt->itv);
         ++jt;
      }
      else
      {
         res.pushBack(g.c_ * it->itv + f.c_ * jt->itv, it->var);
         A += abs(it->itv);
         B += abs(jt->itv);
         ++it;
         ++jt;
      }
   }
   while (it != f.cend())
   {
      res.pushBack(g.c_ * it->itv, it->var);
      A += abs(it->itv);
      ++it;
   }
   while (jt != g.cend())
   {
      res.pushBack(f.c_ * jt->itv, jt->var);
      B += abs(jt->itv);
      ++jt;
   }

   // error term
   res.setError(res.e_ + abs(f.c_) * g.e_ + abs(g.c_) * f.e_ + A * B);

   return res;
}

AffineForm sqr(const AffineForm &f)
{
   AffineForm res;

   // constant term
   res.setConstant(sqr(f.c_));

   Interval S = abs(f.e_);

   // linear terms
   Interval a2 = 2.0 * f.c_;
   auto it = f.cbegin();
   while (it != f.cend())
   {
      res.pushBack(a2 * it->itv, it->var);
      S += abs(it->itv);
      ++it;
   }

   // error term
   Interval T = 0.5 * sqr(S);
   res.setError(abs(a2) * f.e_ + T);

   // constant Term
   res.setConstant(res.c_ + T);

   return res;
}

void AffineForm::minrangeSqrt(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sqrt(a), fb = sqrt(b);
   alpha = 1.0 / (2.0 * fb);
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevSqrt(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sqrt(a), fb = sqrt(b);
   alpha = (fb - fa) / (b - a);
   Interval c = 0.25 / sqr(alpha), fc = sqrt(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm sqrt(const AffineForm &f)
{
   Interval x = f.eval();

   if (x.isEmpty() || x.isStrictlyNegative())
      THROW("Affine form of sqrt on " << x);

   if (x.right() == 0.0)
      return AffineForm();

   if (x.left() < 0.0)
      x.setLeft(0.0);
   if (x.isInf())
      THROW("Affine form of square root on an infinite interval");

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSqrt(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevSqrt(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeUdiv(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = 1.0 / a, fb = 1.0 / b;
   alpha = x.isPositive() ? (-1.0 / sqr(b)) : (-1.0 / sqr(a));
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevUdiv(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), ab = a * b, fa = 1.0 / a;
   alpha = -1.0 / ab;
   Interval c = (x.isPositive() ? sqrt(ab) : -sqrt(ab)), fc = 1.0 / c;
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm udiv(const AffineForm &f)
{
   Interval x = f.eval();

   if (x.isEmpty() || x.containsZero())
      THROW("Affine form of unary division on " << x);

   if (x.isInf())
   {
      Interval y = 1.0 / x;
      AffineForm res(y.left(), {}, {}, Interval(0, y.width()));
      return res;
   }

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeUdiv(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevUdiv(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

AffineForm operator/(const AffineForm &f, const AffineForm &g)
{
   return f * udiv(g);
}

AffineForm AffineForm::linearize(const AffineForm &f, const Interval &alpha,
                                 const Interval &zeta, const Interval &delta)
{
   AffineForm res;

   // constant term
   res.setConstant(alpha * f.c_ + zeta);

   // linear terms
   auto it = f.cbegin();
   while (it != f.cend())
   {
      res.pushBack(alpha * it->itv, it->var);
      ++it;
   }

   // error term
   res.setError(abs(alpha) * f.e_ + delta);

   return res;
}

AffineForm AffineForm::powrec(const AffineForm &f, unsigned int e)
{
   if (e == 1)
      return f;
   else if (e % 2 == 0)
      return powrec(sqr(f), e / 2);
   else
      return f * powrec(sqr(f), (e - 1) / 2);
}

void AffineForm::minrangePow(const Interval &x, unsigned int e, Interval &alpha,
                             Interval &zeta, Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = pow(a, e), fb = pow(b, e);
   alpha = x.isPositive() ? e * pow(a, e - 1) : e * pow(b, e - 1);
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevPow(const Interval &x, unsigned int e, Interval &alpha,
                              Interval &zeta, Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = pow(a, e), fb = pow(b, e);
   alpha = (fb - fa) / (b - a);

   if (x.isPositive())
   {
      if (e % 2 == 0)
      {
         Interval c = exp(log(alpha / e) / (e - 1)), fc = pow(c, e);
         approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
      }
      else
      {
         Interval c = exp(log(alpha / e) / (e - 1)), fc = pow(c, e);
         approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
      }
   }
   else
   {
      if (e % 2 == 0)
      {
         Interval c = -exp(log(-alpha / e) / (e - 1)), fc = pow(c, e);
         approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
      }
      else
      {
         Interval c = -exp(log(alpha / e) / (e - 1)), fc = pow(c, e);
         approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
      }
   }
}

AffineForm AffineForm::powapx(const AffineForm &f, unsigned int e)
{
   Interval x = f.eval();
   if (x.isEmpty() || x.isInf())
      THROW("Affine form of pow on " << x);

   if (x.containsZero())
      return powrec(f, e);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangePow(x, e, alpha, zeta, delta);
   else
      AffineForm::chebyshevPow(x, e, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

AffineForm pow(const AffineForm &f, unsigned int e)
{
   switch (e)
   {
   case 0:
      return AffineForm(1.0);
   case 1:
      return f;
   case 2:
      return sqr(f);
   default:
      return AffineForm::powapx(f, e);
   }
}

void AffineForm::minrangePowr(const Interval &x, const double &r, Interval &alpha,
                              Interval &zeta, Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = powr(a, r), fb = powr(b, r);
   if (r < 0.0)
      alpha = r * powr(b, r - 1);
   else
      alpha = r * powr(a, r - 1);
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevPowr(const Interval &x, const double &r, Interval &alpha,
                               Interval &zeta, Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = powr(a, r), fb = powr(b, r);
   alpha = (fb - fa) / (b - a);
   Interval c = exp(log(alpha / r) / (r - 1)), fc = powr(c, r);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm powr(const AffineForm &f, const double &r)
{
   Interval x = f.eval() & Interval::positive();
   if (x.isEmpty() || x.containsZero() || x.isInf())
      THROW("Affine form of powr on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangePowr(x, r, alpha, zeta, delta);
   else
      AffineForm::chebyshevPowr(x, r, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

AffineForm min(const AffineForm &f, const AffineForm &g)
{
   Interval x = f.eval(), y = g.eval();
   if (x.isEmpty() || y.isEmpty())
      THROW("Affine form of min on " << x << " and " << y);

   if (x.overlaps(y))
   {
      Interval z = min(x, y);
      AffineForm res(z.left(), {}, {}, Interval(0, z.width()));
      return res;
   }
   else if (x.isCertainlyLt(y))
   {
      return f;
   }
   else
   {
      return g;
   }
}

AffineForm max(const AffineForm &f, const AffineForm &g)
{
   Interval x = f.eval(), y = g.eval();
   if (x.isEmpty() || y.isEmpty())
      THROW("Affine form of max on " << x << " and " << y);

   if (x.overlaps(y))
   {
      Interval z = max(x, y);
      AffineForm res(z.left(), {}, {}, Interval(0, z.width()));
      return res;
   }
   else if (x.isCertainlyGt(y))
   {
      return f;
   }
   else
   {
      return g;
   }
}

AffineForm abs(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of abs on " << x);

   if (x.strictlyContainsZero())
   {
      Interval y = abs(x);
      AffineForm res(0.0, {}, {}, y);
      return res;
   }
   else if (x.isStrictlyPositive())
   {
      return f;
   }
   else
   {
      return -f;
   }
}

AffineForm sgn(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of sgn on " << x);

   Interval y = sgn(x);

   AffineForm res(y.left(), {}, {}, Interval(0, y.width()));
   return res;
}

void AffineForm::minrangeExp(const Interval &x, Interval &alpha, Interval &zeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = exp(a), fb = exp(b);
   alpha = exp(a);
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevExp(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = exp(a), fb = exp(b);
   alpha = (fb - fa) / (b - a);
   Interval c = log(alpha), fc = alpha;
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm exp(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty() || x.isInf())
      THROW("Affine form of exp on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeExp(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevExp(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeLog(const Interval &x, Interval &alpha, Interval &zeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = log(a), fb = log(b);
   alpha = 1 / b;
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevLog(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = log(a), fb = log(b);
   alpha = (fb - fa) / (b - a);
   Interval c = 1.0 / alpha, fc = log(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm log(const AffineForm &f)
{
   Interval x = f.eval() & Interval::positive();
   if (x.isEmpty() || x.containsZero() || x.isInf())
      THROW("Affine form of log on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeLog(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevLog(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeCos(const Interval &x, Interval &alpha, Interval &zeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cos(a), fb = cos(b);
   Interval d = -sin(x);
   alpha = (d.left() > 0.0) ? d.left() : d.right(); // derivative nearest 0
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevCos(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval cx = cos(x);
   if (cx.strictlyContainsZero())
   {
      minrangeCos(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = cos(a), fb = cos(b);

   alpha = (fb - fa) / (b - a);

   if (alpha.isNegative())
   {
      // shift x + 2*k*pi in [0, pi]
      Interval k = round((Interval::zeroPi() - x) / Interval::twoPi());

      if (!k.isSingleton())
      {
         minrangeCos(x, alpha, zeta, delta);
         return;
      }
      Interval c = asin(-alpha);
      if (fa.isNegative())
         c = Interval::pi() - c;
      c -= k * Interval::twoPi();
      Interval fc = cos(c);

      approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
   }
   else
   {
      // shift x + 2*k*pi in [-pi, 0]
      Interval k = round((Interval::minusPiZero() - x) / Interval::twoPi());

      if (!k.isSingleton())
      {
         minrangeCos(x, alpha, zeta, delta);
         return;
      }

      Interval c = asin(-alpha);
      if (fa.isNegative())
         c = -(Interval::pi() + c);
      c -= k * Interval::twoPi();
      Interval fc = cos(c);

      approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
   }
}

AffineForm cos(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of cos on " << x);

   if (x.isInf() || x.containskPi())
   {
      Interval y = cos(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeCos(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevCos(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeSin(const Interval &x, Interval &alpha, Interval &zeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sin(a), fb = sin(b);
   Interval d = cos(x);
   alpha = (d.left() > 0.0) ? d.left() : d.right(); // derivative nearest 0
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevSin(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   chebyshevCos(x - Interval::halfPi(), alpha, zeta, delta);
   zeta -= alpha * Interval::halfPi();
}

AffineForm sin(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of sin on " << x);

   if (x.isInf() || x.containsHalfPiPluskPi())
   {
      Interval y = sin(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSin(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevSin(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeTan(const Interval &x, Interval &alpha, Interval &zeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = tan(a), fb = tan(b);
   Interval d = 1.0 / sqr(cos(x));
   alpha = (d.left() < d.right()) ? d.left() : d.right(); // smallest derivative
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevTan(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval tx = tan(x);

   if (tx.strictlyContainsZero())
   {
      minrangeTan(x, alpha, zeta, delta);
      return;
   }

   Interval k = round((Interval::minusHalfPiPlusHalfPi() - x) / Interval::pi());

   if (!k.isSingleton())
   {
      minrangeTan(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = tan(a), fb = tan(b);

   alpha = (fb - fa) / (b - a);

   Interval c = (tx.isPositive() ? acos(1.0 / sqrt(alpha)) : -acos(1.0 / sqrt(alpha)));
   c -= k * Interval::pi();
   Interval fc = tan(c);

   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm tan(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty() || x.isInf() || x.containsHalfPiPluskPi())
      THROW("Affine form of tan on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeTan(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevTan(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeCosh(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cosh(a), fb = cosh(b);
   Interval d = sinh(x);
   alpha = (d.left() < d.right()) ? d.left() : d.right(); // smallest derivative
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevCosh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cosh(a), fb = cosh(b);
   alpha = (fb - fa) / (b - a);
   Interval c = asinh(alpha), fc = cosh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm cosh(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of cosh on " << x);

   if (x.containsZero())
   {
      Interval y = cosh(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeCosh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevCosh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeSinh(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sinh(a), fb = sinh(b);
   Interval d = cosh(x);
   alpha = (d.left() < d.right()) ? d.left() : d.right(); // smallest derivative
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevSinh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeSinh(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = sinh(a), fb = sinh(b);
   alpha = (fb - fa) / (b - a);
   Interval c = (x.isPositive() ? acosh(alpha) : -acosh(alpha)), fc = sinh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm sinh(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of sinh on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSinh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevSinh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeTanh(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = tanh(a), fb = tanh(b);
   Interval d = 1.0 / sqr(cosh(x));
   alpha = (d.left() < d.right()) ? d.left() : d.right(); // smallest derivative
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevTanh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeTanh(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = tanh(a), fb = tanh(b);
   alpha = (fb - fa) / (b - a);
   Interval c = (x.isPositive() ? acosh(sqrt(1.0 / alpha)) : -acosh(sqrt(1.0 / alpha))),
            fc = tanh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm tanh(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of tanh on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeTanh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevTanh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAcos(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = acos(a), fb = acos(b);
   Interval d = -1.0 / sqrt(1 - sqr(x));
   alpha = (d.left() > 0.0) ? d.left() : d.right(); // derivative nearest 0
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAcos(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeAcos(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = acos(a), fb = acos(b);
   alpha = (fb - fa) / (b - a);
   Interval d = sqrt(1.0 - 1.0 / sqr(alpha)), c = (x.isNegative() ? (-d) : d),
            fc = acos(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm acos(const AffineForm &f)
{
   Interval x = f.eval() & Interval::minusOnePlusOne();
   if (x.isEmpty())
      THROW("Affine form of acos on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAcos(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAcos(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAsin(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = asin(a), fb = asin(b);
   Interval d = 1.0 / sqrt(1 - sqr(x));
   alpha = (d.left() > 0.0) ? d.left() : d.right(); // derivative nearest 0
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAsin(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeAsin(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = asin(a), fb = asin(b);
   alpha = (fb - fa) / (b - a);
   Interval d = sqrt(1.0 - 1.0 / sqr(alpha)), c = (x.isNegative() ? (-d) : d),
            fc = asin(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm asin(const AffineForm &f)
{
   Interval x = f.eval() & Interval::minusOnePlusOne();
   if (x.isEmpty())
      THROW("Affine form of asin on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAsin(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAsin(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAtan(const Interval &x, Interval &alpha, Interval &zeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = atan(a), fb = atan(b);
   Interval d = 1.0 / (1.0 + sqr(x));
   alpha = d.left();
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAtan(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeAtan(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = atan(a), fb = atan(b);
   alpha = (fb - fa) / (b - a);
   Interval d = sqrt(1.0 / alpha - 1.0), c = (x.isNegative() ? (-d) : d), fc = atan(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm atan(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of atan on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAtan(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAtan(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAcosh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = acosh(a), fb = acosh(b);
   Interval d = 1.0 / sqrt(sqr(x) - 1.0);
   alpha = d.left();
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAcosh(const Interval &x, Interval &alpha, Interval &zeta,
                                Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = acosh(a), fb = acosh(b);
   alpha = (fb - fa) / (b - a);
   Interval c = sqrt(1.0 + 1.0 / sqr(alpha)), fc = acosh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm acosh(const AffineForm &f)
{
   Interval x = f.eval() & Interval::moreThan(1.0);
   if (x.isEmpty())
      THROW("Affine form of acosh on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAcosh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAcosh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAsinh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = asinh(a), fb = asinh(b);
   Interval d = 1.0 / sqrt(1.0 + sqr(x));
   alpha = d.left();
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAsinh(const Interval &x, Interval &alpha, Interval &zeta,
                                Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeAsinh(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = asinh(a), fb = asinh(b);
   alpha = (fb - fa) / (b - a);
   Interval d = sqrt(1.0 / sqr(alpha) - 1.0), c = (x.isNegative() ? (-d) : d),
            fc = asinh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm asinh(const AffineForm &f)
{
   Interval x = f.eval();
   if (x.isEmpty())
      THROW("Affine form of asinh on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAsinh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAsinh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

void AffineForm::minrangeAtanh(const Interval &x, Interval &alpha, Interval &zeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = atanh(a), fb = atanh(b);
   Interval d = 1.0 / (1.0 - sqr(x));
   alpha = (d.left() > 0.0) ? d.left() : d.right(); // derivative nearest 0
   approxZetaDelta(alpha, a, fa, b, fb, zeta, delta);
}

void AffineForm::chebyshevAtanh(const Interval &x, Interval &alpha, Interval &zeta,
                                Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeAtanh(x, alpha, zeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = atanh(a), fb = atanh(b);
   alpha = (fb - fa) / (b - a);
   Interval d = sqrt(1.0 - 1.0 / alpha), c = (x.isNegative() ? (-d) : d), fc = atanh(c);
   approxZetaDelta(alpha, a, fa, c, fc, zeta, delta);
}

AffineForm atanh(const AffineForm &f)
{
   Interval x = f.eval() & Interval::minusOnePlusOne();
   if (x.isEmpty() || x.contains(1.0) || x.contains(-1.0))
      THROW("Affine form of atanh on " << x);

   Interval alpha, zeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeAtanh(x, alpha, zeta, delta);
   else
      AffineForm::chebyshevAtanh(x, alpha, zeta, delta);

   return AffineForm::linearize(f, alpha, zeta, delta);
}

Interval AffineForm::eval() const
{
   // sum_i a_I*[-1, 1] is calculated as follows: s is the sum of absolute values
   // sum_i |a_i| rounded upward and the result is equal to the interval [-s, s]
   Double::rndUp();
   double s = 0.0;
   for (auto it = l_.cbegin(); it != l_.cend(); ++it)
      s += Double::max(Double::abs(it->itv.left()), Double::abs(it->itv.right()));

   return c_ + Interval(-s, s) + Interval(-e_.right(), e_.right());
}

void AffineForm::approxZetaDelta(const Interval &alpha, const Interval &a,
                                 const Interval &fa, const Interval &b,
                                 const Interval &fb, Interval &zeta, Interval &delta)
{
   Interval u = 0.5 * (fa - alpha * a), v = 0.5 * (fb - alpha * b);
   zeta = u + v;
   delta = abs(u - v);
}

void AffineForm::setConstant(const Interval &x)
{
   THROW_IF(x.isEmpty() || x.isInf(), "Affine form invalid");
   c_ = x;
}

void AffineForm::setError(const Interval &x)
{
   THROW_IF(x.isEmpty() || x.isInf(), "Affine form invalid");
   e_ = x;
}

void AffineForm::pushBack(const Interval &x, int var)
{
   THROW_IF(x.isEmpty() || x.isInf(), "Affine form invalid");
   if (x.isZero())
      return;
   l_.push_back({x, var});
}

void AffineForm::push(const Interval &x, int var)
{
   THROW_IF(x.isEmpty() || x.isInf(), "Affine form invalid");
   if (x.isZero())
      return;

   if (l_.empty() || var < l_.front().var)
   {
      l_.push_front({x, var});
   }
   else if (var > l_.back().var)
   {
      l_.push_back({x, var});
   }
   else
   {
      auto it = l_.begin();
      bool iter = true;
      while (iter)
      {
         if (var < it->var)
         {
            l_.insert(it, {x, var});
            iter = false;
         }
         else if (var == it->var)
         {
            Interval y = x + it->itv;
            if (y.isZero())
            {
               l_.erase(it);
            }
            else
            {
               it->itv = y;
            }
            iter = false;
         }
         else
         {
            ++it;
         }
      }
   }
}

void AffineForm::push(const AffineForm &f)
{
   setConstant(c_ + f.c_);
   setError(e_ + f.e_);
   auto it = f.cbegin();
   while (it != f.cend())
   {
      push(it->itv, it->var);
      ++it;
   }
}

AffineForm::iterator AffineForm::begin()
{
   return l_.begin();
}

AffineForm::iterator AffineForm::end()
{
   return l_.end();
}

int AffineForm::var(const iterator &it)
{
   return it->var;
}

const Interval &AffineForm::itv(const iterator &it)
{
   return it->itv;
}

AffineForm::const_iterator AffineForm::cbegin() const
{
   return l_.cbegin();
}

AffineForm::const_iterator AffineForm::cend() const
{
   return l_.cend();
}

int AffineForm::var(const const_iterator &it)
{
   return it->var;
}

const Interval &AffineForm::itv(const const_iterator &it)
{
   return it->itv;
}

std::ostream &operator<<(std::ostream &os, const AffineForm &f)
{
   os << f.c_;

   auto it = f.cbegin();
   while (it != f.cend())
   {
      if (it->itv.isNegative())
         os << " - " << abs(it->itv);
      else
         os << " + " << it->itv;

      os << "*v" << AffineForm::var(it);
      ++it;
   }

   os << " ! " << f.e_;
   return os;
}

} // namespace realpaver
