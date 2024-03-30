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
 * @file   Contractor3BCID.cpp
 * @brief  3BCID contractor
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorVar3BCID.hpp"

namespace realpaver {


ContractorVar3BCID::ContractorVar3BCID(SharedContractor op, Variable v,
                                       std::unique_ptr<IntervalSlicer> slicer3B,
                                       std::unique_ptr<IntervalSlicer> slicerCID)
      : v_(v),
        ctc3B_(nullptr),
        ctcCID_(nullptr)
{
   ctc3B_ = new ContractorVar3B(op, v, std::move(slicer3B));
   ctcCID_ = new ContractorVarCID(op, v, std::move(slicerCID));
}

ContractorVar3BCID::ContractorVar3BCID(SharedContractor op, Variable v,
                                       size_t n3B, size_t nCID)
      : v_(v),
        ctc3B_(nullptr),
        ctcCID_(nullptr)
{
   ctc3B_ = new ContractorVar3B(op, v, n3B);
   ctcCID_ = new ContractorVarCID(op, v, nCID);
}

ContractorVar3BCID::~ContractorVar3BCID()
{
   delete ctc3B_;
   delete ctcCID_;
}

Variable ContractorVar3BCID::getVar() const
{
   return v_;
}

void ContractorVar3BCID::setVar(Variable v)
{
   ASSERT(scope().contains(v),
          "Bad variable " << v << " in a var3BCID contractor");

   v_ = v;
}

Scope ContractorVar3BCID::scope() const
{
   return ctc3B_->scope();
}

Proof ContractorVar3BCID::contract(IntervalBox& B)
{
   Interval dom = B.get(v_);

   Proof proof = ctc3B_->contract(B);

   if (proof == Proof::Empty)
      return Proof::Empty;

   if (dom.isSetEq(B.get(v_)))
      return Proof::Maybe;

   return ctcCID_->contract(B);
}

void ContractorVar3BCID::print(std::ostream& os) const
{
   os << "var3BCID contractor on " << v_.getName();
}
   
} // namespace
