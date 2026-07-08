/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2026 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   LinearizerInnerTaylor.cpp
 * @brief  Linearization of nonlinear problems based on Taylor forms
 * @author Raphaël Chenouard
 * @date   11 Feb 2026
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/LinearizerInnerTaylor.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {
LinearizerInnerTaylor::LinearizerInnerTaylor(SharedDagExtended dag, int cobj, bool hansen,
                                             CornerStyle style, unsigned seed)
    : LinearizerInner(dag, cobj)
    , hansen_(hansen)
    , style_(style)
    , corner_(scope().size())
    , gen_()
{
   if (seed == 0)
      gen_ = IntRandom();
   else
      gen_ = IntRandom(seed);
}

void LinearizerInnerTaylor::useHansenDerivatives(bool b)
{
   hansen_ = b;
}

void LinearizerInnerTaylor::fixCorner(const Bitset &corner, bool opposite)
{
   REALPAVER_ASSERT(corner_.size() == corner.size(), "Bad corner");

   style_ = opposite ? CornerStyle::UserOpposite : CornerStyle::User;
   corner_ = corner;
}

void LinearizerInnerTaylor::selectCorner(const IntervalBox &B)
{
   switch (style_)
   {
   case CornerStyle::Random:
   case CornerStyle::RandomOpposite:
      for (size_t i = 0; i < corner_.size(); ++i)
      {
         if (gen_.nextBool())
            corner_.setOne(i);
         else
            corner_.setZero(i);
      }
      break;
   case CornerStyle::Left:
   case CornerStyle::LeftRight:
      corner_.setAllZero();
      break;
   case CornerStyle::Right:
      corner_.setAllOne();
      break;
   case CornerStyle::User:
   case CornerStyle::UserOpposite:
      // nothing
      break;
   }
}

void LinearizerInnerTaylor::makeVars(LPModel &lpm, const IntervalBox &B)
{
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      setLinVarIndex(v.id(), lv.getIndex());
   }
}

bool LinearizerInnerTaylor::makeOne(LPModel &lpm, const IntervalBox &B,
                                    const Bitset &corner, const Bitset &active)
{
   // makes the corner of the box
   RealPoint c(scop_);
   for (const auto &v : scop_)
   {
      Interval dom = B.get(v);
      if (corner.get(scop_.index(v)))
         c.set(v, dom.right());
      else
         c.set(v, dom.left());
   }

   LOG_INTER("ACTIVE: " << active)

   // generates the constraints
   for (size_t i = 0; i < edag_->size(); ++i)
   {
      SharedDagFun f;
      if (i != cobj_)
         f = edag_->getActiveFun(i, B, active);
      else
         f = edag_->dag()->fun(i);

      // Case of inactive constraints or equalities if not relaxed (except objective
      // constraint)
      if (f == nullptr ||
          (f->getImage().width() == 0 && i != cobj_ &&
           getRelaxTol() == 0)) // || (f->getImage().width() == 0 && i != cobj_))
         continue;

      LOG_LOW("Taylor relaxation for constraint @ " << f->index());
      Interval img = f->getImage();

      // evaluates the function at c
      Interval fc = f->iEval(c);
      if (fc.isEmpty())
         return false;

      // differentiates the function
      IntervalVector G(f->nbVars());

      if (hansen_)
         f->iDiffHansen(B, c, G);
      else
         f->iDiff(B, G);

      if (G.isEmpty())
         return false;

      if (G.isInf())
         continue;

      // LOG_INTER("Gradient: " << G)

      // lower bounding constraint of the form lo <= u generated if the right
      // bound of the image of the function is finite
      // c(x)<=b
      // Inner relaxation: c(x)-b <= c_inner(x)-b <= 0
      // c_inner(x) = c(x_) + inner_taylor(x)
      // inner_taylor(x) = sum(i in 1..n)(G_i^*(x-x_))
      // c(x)-b <= c(x_) + sum(i in 1..n)(G_i^*(x-x_))-b <= 0
      // Then focusing on: c(x_) + sum(i in 1..n)(G_i^*(x-x_))-b <= 0
      // sum(i in 1..n)(G_i^*(x-x_)) <= b - c(x_)
      // sum(i in 1..n)(G_i^*x) - sum(i in 1..n)(G_i^*x_) <= b - c(x_)
      // lo=sum(i in 1..n)(G_i^*x) <= b - c(x_) + sum(i in 1..n)(G_i^*x_)=u
      // lo <=u
      if (!Double::isInf(img.right()) && i != cobj_)
      {
         Interval u = img.right() - fc;
         LinExpr lo;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            const Interval &z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => left bound of the derivative //(i.e. opposite bound)
               lo.addTerm(z.left(), lv);           // G_i_ * x
               u += z.left() * Interval(c.get(v)); // G_i_ * x^
            }
            else
            {
               // left bound used for this variable (bit = 0)
               // => right bound of the derivative
               lo.addTerm(z.right(), lv);           // G_i^ * x
               u += z.right() * Interval(c.get(v)); // G_i^ * x_
            }
         }

         if (img.width() == 0) // equation c(x) = b, relax as c_inner(x) <= b + tol_
         {
            lpm.addCtr(lo, u.left() + getRelaxTol());
         }
         else // inequalities or objective constraint
            lpm.addCtr(lo, u.left());
      }

      // upper bounding constraint of the form l <= up generated if the left
      // bound of the image of the function is finite
      // b <= c(x) <=> -c(x) <= -b <=> c'(x) <= b'
      // then previous formula applies:
      // c_inner`(x) = -c_inner(x) = -c(x_) - inner_taylor(x)
      // Then: -c(x) <= -c(x_) - inner_taylor(x) <= -b
      // inner_taylor(x) = sum(i in 1..n)(G_i^*(x-x_))
      // -c(x) + b <= -c(x_) - sum(i in 1..n)(G_i^*(x-x_)) + b <= 0
      // Then focusing on: -c(x_) - sum(i in 1..n)(G_i^*(x-x_)) + b <= 0
      // -sum(i in 1..n)(G_i^*(x-x_)) <= c(x_) - b
      // -sum(i in 1..n)(G_i^*x) + sum(i in 1..n)(G_i^*x_) <= c(x_) - b
      // sum(i in 1..n)(-G_i^*x) <= c(x_) - b + sum(i in 1..n)(-G_i^*x_)
      // up' <= l'
      // -l' <= -up'
      // b - c(x_) + sum(i in 1..n)(G_i^*x_) <= sum(i in 1..n)(G_i^*x)
      if (!Double::isInf(img.left()))
      {
         Interval l = fc - img.left(); // c(x_)^ - b

         LinExpr up;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            const Interval &z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               up.addTerm(-z.right(), lv);           // -G_i^*x
               l += -z.right() * Interval(c.get(v)); // -G_i^*x^
            }
            else
            {
               up.addTerm(-z.left(), lv);           // -G_i_*x
               l += -z.left() * Interval(c.get(v)); // -G_i_*x_
            }
         }

         if (img.width() == 0 &&
             i != cobj_) // equation c(x) = b, relax as b - tol_ <= c_inner(x)
         {
            lpm.addCtr(up, l.left() + getRelaxTol());
         }
         else
            lpm.addCtr(up, l.left());
      }
   }
   return true;
}

bool LinearizerInnerTaylor::makeCtrs(LPModel &lpm, const IntervalBox &B,
                                     const Bitset &active)
{
   // selects the first corner
   selectCorner(B);

   // generates the constraints for this corner
   bool b = makeOne(lpm, B, corner_, active);
   if (!b)
      return false;

   // manages the opposite corner if necessary
   if (style_ == CornerStyle::RandomOpposite || style_ == CornerStyle::UserOpposite ||
       style_ == CornerStyle::LeftRight)
   {
      corner_.flipAll();
      return makeOne(lpm, B, corner_, active);
   }
   else
      return true;
}

bool LinearizerInnerTaylor::makeLPImpl(LPModel &lpm, const IntervalBox &B,
                                       const Bitset &active)
{
   makeVars(lpm, B);
   return makeCtrs(lpm, B, active);
}

} // namespace realpaver
