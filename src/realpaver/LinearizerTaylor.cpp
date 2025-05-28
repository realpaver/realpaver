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
 * @file   LinearizerTaylor.cpp
 * @brief  Linearization of nonlinear problems based on Taylor forms
 * @author Laurent Granvilliers
 * @date   11 Fev 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/LinearizerTaylor.hpp"

namespace realpaver {
LinearizerTaylor::LinearizerTaylor(SharedDag dag, bool hansen, CornerStyle style,
                                   unsigned seed)
    : Linearizer(dag)
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

LinearizerTaylor::LinearizerTaylor(SharedDag dag, const IndexList &lfun, bool hansen,
                                   CornerStyle style, unsigned seed)
    : Linearizer(dag, lfun)
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

void LinearizerTaylor::useHansenDerivatives(bool b)
{
   hansen_ = b;
}

void LinearizerTaylor::fixCorner(const Bitset &corner, bool opposite)
{
   ASSERT(corner_.size() == corner.size(), "Bad corner");

   style_ = opposite ? CornerStyle::UserOpposite : CornerStyle::User;
   corner_ = corner;
}

void LinearizerTaylor::selectCorner(const IntervalBox &B)
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

void LinearizerTaylor::makeVars(LPModel &lpm, const IntervalBox &B)
{
   for (const auto &v : scop_)
   {
      Interval val = B.get(v);
      LinVar lv = lpm.makeVar(val.left(), val.right());
      lv.setName(v.getName());
      setLinVarIndex(v.id(), lv.getIndex());
   }
}

bool LinearizerTaylor::makeOne(LPModel &lpm, const IntervalBox &B, const Bitset &corner)
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

   // evaluates the functions at c
   IntervalVector fc(lfun_.size());
   for (size_t i : lfun_)
   {
      DagFun *f = dag_->fun(i);
      Interval x = f->iEval(c);
      if (x.isEmpty())
         return false;
      fc.set(i, x);
   }

   // generates the constraints
   for (size_t i : lfun_)
   {
      DagFun *f = dag_->fun(i);
      Interval img = f->getImage();

      // transforms an equation into an inequality
      if (img.isSingleton())
         img += Interval(-tol_, tol_);

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

      // lower bounding constraint of the form lo <= u generated if the right
      // bound of the image of the function is finite
      if (!Double::isInf(img.right()))
      {
         Interval u = img.right() - fc.get(i);
         LinExpr lo;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => right bound of the derivative
               lo.addTerm(z.right(), lv);
               u += z.right() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 0)
               // => left bound of the derivative
               lo.addTerm(z.left(), lv);
               u += z.left() * Interval(c.get(v));
            }
         }
         lpm.addCtr(lo, u.right());
      }

      // upper bounding constraint of the form up >= l generated if the left
      // bound of the image of the function is finite
      if (!Double::isInf(img.left()))
      {
         Interval l = img.left() - fc.get(i);

         LinExpr up;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            Interval z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => left bound of the derivative
               up.addTerm(z.left(), lv);
               l += z.left() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 1)
               // => right bound of the derivative
               up.addTerm(z.right(), lv);
               l += z.right() * Interval(c.get(v));
            }
         }
         lpm.addCtr(l.left(), up);
      }
   }
   return true;
}

bool LinearizerTaylor::makeCtrs(LPModel &lpm, const IntervalBox &B)
{
   // selects the first corner
   selectCorner(B);

   // generates the constraints for this corner
   bool b = makeOne(lpm, B, corner_);
   if (!b)
      return false;

   // manages the opposite corner if necessary
   if (style_ == CornerStyle::RandomOpposite || style_ == CornerStyle::UserOpposite)
   {
      corner_.flipAll();
      return makeOne(lpm, B, corner_);
   }
   else
      return true;
}

bool LinearizerTaylor::make(LPModel &lpm, const IntervalBox &B)
{
   makeVars(lpm, B);
   return makeCtrs(lpm, B);
}

} // namespace realpaver
