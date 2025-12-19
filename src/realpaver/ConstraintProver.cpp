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
 * @file   ConstraintProver.cpp
 * @brief  Prover for feasible points
 * @author Laurent Granvilliers
 * @date   29 Sep 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintArithmetic.hpp"
#include "realpaver/ConstraintCond.hpp"
#include "realpaver/ConstraintPiecewise.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/ConstraintTable.hpp"

namespace realpaver {

ConstraintProver::ConstraintProver(IntervalBox &B, double tol)
    : ConstraintVisitor()
    , B_(&B)
    , tol_(tol)
    , feasible_(false)
{
   ASSERT(tol >= 0.0, "A tolerance used to relax constraints must be positive");
}

double ConstraintProver::getTolerance() const
{
   return tol_;
}

bool ConstraintProver::isFeasible() const
{
   return feasible_;
}

void ConstraintProver::apply(const ConstraintCond *c)
{
   ConstraintCond *ptr = const_cast<ConstraintCond *>(c);

   Proof proof = ptr->guard().isSatisfied(*B_);

   if (proof == Proof::Empty)
   {
      feasible_ = true;
   }
   else if (proof == Proof::Inner)
   {
      ConstraintProver prover(*B_, tol_);
      ptr->body().acceptVisitor(prover);
      feasible_ = prover.isFeasible();
   }
   else
   {
      feasible_ = false;
   }
}

void ConstraintProver::apply(const ConstraintTable *c)
{
   ConstraintTable *ptr = const_cast<ConstraintTable *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

void ConstraintProver::apply(const ConstraintPiecewise *c)
{
   ConstraintPiecewise *ptr = const_cast<ConstraintPiecewise *>(c);
   Interval dom = B_->get(ptr->variable());

   // counts the number of active guards
   int ng = 0, j = -1;
   for (size_t i = 0; i < ptr->nbPieces(); i++)
   {
      if (B_->get(ptr->binary(i)).isOne() && ptr->interval(i).contains(dom))
      {
         ++ng;
         j = i;
      }
   }
   if (ng == 1)
   {
      // only one active guard => checks the associated constraint
      ConstraintProver prover(*B_, tol_);
      ptr->constraint(j).acceptVisitor(prover);
      feasible_ = prover.isFeasible();
   }
   else
   {
      feasible_ = false;
   }
}

void ConstraintProver::apply(const ConstraintEq *c)
{
   ConstraintEq *ptr = const_cast<ConstraintEq *>(c);
   Interval l = ptr->left().eval(*B_), r = ptr->right().eval(*B_), d = l - r;
   if (d.isEmpty())
   {
      feasible_ = false;
   }
   else
   {
      feasible_ = ((d.left() >= -tol_) && (d.right() <= tol_));
   }
}

void ConstraintProver::apply(const ConstraintLe *c)
{
   ConstraintLe *ptr = const_cast<ConstraintLe *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

void ConstraintProver::apply(const ConstraintLt *c)
{
   ConstraintLt *ptr = const_cast<ConstraintLt *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

void ConstraintProver::apply(const ConstraintGe *c)
{
   ConstraintGe *ptr = const_cast<ConstraintGe *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

void ConstraintProver::apply(const ConstraintGt *c)
{
   ConstraintGt *ptr = const_cast<ConstraintGt *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

void ConstraintProver::apply(const ConstraintIn *c)
{
   ConstraintIn *ptr = const_cast<ConstraintIn *>(c);
   feasible_ = (ptr->isSatisfied(*B_) == Proof::Inner);
}

} // namespace realpaver
