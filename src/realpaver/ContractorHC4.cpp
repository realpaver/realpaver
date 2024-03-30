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
 * @file   ContractorHC4.cpp
 * @brief  HC4 contractor
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorHC4.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorHC4::ContractorHC4(SharedDag dag)
      : dag_(dag)
{
   context_ = dag_->cloneDefaultContext();

   SharedContractorVector pool = std::make_shared<ContractorVector>();

   for (size_t i=0; i<dag_->nbFuns(); ++i)
   {
      SharedContractor op = std::make_shared<SharedHC4Revise>(dag_, i);
      pool->push(op);
   }

   propag_ = new PropagationAlg(pool);
}

ContractorHC4::~ContractorHC4()
{
   if (propag_ != nullptr) delete propag_;
   if (context_ != nullptr) delete context_;
}

Scope ContractorHC4::scope() const
{
   return dag_->scope();
}

Proof ContractorHC4::contract(IntervalBox& B)
{
   context_->reset();
   dag_->bindContext(context_);

   Proof proof = propag_->contract(B);

   context_ = dag_->unbindContext();

   return proof;   
}

void ContractorHC4::print(std::ostream& os) const
{
   os << "HC4";
}

Tolerance ContractorHC4::getTol() const
{
   return propag_->getTol();
}

void ContractorHC4::setTol(Tolerance tol)
{
   propag_->setTol(tol);
}

size_t ContractorHC4::getMaxIter() const
{
   return propag_->getMaxIter();
}

void ContractorHC4::setMaxIter(size_t n)
{
   propag_->setMaxIter(n);
}

void ContractorHC4::push(SharedContractor op)
{
   propag_->push(op);
}

/*----------------------------------------------------------------------------*/

ContractorHC4::SharedHC4Revise::SharedHC4Revise(SharedDag dag, size_t i)
      : dag_(dag), idx_(i)
{
   ASSERT(i < dag_->nbFuns(), "");
}

Scope ContractorHC4::SharedHC4Revise::scope() const
{
   return dag_->fun(idx_)->scope();
}

Proof ContractorHC4::SharedHC4Revise::contract(IntervalBox& B)
{
   LOG_LOW("SharedHC4Revise contractor @ " << idx_ << " on " << B);

   Proof proof = dag_->fun(idx_)->sharedHc4Revise(B);

   LOG_LOW(" -> " << proof << ", " << B);

   return proof;
}

void ContractorHC4::SharedHC4Revise::print(std::ostream& os) const
{
    os << "SharedHC4Revise contractor / function " << " @ " << idx_;
}

} // namespace
