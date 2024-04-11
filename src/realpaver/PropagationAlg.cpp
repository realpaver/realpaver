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
 * @file   PropagationAlg.cpp
 * @brief  Constraint propagation algorithm
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#include <unordered_set>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/PropagationAlg.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"

#include <queue>
#include "realpaver/Bitset.hpp"

namespace realpaver {

PropagationAlg::PropagationAlg(SharedContractorPool pool)
      : Contractor(),
        pool_(pool),
        tol_(Param::GetDblParam("PROPAGATION_REL_TOL"), 0.0),
        maxiter_(Param::GetIntParam("PROPAGATION_ITER_LIMIT")),
        certif_()
{
   if (pool == nullptr)
   {
      pool_ = std::make_shared<ContractorVector>();
   }
}

Tolerance PropagationAlg::getTol() const
{
   return tol_;
}

void PropagationAlg::setTol(Tolerance tol)
{
   tol_ = tol;
}

size_t PropagationAlg::poolSize() const
{
   return pool_->poolSize();
}

void PropagationAlg::push(SharedContractor op)
{
   pool_->push(op);
}

size_t PropagationAlg::getMaxIter() const
{
   return maxiter_;
}

void PropagationAlg::setMaxIter(size_t n)
{
   maxiter_ = n;
}

Proof PropagationAlg::proofAt(size_t i) const
{
   return certif_[i];
}

SharedContractorPool PropagationAlg::getPool() const
{
   return pool_;
}

void PropagationAlg::setPool(SharedContractorPool pool)
{
   pool_ = pool;
}

Scope PropagationAlg::scope() const
{
   return pool_->scope();
}

Proof PropagationAlg::contractBis(IntervalBox& B)
{
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

               for (size_t i=0; i<N; ++i)
               {
                  if (!active.get(i))
                  {
                     SharedContractor ctc = pool_->contractorAt(i);
                     if (ctc->dependsOn(v))
                     {
                        queue.push(i);
                        active.setOne(i);
                     }
                  }
               }
            }
            else
            {
               LOG_LOW("  " << prev << " -> " << curr
                            << " not reduced enough");
            }
         }
      }
   }
   return proof;
}

Proof PropagationAlg::contract(IntervalBox& B)
{
   ASSERT(pool_ != nullptr, "No pool is assigned in a propagator");

   Scope scop = pool_->scope();

   // initialization: activates all contractors
   size_t N = pool_->poolSize();

   // propagation queue
   std::vector<size_t> queue(N);
   for (size_t i=0; i<N; ++i) queue[i] = i;

   // number of active contractors
   size_t count = N;

   // vector of proof certificates
   certif_.resize(N);

   // Set of variables used to check the domain modifications
   ModifSetType modif;

   // copy used to check the domain modifications
   IntervalBox* copy = B.clone();

   // result of the algorithm
   Proof proof;

   // index of the next contractor to be applied from the queue
   size_t next = 0;

   // number of propagation steps
   size_t nb_steps = 0;

   LOG_INTER("Propagation algorithm on " << B);

   do
   {
      // apply the next contractor from the queue
      size_t j = queue[next];
      proof = pool_->contractorAt(j)->contract(B);
      certif_[j] = proof;

      if (proof != Proof::Empty)
      {
         ++next;

         // propagation when the queue is empty
         if (next == count)
         {                 
            if (++nb_steps > maxiter_)
            {
               count = 0;
               LOG_INTER("Stops on maxiter: " << maxiter_);
            }
            else
            {
               // detects the variables whose domains have been modified
               modif.clear();

               for (const auto& v : scop)
               {
                  const Interval& prev = copy->get(v);
                  const Interval& curr = B.get(v);

                  LOG_LOW("Propagation test on " << v.getName() << " ("
                                                 << tol_ << ")");

                  if (tol_.isImproved(prev, curr))
                  {
                     LOG_LOW("  " << prev << " -> " << curr << " reduced enough"
                                  << " -> propagation");

                     modif.insert(v);
                  }
                  else
                  {
                     LOG_LOW("  " << prev << " -> " << curr
                                  << " not reduced enough");
                  }
               }

               // activates all the contractors depending on a modified
               // variable
               next = count = 0;

               if (modif.size() > 0)
               {
                  for (size_t i=0; i<N; ++i)
                     if (certif_[i] != Proof::Inner &&
                         contractorDependsOn(i, modif))
                     {
                        queue[count] = i;
                        ++count;
                     }

                  // save the current box for the next propagation step
                  if (count != 0)
                  {
                     copy->setOnScope(B, scop);
                  }
               }
            }
         }
      }
   }
   while (proof != Proof::Empty && count > 0);

   if (proof != Proof::Empty)
   {
      proof = certif_[0];
      for (size_t i=1; i<N; ++i)
         proof = std::min(proof,certif_[i]);
   }

   delete copy;

   LOG_INTER(" -> " << proof << ", " << B);
   LOG_INTER("End of propagation, " << nb_steps << " loop(s)");

   return proof;
}

void PropagationAlg::print(std::ostream& os) const
{
   os << "PropagationAlg on " << pool_->poolSize() << " contractors";
}

bool PropagationAlg::contractorDependsOn(size_t i, const ModifSetType& ms)
{
   SharedContractor op = pool_->contractorAt(i);

   for (const auto& v : ms)
      if (op->dependsOn(v)) return true;

   return false;
}

} // namespace

