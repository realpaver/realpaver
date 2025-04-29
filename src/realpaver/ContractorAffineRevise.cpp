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
 * @file   ContractorAffineRevise.cpp
 * @brief  Contractor based on affine forms
 * @author Laurent Granvilliers
 * @date   21 Apr 2025
 */

#include "realpaver/AffineCreator.hpp"
#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorAffineRevise.hpp"

namespace realpaver {

ContractorAffineRevise::ContractorAffineRevise(SharedDag dag, size_t i, bool minrange)
    : Contractor()
    , dag_(dag)
    , idx_(i)
    , minrange_(minrange)
{
   ASSERT(dag != nullptr, "Creation of affine contractor from a null pointer");
   ASSERT(i < dag->nbFuns(),
          "Creation of affine contractor given a bad " << " function index " << i);
}

Scope ContractorAffineRevise::scope() const
{
   return dag_->fun(idx_)->scope();
}

Proof ContractorAffineRevise::contract(IntervalBox &B)
{
   AffineCreator creator(dag_, idx_, minrange_);
   creator.create(B);
   const AffineForm &f = creator.fun(0);

   Interval I = dag_->fun(idx_)->getImage();
   Interval E = f.errorTerm();
   Interval J = I - f.constantTerm() + Interval(-E.right(), E.right());

   if (J.isUniverse())
      return Proof::Maybe;

   // sum of absolute values of coefficients of the affine form
   double s = 0.0;
   Double::rndUp();
   for (auto it = f.cbegin(); it != f.cend(); ++it)
      s += abs(it->itv).right();

   // satisfaction test
   Interval K(-s, s);
   if (J.contains(K))
      return Proof::Inner;
   else if (J.isDisjoint(K))
      return Proof::Empty;

   // reduction of domains
   for (auto it = f.cbegin(); it != f.cend(); ++it)
   {
      const Variable &v = dag_->scope().var(it->var);

      // new domain of the variable ei associated with v
      Double::rndUp();
      double si = s - abs(it->itv).right();
      Interval domi = Interval::minusOnePlusOne() & ((J + Interval(-si, si)) / it->itv);

      // new domain of v
      const AffineForm &g = creator.fun(v);
      Interval domv = g.constantTerm() + g.cbegin()->itv * domi;
      domv &= B.get(v);
      if (domv.isEmpty())
         return Proof::Empty;
      else
         B.set(v, domv);
   }

   return Proof::Maybe;
}

void ContractorAffineRevise::print(std::ostream &os) const
{
   os << "Affine contractor / function " << " @ " << idx_;
}

SharedDag ContractorAffineRevise::getDag() const
{
   return dag_;
}

size_t ContractorAffineRevise::getFunIndex() const
{
   return idx_;
}

void ContractorAffineRevise::useMinrange(bool minrange)
{
   minrange_ = minrange;
}

} // namespace realpaver
