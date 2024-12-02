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
 * @file   IntervalPropagator.cpp
 * @brief  Constraint propagation over interval contractors
 * @author Laurent Granvilliers
 * @date   2024-7-1
*/

#include <queue>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Bitset.hpp"
#include "realpaver/IntervalPropagator.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

namespace realpaver {

IntervalPropagator::IntervalPropagator(SharedContractorPool pool)
      : Contractor(),
        pool_(pool),
        tol_(Param::GetDblParam("PROPAGATION_REL_TOL"), 0.0),
        certif_()
{
   if (pool == nullptr)
   {
      pool_ = std::make_shared<ContractorPool>();
   }
}

Tolerance IntervalPropagator::getTol() const
{
   return tol_;
}

void IntervalPropagator::setTol(Tolerance tol)
{
   tol_ = tol;
}

size_t IntervalPropagator::poolSize() const
{
   return pool_->poolSize();
}

void IntervalPropagator::push(SharedContractor op)
{
   pool_->push(op);
}

Proof IntervalPropagator::proofAt(size_t i) const
{
   return certif_[i];
}

SharedContractorPool IntervalPropagator::getPool() const
{
   return pool_;
}

void IntervalPropagator::setPool(SharedContractorPool pool)
{
   pool_ = pool;
}

Scope IntervalPropagator::scope() const
{
   return pool_->scope();
}

Proof IntervalPropagator::contract(IntervalBox& B)
{
   ASSERT(pool_ != nullptr, "No pool is assigned in a propagator");

   // initialization: activates all contractors
   size_t N = pool_->poolSize();

   // propagation queue
   std::queue<size_t> queue;
   for (size_t i=0; i<N; ++i) queue.push(i);

   // vector of proof certificates
   certif_.resize(N);

   Bitset active(N);
   active.setAllOne();

   Proof proof = Proof::Maybe;
   IntervalBox copy(B);

   while ((proof != Proof::Empty) && (queue.size() > 0))
   {
      size_t j = queue.front();
      queue.pop();
      SharedContractor op = pool_->contractorAt(j);

      copy.setOnScope(B, op->scope());

      proof = op->contract(B);
      certif_[j] = proof;
      active.setZero(j);

      if (proof != Proof::Empty)
      {
         for (const auto& v : op->scope())
         {
            const Interval& prev = copy.get(v);
            const Interval& curr = B.get(v);

            LOG_LOW("Propagation test on " << v.getName() << " ("
                                           << tol_ << ")");

            if (tol_.isImproved(prev, curr))
            {
               LOG_LOW("  " << prev << " -> " << curr << " reduced enough"
                            << " -> propagation");

               // considers the dependency relation of v
               for (size_t k=0; k<pool_->depSize(v); ++k)
               {
                  size_t l = pool_->depAt(v, k);
                  if (!active.get(l) && (l != j))
                  {
                     queue.push(l);
                     active.setOne(l);
                  }
               }
            }
         }
      }
   }
   return proof;
}

void IntervalPropagator::print(std::ostream& os) const
{
   os << "IntervalPropagator on " << pool_->poolSize() << " contractors";
}

} // namespace
