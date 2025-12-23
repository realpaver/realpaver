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
 * @file   ProverOptim.cpp
 * @brief  Prover for optimization problems
 * @author Laurent Granvilliers
 * @date   30 Sep 2025
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ConstraintProver.hpp"
#include "realpaver/ProverOptim.hpp"

namespace realpaver {

ProverOptim::ProverOptim(const Problem &p, const double &tol)
    : ctrs_()
    , tol_(tol)
{
   REALPAVER_ASSERT(tol >= 0.0, "A tolerance used to relax constraints must be positive");

   for (size_t i = 0; i < p.nbCtrs(); ++i)
   {
      Constraint c = p.ctrAt(i);
      if (c.isInitial())
         ctrs_.push_back(c);
   }
}

bool ProverOptim::certify(IntervalBox &B)
{
   for (size_t i = 0; i < ctrs_.size(); ++i)
   {
      ConstraintProver cp(B, tol_);
      ctrs_[i].acceptVisitor(cp);
      if (!cp.isFeasible())
         return false;
   }
   return true;
}

bool ProverOptim::certify(IntervalBox &B, const Bitset &active)
{
   for (size_t i = 0; i < ctrs_.size(); ++i)
   {
      if (active.get(ctrs_[i].id()))
      {
         ConstraintProver cp(B, tol_);
         ctrs_[i].acceptVisitor(cp);
         if (!cp.isFeasible())
            return false;
      }
   }
   return true;
}

double ProverOptim::getTol() const
{
   return tol_;
}

void ProverOptim::setTol(const double &tol)
{
   REALPAVER_ASSERT(tol >= 0.0, "A tolerance used to relax constraints must be positive");
   tol_ = tol;
}

} // namespace realpaver
