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
 * @file   ContractorAffine.cpp
 * @brief  Propagation algorithm based on affine contractors
 * @author Laurent Granvilliers
 * @date   23 Apr 2025
 */

#include "realpaver/ContractorAffine.hpp"
#include "realpaver/ContractorAffineRevise.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorAffine::ContractorAffine(SharedDag dag, bool minrange)
    : dag_(dag)
{
   SharedContractorPool pool = std::make_shared<ContractorPool>();

   for (size_t i = 0; i < dag_->nbFuns(); ++i)
   {
      SharedContractor op = std::make_shared<ContractorAffineRevise>(dag_, i, minrange);
      pool->push(op);
   }

   propag_ = new IntervalPropagator(pool);
}

ContractorAffine::~ContractorAffine()
{
   if (propag_ != nullptr)
      delete propag_;
}

Scope ContractorAffine::scope() const
{
   return dag_->scope();
}

Proof ContractorAffine::contract(IntervalBox &B)
{
   LOG_INTER("HC4");
   return propag_->contract(B);
}

void ContractorAffine::print(std::ostream &os) const
{
   os << "HC4";
}

double ContractorAffine::getTol() const
{
   return propag_->getTol();
}

void ContractorAffine::setTol(double tol)
{
   propag_->setTol(tol);
}

void ContractorAffine::push(SharedContractor op)
{
   propag_->push(op);
}

} // namespace realpaver
