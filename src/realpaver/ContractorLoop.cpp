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
 * @file   ContractorLoop.cpp
 * @brief. Loop on a contractor
 * @author Laurent Granvilliers
 * @date   4-3-2025
 */
#include "realpaver/ContractorLoop.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

ContractorLoop::ContractorLoop(SharedContractor op)
    : op_(op)
    , tol_(Param::GetDblParam("LOOP_CONTRACTOR_TOL"))
{
   ASSERT(op != nullptr, "Bad operator in the loop contractor");
}

double ContractorLoop::getTol() const
{
   return tol_;
}

void ContractorLoop::setTol(double tol)
{
   ASSERT(tol >= 0.0 && tol <= 1.0, "A relative tolerance must be in [0, 1]");
   tol_ = tol;
}

Scope ContractorLoop::scope() const
{
   return op_->scope();
}

Proof ContractorLoop::contract(IntervalBox &B)
{
   ASSERT(B.scope().contains(op_->scope()), "Bad scopes in the loop contractor");

   bool iter = true;
   IntervalBox prev(B);
   while (iter)
   {
      if (op_->contract(B) == Proof::Empty)
         return Proof::Empty;
      if (!B.improves(prev, tol_))
         iter = false;
      else
         prev.setOnScope(B, scope());
   }
   return Proof::Maybe;
}

void ContractorLoop::print(std::ostream &os) const
{
   os << "Loop contractor on ";
   op_->print(os);
}

} // namespace realpaver
