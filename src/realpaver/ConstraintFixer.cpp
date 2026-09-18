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
 * @file   ConstraintFixer.cpp
 * @brief  Rewriting of constraints
 * @author Laurent Granvilliers
 * @date   28 May 2025
 */

#include "Constraint.hpp"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintFixer.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintTable.hpp"
#include "realpaver/TermFixer.hpp"
#include <memory>

namespace realpaver {

ConstraintFixer::ConstraintFixer(SharedProblemBridge bridge)
    : bridge_(bridge)
    , rc_()
{
   REALPAVER_ASSERT(bridge != nullptr, "Bad initiualization of a constraint fixer");
}

Constraint ConstraintFixer::getConstraint() const
{
   return rc_;
}

void ConstraintFixer::apply(const ConstraintCond *c)
{
   ConstraintFixer fg(bridge_), bg(bridge_);
   c->guard().acceptVisitor(fg);
   c->body().acceptVisitor(bg);
   rc_ = cond(fg.getConstraint(), bg.getConstraint());
}

void ConstraintFixer::apply(const ConstraintTable *c)
{
   // checks the consistent assignments with respect to the variable domains
   Bitset consistent(c->nbRows());
   consistent.setAllZero();

   const DomainBox &box = bridge_->srcRegion();

   for (size_t i = 0; i < c->nbRows(); ++i)
   {
      size_t j = 0;
      bool cons = true;
      while (cons && j < c->nbCols())
      {
         Variable v = c->getVar(j);

         if (bridge_->isUnfixed(v))
         {
            if (c->getVal(i, j).isDisjoint(box.get(v)->intervalHull()))
               cons = false;
         }
         ++j;
      }
      if (cons)
         consistent.setOne(i);
   }

   // rewrites the constraint
   std::shared_ptr<ConstraintTable> tbl = std::make_shared<ConstraintTable>();

   for (size_t j = 0; j < c->nbCols(); ++j)
   {
      Variable v = c->getVar(j);
      if (bridge_->isUnfixed(v))
      {
         Variable w = bridge_->srcToDestVar(v);
         ConstraintTableCol col(w);
         for (size_t i = 0; i < c->nbRows(); ++i)
            if (consistent.get(i))
               col.addValue(c->getVal(i, j));

         tbl->addCol(col);
      }
   }
   tbl->removeRedundantRows();
   rc_ = Constraint(tbl);
}

void ConstraintFixer::apply(const ConstraintPiecewise *c)
{
   const DomainBox &box = bridge_->srcRegion();
   Variable v = c->variable();
   Interval vdom = box.get(v)->intervalHull();

   // is the reference variable fixed ?
   if (bridge_->isFixed(c->variable()))
   {
      // yes, there are two cases:
      // - one active guard
      // - two active guards that share one bound which is also the value of the reference
      //   variable
      int n1 = -1, n2 = -1;
      for (size_t i = 0; i < c->nbPieces(); i++)
      {
         if (vdom.overlaps(c->interval(i)))
         {
            if (n1 == -1)
               n1 = i;
            else
               n2 = i;
         }
      }

      // one active guard?
      if (n2 == -1)
      {
         // yes => only the constraint of the active guard
         ConstraintFixer fixer(bridge_);
         c->constraint(n1).acceptVisitor(fixer);
         rc_ = fixer.getConstraint();
      }
      else
      {
         // rewrites the body of the first constraint
         ConstraintFixer fixer1(bridge_);
         c->constraint(n1).acceptVisitor(fixer1);
         Constraint c1 = fixer1.getConstraint();
         if (c1.isConstant())
            n1 = -1;

         // rewrites the body of the second constraint
         ConstraintFixer fixer2(bridge_);
         c->constraint(n2).acceptVisitor(fixer2);
         Constraint c2 = fixer2.getConstraint();
         if (c2.isConstant())
            n2 = -1;

         if (n1 == -1)
         {
            rc_ = c2;
         }
         else if (n2 == -1)
         {
            rc_ = c1;
         }
         else
         {
            // creates a piecewise constraint on a new variable associated with v in the
            // destination poblem
            Variable w = bridge_->insertVar(v, *box.get(v));
            std::shared_ptr<ConstraintPiecewise> pw =
                std::make_shared<ConstraintPiecewise>(w);

            Variable b1 = bridge_->srcToDestVar(c->binary(n1)),
                     b2 = bridge_->srcToDestVar(c->binary(n2));

            pw->addPiece(b1, c->interval(n1), c1);
            pw->addPiece(b2, c->interval(n2), c2);

            rc_ = Constraint(pw);

            // remark: v is now is fixed and unfixed in the bridge
            // it must be removed as a fixed variable
            bridge_->removeFixedVar(v);
         }
      }
   }
   else
   {
      // gets the new variable associated with v and makes a piecewise constraint for w
      Variable w = bridge_->srcToDestVar(v);
      std::shared_ptr<ConstraintPiecewise> pw = std::make_shared<ConstraintPiecewise>(w);

      // examines the different pieces
      for (size_t i = 0; i < c->nbPieces(); i++)
      {
         if (!vdom.overlaps(c->interval(i)))
            continue; // piece inactive

         Variable bv = c->binary(i);
         if (bridge_->isFixed(bv))
         {
            // binary variable fixed: piece not considered if its domain
            // is 0, otherwise only this piece is active
            Interval bdom = box.get(bv)->intervalHull();
            if (bdom.isOne())
            {
               ConstraintFixer fixer(bridge_);
               c->constraint(i).acceptVisitor(fixer);
               rc_ = fixer.getConstraint();
               return;
            }
         }
         else
         {
            // binary variable not fixed
            Variable bw = bridge_->srcToDestVar(bv);
            ConstraintFixer fixer(bridge_);
            c->constraint(i).acceptVisitor(fixer);

            Constraint body = fixer.getConstraint();
            if (!body.isConstant())
               pw->addPiece(bw, vdom & c->interval(i), body);
         }
      }

      if (pw->nbPieces() > 1)
         rc_ = Constraint(pw);
      else
         rc_ = pw->constraint(0);
   }
}

void ConstraintFixer::apply(const ConstraintEq *c)
{
   TermFixer vl(bridge_), vr(bridge_);
   c->left().acceptVisitor(vl);
   c->right().acceptVisitor(vr);
   rc_ = (vl.getTerm() == vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintLe *c)
{
   TermFixer vl(bridge_), vr(bridge_);
   c->left().acceptVisitor(vl);
   c->right().acceptVisitor(vr);
   rc_ = (vl.getTerm() <= vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintLt *c)
{
   TermFixer vl(bridge_), vr(bridge_);
   c->left().acceptVisitor(vl);
   c->right().acceptVisitor(vr);
   rc_ = (vl.getTerm() < vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintGe *c)
{
   TermFixer vl(bridge_), vr(bridge_);
   c->left().acceptVisitor(vl);
   c->right().acceptVisitor(vr);
   rc_ = (vl.getTerm() >= vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintGt *c)
{
   TermFixer vl(bridge_), vr(bridge_);
   c->left().acceptVisitor(vl);
   c->right().acceptVisitor(vr);
   rc_ = (vl.getTerm() > vr.getTerm());
}

void ConstraintFixer::apply(const ConstraintIn *c)
{
   TermFixer vis(bridge_);
   c->term().acceptVisitor(vis);
   rc_ = in(vis.getTerm(), c->image());
}

} // namespace realpaver
