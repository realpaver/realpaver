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
 * @file   ContractorVar3B.cpp
 * @brief  3B contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVar3B.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

ContractorVar3B::ContractorVar3B(SharedContractor op, Variable v,
                                 std::unique_ptr<IntervalSlicer> slicer)
    : op_(op)
    , v_(v)
    , slicer_(std::move(slicer))
    , var_min_width_(Params::GetDblParam("VAR3BCID_MIN_WIDTH"))
{
   ASSERT(op_.get() != nullptr, "No operator in a var3B contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v << " in a var3B contractor");
   ASSERT(slicer_ != nullptr, "No slicer in a var3B contractor");
}

ContractorVar3B::ContractorVar3B(SharedContractor op, Variable v, size_t n)
    : op_(op)
    , v_(v)
    , slicer_(nullptr)
{
   ASSERT(op_.get() != nullptr, "No operator in a var3B contractor");
   ASSERT(op->scope().contains(v), "Bad variable " << v << " in a var3B contractor");
   ASSERT(n > 1, "Bad number of slices in a var3B contractor: " << n);

   slicer_ = std::make_unique<IntervalPartitionMaker>(n);
}

Scope ContractorVar3B::scope() const
{
   return op_->scope();
}

Variable ContractorVar3B::getVar() const
{
   return v_;
}

void ContractorVar3B::setVar(Variable v)
{
   ASSERT(scope().contains(v), "Bad variable " << v << " in a var3B contractor");

   v_ = v;
}

double ContractorVar3B::varMinWidth() const
{
   return var_min_width_;
}

void ContractorVar3B::setVarMinWidth(double val)
{
   var_min_width_ = val;
}

Proof ContractorVar3B::contract(IntervalBox &B)
{
   Interval dom = B.get(v_);

   // not handles too small domains
   if (dom.width() < var_min_width_)
      return Proof::Maybe;

   // not handles infinite domains
   if (dom.isInf())
      return Proof::Maybe;

   // slices the domain
   slicer_->apply(dom);
   size_t nbs = slicer_->nbSlices();

   if (nbs == 1)
      return op_->contract(B);

   // left to right
   size_t nbl = 0;             // number of left inconsistent facets
   auto it = slicer_->begin(); // iterator on the first slice
   bool iter = true;
   Interval lslice;

   while (iter)
   {
      IntervalBox *facet = B.clone();
      lslice = *it;
      facet->set(v_, lslice);

      Proof certif = op_->contract(*facet);

      if (certif == Proof::Empty)
      {
         ++nbl;
         ++it;
         if (nbl == nbs)
            iter = false;
      }
      else
      {
         lslice = facet->get(v_);
         iter = false;
      }

      delete facet;
   }

   if (nbl == nbs)
      return Proof::Empty;

   // right to left
   size_t nbr = 0; // number of right inconsistent facets
   auto jt = slicer_->rbegin();
   iter = true;
   Interval rslice;

   while (iter)
   {
      IntervalBox *facet = B.clone();
      rslice = *jt;
      facet->set(v_, rslice);

      Proof certif = op_->contract(*facet);

      if (certif == Proof::Empty)
      {
         ++nbr;
         ++jt;
         if (nbr == nbs - nbl - 1)
         {
            // only one consistent facet
            iter = false;
            rslice = lslice;
         }
      }
      else
      {
         rslice = facet->get(v_);
         iter = false;
      }

      delete facet;
   }

   B.set(v_, Interval(lslice.left(), rslice.right()));

   return Proof::Maybe;
}

void ContractorVar3B::print(std::ostream &os) const
{
   os << "var3B contractor on " << v_.getName();
}

} // namespace realpaver
