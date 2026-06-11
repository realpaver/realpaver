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
    : LinearizerInner(dag)
    , hansen_(hansen)
    , style_(style)
    , corner_(scope().size())
    , gen_()
    , cobj_(cobj)
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

   // generates the constraints
   for (size_t i = 0; i < edag_->size(); ++i)
   {
      SharedDagFun f;
      if (i != cobj_)
         f = edag_->getActiveFun(i, B, active);
      else
         f = edag_->dag()->fun(i);

      if (f == nullptr || (f->getImage().width() == 0 && i != cobj_))
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

      // if (i == cobj_)
      // {
      //    std::cout << ":::" << dag_->fun(i)->getTerm() << " in "
      //              << dag_->fun(i)->getImage() << std::endl;
      // }

      // lower bounding constraint of the form lo <= u generated if the right
      // bound of the image of the function is finite
      if (!Double::isInf(img.right()) && (i != cobj_ || style_ != CornerStyle::Left))
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
               // => right bound of the derivative
               lo.addTerm(z.left(), lv);
               u += z.left() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 0)
               // => left bound of the derivative
               lo.addTerm(z.right(), lv);
               u += z.right() * Interval(c.get(v));
            }
         }
         lpm.addCtr(lo, u.left() - lpm.getFeasTol());
      }

      // upper bounding constraint of the form up >= l generated if the left
      // bound of the image of the function is finite
      if (!Double::isInf(img.left()) && (i != cobj_ || style_ != CornerStyle::Right))
      {
         Interval l = img.left() - fc;

         LinExpr up;

         for (const auto &v : f->scope())
         {
            LinVar lv = lpm.getLinVar(linVarIndex(v));

            // derivative of f wrt. v
            const Interval &z = G[f->scope().index(v)];

            if (corner.get(scop_.index(v)))
            {
               // right bound used for this variable (bit = 1)
               // => left bound of the derivative
               up.addTerm(-z.right(), lv);
               l += z.right() * Interval(c.get(v));
            }
            else
            {
               // left bound used for this variable (bit = 1)
               // => right bound of the derivative
               up.addTerm(-z.left(), lv);
               l += z.left() * Interval(c.get(v));
            }
         }
         lpm.addCtr(up, -l.right() - lpm.getFeasTol());
      }
   }
   // std::cout << "NB LINCTR: " << lpm.getNbLinCtrs() << std::endl;
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
