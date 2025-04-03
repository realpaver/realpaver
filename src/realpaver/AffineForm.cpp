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
 * @date   10/03/2025
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
   ASSERT(!x.isEmpty(), "Affine form invalid (empty)");
   ASSERT(!x.isInf(), "Affine form invalid (inf)");
}

AffineForm::AffineForm(const double &a)
    : c_(a)
    , l_()
    , e_(0.0)
{
   ASSERT(!Double::isNan(a), "Affine form invalid (NaN)");
   ASSERT(!Double::isInf(a), "Affine form invalid (inf)");
}

AffineForm::AffineForm(int var, const Interval &domain)
    : c_(0.0)
    , l_()
    , e_(0.0)
{
   ASSERT(!domain.isEmpty(), "Affine form invalid (empty domain)");

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
   ASSERT(!Double::isNan(a0), "Affine form invalid (NaN)");
   ASSERT(!Double::isInf(a0), "Affine form invalid (inf)");
   ASSERT(A.size() == V.size(), "Bad initialization of an affine form");

   auto ita = A.begin();
   auto itv = V.begin();

   while (ita != A.end())
   {
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
   ASSERT(!a0.isEmpty(), "Affine form invalid (empty)");
   ASSERT(!a0.isInf(), "Affine form invalid (inf)");
   ASSERT(A.size() == V.size(), "Bad initialization of an affine form");

   auto ita = A.begin();
   auto itv = V.begin();

   while (ita != A.end())
   {
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

void AffineForm::setEmpty()
{
   c_ = Interval::emptyset();
}

bool AffineForm::isEmpty() const
{
   return c_.isEmpty();
}

size_t AffineForm::nbLinearTerms() const
{
   return l_.size();
}

AffineForm AffineForm::emptyForm()
{
   AffineForm res;
   res.setEmpty();
   return res;
}

AffineForm AffineForm::infForm()
{
   AffineForm res;
   res.e_ = Interval::positive();
   return res;
}

bool AffineForm::isInf() const
{
   return e_.isInf();
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
   MINRANGE = b;
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
   if (f.isEmpty() || g.isEmpty())
      return AffineForm::emptyForm();

   AffineForm res;

   // constants
   res.c_ = f.c_ + g.c_;

   // error terms
   res.e_ = f.e_ + g.e_;

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
   if (f.isEmpty() || g.isEmpty())
      return AffineForm::emptyForm();

   AffineForm res;

   // constants
   res.c_ = f.c_ - g.c_;

   // error terms (sum of errors)
   res.e_ = f.e_ + g.e_;

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
   if (f.isEmpty())
      return AffineForm::emptyForm();

   AffineForm res;

   // constant term
   res.c_ = -f.c_;

   // error term (the same)
   res.e_ = f.e_;

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
   if (f.isEmpty() || g.isEmpty())
      return AffineForm::emptyForm();

   AffineForm res;

   Interval A = abs(f.e_), B = abs(g.e_);

   // constant term
   res.c_ = f.c_ * g.c_;

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
   res.e_ += abs(f.c_) * g.e_ + abs(g.c_) * f.e_ + A * B;

   return res;
}

AffineForm sqr(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   AffineForm res;

   // constant term
   res.c_ = sqr(f.c_);

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
   res.e_ = abs(a2) * f.e_ + T;

   // constant Term
   res.c_ += T;

   return res;
}

void AffineForm::minrangeSqrt(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sqrt(a), fb = sqrt(b);

   alpha = 1.0 / (2.0 * fb);
   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevSqrt(const Interval &x, Interval &alpha, Interval &dzeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sqrt(a), fb = sqrt(b);

   alpha = (fb - fa) / (b - a);

   // c is such that f'(c) = alpha -> 1/2*sqrt(x)=alpha
   Interval c = 0.25 / sqr(alpha), fc = sqrt(c);

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm sqrt(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();

   if (x.isEmpty() || x.isStrictlyNegative())
      return AffineForm::emptyForm();

   if (x.right() == 0.0)
      return AffineForm();

   if (x.left() < 0.0)
      x.setLeft(0.0);
   if (x.isInf())
      return AffineForm::infForm();

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSqrt(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevSqrt(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeUdiv(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = 1.0 / a, fb = 1.0 / b;

   alpha = x.isPositive() ? (-1.0 / sqr(b)) : (-1.0 / sqr(a));
   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevUdiv(const Interval &x, Interval &alpha, Interval &dzeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), ab = a * b, fa = 1.0 / a;

   // (f(b)-f(a))/(b-a) -> -1/ab
   alpha = -1.0 / ab;

   // c is such that f'(c) = alpha -> c = sqrt(ab)
   Interval c = (x.isPositive() ? sqrt(ab) : -sqrt(ab)), fc = 1.0 / c;

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm udiv(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();

   if (x.isEmpty() || x.isZero())
      return AffineForm::emptyForm();
   if (x.containsZero())
      return AffineForm::infForm();
   if (x.isInf())
   {
      Interval y = 1.0 / x;
      AffineForm res(y.left(), {}, {}, Interval(0, y.width()));
      return res;
   }

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeUdiv(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevUdiv(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

AffineForm operator/(const AffineForm &f, const AffineForm &g)
{
   return f * udiv(g);
}

AffineForm AffineForm::linearize(const AffineForm &f, const Interval &alpha,
                                 const Interval &dzeta, const Interval &delta)
{
   AffineForm res;

   // constant term
   res.c_ = alpha * f.c_ + dzeta;

   // linear terms
   auto it = f.cbegin();
   while (it != f.cend())
   {
      res.pushBack(alpha * it->itv, it->var);
      ++it;
   }

   // error term
   res.e_ = abs(alpha) * f.e_ + delta;

   return res;
}

AffineForm AffineForm::powrec(const AffineForm &f, int e)
{
   if (e == 1)
      return f;
   else if (e % 2 == 0)
      return powrec(sqr(f), e / 2);
   else
      return f * powrec(sqr(f), (e - 1) / 2);
}

AffineForm pow(const AffineForm &f, int e)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   if (e > 2)
      return AffineForm::powrec(f, e);
   else if (e == 2)
      return sqr(f);
   else if (e == 1)
      return f;
   else if (e == 0)
      return AffineForm(1.0);
   else
      return udiv(pow(f, -e));
}

AffineForm min(const AffineForm &f, const AffineForm &g)
{
   if (f.isEmpty() || g.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval(), y = g.eval();
   if (x.isEmpty() || y.isEmpty())
      return AffineForm::emptyForm();

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
   if (f.isEmpty() || g.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval(), y = g.eval();
   if (x.isEmpty() || y.isEmpty())
      return AffineForm::emptyForm();

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
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();

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
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();

   Interval y = sgn(x);

   AffineForm res(y.left(), {}, {}, Interval(0, y.width()));
   return res;
}

void AffineForm::minrangeExp(const Interval &x, Interval &alpha, Interval &dzeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = exp(a), fb = exp(b);

   alpha = exp(a);
   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevExp(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = exp(a), fb = exp(b);

   alpha = (fb - fa) / (b - a);

   Interval c = log(alpha), fc = alpha;
   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm exp(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();
   if (x.isInf())
      return AffineForm::infForm();

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeExp(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevExp(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeLog(const Interval &x, Interval &alpha, Interval &dzeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = log(a), fb = log(b);

   alpha = 1 / b;
   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevLog(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = log(a), fb = log(b);

   alpha = (fb - fa) / (b - a);

   Interval c = 1.0 / alpha, fc = log(c);
   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm log(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty() || x.isNegative())
      return AffineForm::emptyForm();
   if (x.containsZero() || x.isInf())
      return AffineForm::infForm();

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeLog(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevLog(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeCos(const Interval &x, Interval &alpha, Interval &dzeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cos(a), fb = cos(b);

   // interval of derivatives
   Interval d = -sin(x);

   // derivative nearest 0
   alpha = (d.left() > 0.0) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevCos(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval cx = cos(x);
   if (cx.strictlyContainsZero())
   {
      minrangeCos(x, alpha, dzeta, delta);
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
         minrangeCos(x, alpha, dzeta, delta);
         return;
      }
      Interval c = asin(-alpha);
      if (fa.isNegative())
         c = Interval::pi() - c;
      c -= k * Interval::twoPi();
      Interval fc = cos(c);

      approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
   }
   else
   {
      // shift x + 2*k*pi in [-pi, 0]
      Interval k = round((Interval::minusPiZero() - x) / Interval::twoPi());

      if (!k.isSingleton())
      {
         minrangeTan(x, alpha, dzeta, delta);
         return;
      }

      Interval c = asin(-alpha);
      if (fa.isNegative())
         c = -(Interval::pi() + c);
      c -= k * Interval::twoPi();
      Interval fc = cos(c);

      approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
   }
}

AffineForm cos(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();
   if (x.isInf() || x.containskPi())
   {
      Interval y = cos(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeCos(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevCos(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeSin(const Interval &x, Interval &alpha, Interval &dzeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sin(a), fb = sin(b);

   // interval of derivatives
   Interval d = cos(x);

   // derivative nearest 0
   alpha = (d.left() > 0.0) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevSin(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   chebyshevCos(x - Interval::halfPi(), alpha, dzeta, delta);
   dzeta -= alpha * Interval::halfPi();
}

AffineForm sin(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();
   if (x.isInf() || x.containsHalfPiPluskPi())
   {
      Interval y = sin(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSin(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevSin(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeTan(const Interval &x, Interval &alpha, Interval &dzeta,
                             Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = tan(a), fb = tan(b);

   // interval of derivatives
   Interval d = 1.0 / sqr(cos(x));

   // smallest derivative
   alpha = (d.left() < d.right()) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevTan(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval tx = tan(x);

   if (tx.strictlyContainsZero())
   {
      minrangeTan(x, alpha, dzeta, delta);
      return;
   }

   Interval k = round((Interval::minusHalfPiPlusHalfPi() - x) / Interval::pi());

   if (!k.isSingleton())
   {
      minrangeTan(x, alpha, dzeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = tan(a), fb = tan(b);

   alpha = (fb - fa) / (b - a);

   Interval c = (tx.isPositive() ? acos(1.0 / sqrt(alpha)) : -acos(1.0 / sqrt(alpha)));
   c -= k * Interval::pi();
   Interval fc = tan(c);

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm tan(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();
   if (x.isInf() || x.containsHalfPiPluskPi())
   {
      return AffineForm::infForm();
   }

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeTan(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevTan(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeCosh(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cosh(a), fb = cosh(b);

   // interval of derivatives
   Interval d = sinh(x);

   // smallest derivative
   alpha = (d.left() < d.right()) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevCosh(const Interval &x, Interval &alpha, Interval &dzeta,
                               Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = cosh(a), fb = cosh(b);

   alpha = (fb - fa) / (b - a);

   Interval c = asinh(alpha), fc = cosh(c);

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm cosh(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();

   if (x.containsZero())
   {
      Interval y = cosh(x);
      return AffineForm(y.left(), {}, {}, Interval(0, y.width()));
   }

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeCosh(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevCosh(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeSinh(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = sinh(a), fb = sinh(b);

   // interval of derivatives
   Interval d = cosh(x);

   // smallest derivative
   alpha = (d.left() < d.right()) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevSinh(const Interval &x, Interval &alpha, Interval &dzeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeSinh(x, alpha, dzeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = sinh(a), fb = sinh(b);

   alpha = (fb - fa) / (b - a);

   Interval c = (x.isPositive() ? acosh(alpha) : -acosh(alpha)), fc = sinh(c);

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm sinh(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeSinh(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevSinh(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

void AffineForm::minrangeTanh(const Interval &x, Interval &alpha, Interval &dzeta,
                              Interval &delta)
{
   Interval a(x.left()), b(x.right()), fa = tanh(a), fb = tanh(b);

   // interval of derivatives
   Interval d = 1.0 / sqr(cosh(x));

   // smallest derivative
   alpha = (d.left() < d.right()) ? d.left() : d.right();

   approxDzetaDelta(alpha, a, fa, b, fb, dzeta, delta);
}

void AffineForm::chebyshevTanh(const Interval &x, Interval &alpha, Interval &dzeta,
                               Interval &delta)
{
   if (x.strictlyContainsZero())
   {
      minrangeTanh(x, alpha, dzeta, delta);
      return;
   }

   Interval a(x.left()), b(x.right()), fa = tanh(a), fb = tanh(b);

   alpha = (fb - fa) / (b - a);

   Interval c = (x.isPositive() ? acosh(sqrt(1.0 / alpha)) : -acosh(sqrt(1.0 / alpha))),
            fc = tanh(c);

   approxDzetaDelta(alpha, a, fa, c, fc, dzeta, delta);
}

AffineForm tanh(const AffineForm &f)
{
   if (f.isEmpty())
      return AffineForm::emptyForm();

   Interval x = f.eval();
   if (x.isEmpty())
      return AffineForm::emptyForm();

   Interval alpha, dzeta, delta;

   if (AffineForm::usesMinrange())
      AffineForm::minrangeTanh(x, alpha, dzeta, delta);
   else
      AffineForm::chebyshevTanh(x, alpha, dzeta, delta);

   return AffineForm::linearize(f, alpha, dzeta, delta);
}

Interval AffineForm::eval() const
{
   Interval res = c_ + e_ * Interval::minusOnePlusOne();
   auto it = l_.cbegin();
   while (it != l_.cend())
   {
      res += it->itv * Interval::minusOnePlusOne();
      ++it;
   }
   return res;
}

void AffineForm::approxDzetaDelta(const Interval &alpha, const Interval &a,
                                  const Interval &fa, const Interval &b,
                                  const Interval &fb, Interval &dzeta, Interval &delta)
{
   Interval u = 0.5 * (fa - alpha * a), v = 0.5 * (fb - alpha * b);
   dzeta = u + v;
   delta = abs(u - v);
}

void AffineForm::pushBack(const Interval &x, int var)
{
   if (x.isZero())
      return;
   l_.push_back({x, var});
}

void AffineForm::push(const Interval &x, int var)
{
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
   c_ += f.c_;
   e_ += f.e_;
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
