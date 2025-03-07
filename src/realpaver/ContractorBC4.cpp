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
 * @file   ContractorBC4.cpp
 * @brief  BC4 contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/ContractorBC4.hpp"
#include "realpaver/Logger.hpp"

namespace realpaver {

ContractorBC4::ContractorBC4(SharedDag dag)
      : dag_(dag),
        vop_()
{
   SharedContractorPool pool = std::make_shared<ContractorPool>();

   for (size_t i=0; i<dag_->nbFuns(); ++i)
   {
      std::shared_ptr<ContractorBC4Revise>
         op = std::make_shared<ContractorBC4Revise>(dag_, i);
      pool->push(op);
      vop_.push_back(op);
   }

   propag_ = new IntervalPropagator(pool);
}

ContractorBC4::~ContractorBC4()
{
   if (propag_ != nullptr) delete propag_;
}

Scope ContractorBC4::scope() const
{
   return dag_->scope();
}

Proof ContractorBC4::contract(IntervalBox& B)
{
   LOG_INTER("BC4");
   return propag_->contract(B);
}

void ContractorBC4::print(std::ostream& os) const
{
   os << "BC4";
}

void ContractorBC4::setBC4RevisePeelFactor(double f)
{
   for (auto& op : vop_)
      op->setPeelFactor(f);
}

void ContractorBC4::setBC4ReviseMaxIter(size_t val)
{
   for (auto& op : vop_)
      op->setMaxIter(val);
}

double ContractorBC4::getTol() const
{
   return propag_->getTol();
}

void ContractorBC4::setTol(double tol)
{
   propag_->setTol(tol);
}

void ContractorBC4::push(SharedContractor op)
{
   propag_->push(op);
}

} // namespace
