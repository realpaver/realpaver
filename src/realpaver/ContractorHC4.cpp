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
 * @date   2024-4-11
 */

#include "realpaver/ContractorHC4.hpp"
#include "realpaver/ContractorHC4Revise.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorHC4::ContractorHC4(SharedDag dag)
    : dag_(dag)
{
   SharedContractorPool pool = std::make_shared<ContractorPool>();

   for (size_t i = 0; i < dag_->nbFuns(); ++i)
   {
      SharedContractor op = std::make_shared<ContractorHC4Revise>(dag_, i);
      pool->push(op);
   }

   propag_ = new IntervalPropagator(pool);
}

ContractorHC4::~ContractorHC4()
{
   if (propag_ != nullptr)
      delete propag_;
}

Scope ContractorHC4::scope() const
{
   return dag_->scope();
}

Proof ContractorHC4::contract(IntervalBox &B)
{
   LOG_INTER("HC4");
   return propag_->contract(B);
}

void ContractorHC4::print(std::ostream &os) const
{
   os << "HC4";
}

double ContractorHC4::getTol() const
{
   return propag_->getTol();
}

void ContractorHC4::setTol(double tol)
{
   propag_->setTol(tol);
}

void ContractorHC4::push(SharedContractor op)
{
   propag_->push(op);
}

} // namespace realpaver
