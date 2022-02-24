///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Propagator.hpp"

namespace realpaver {

Propagator::Propagator(ContractorPool* pool)
      : Contractor(),
        pool_(pool),
        dtol_(Param::getTolParam("PROPAGATOR_DTOL")),
        maxiter_(Param::getIntParam("PROPAGATOR_ITER_LIMIT")),
        certif_()
{}

Tolerance Propagator::getDistTol() const
{
   return dtol_;
}

void Propagator::setDistTol(Tolerance tol)
{
   dtol_ = tol;
}

size_t Propagator::poolSize() const
{
   return pool_->poolSize();
}

size_t Propagator::getMaxIterations() const
{
   return maxiter_;
}

void Propagator::setMaxIterations(size_t n)
{
   maxiter_ = n;
}

Proof Propagator::proofAt(size_t i) const
{
   return certif_[i];
}

ContractorPool* Propagator::getPool() const
{
   return pool_;
}

void Propagator::setPool(ContractorPool* pool)
{
   pool_ = pool;
}

bool Propagator::dependsOn(const Bitset& bs) const
{
   return pool_->dependsOn(bs);
}

Scope Propagator::scope() const
{
   return pool_->scope();
}

Proof Propagator::contract(IntervalVector& X)
{
   ASSERT(pool_ != nullptr, "No pool is assigned in a propagator");

   Scope scope = pool_->scope();

   // initialization: activates all contractors
   size_t N = pool_->poolSize();

   // propagation queue
   std::vector<size_t> queue(N);
   for (size_t i=0; i<N; ++i) queue[i] = i;

   // number of active contractors
   size_t count = N;

   // vector of proof certificates
   certif_.resize(N);

   // bitset used to check the domain modifications
   Bitset modified(1 + scope.maxIndex());

   // copy used to check the domain modifications
   IntervalVector* copy = X.clone();

   // result of the algorithm
   Proof proof;

   // index of the next contractor to be applied from the queue
   size_t next = 0;

   // number of propagation steps
   size_t num_steps = 0;

   do
   {
      // apply the next contractor from the queue
      size_t j = queue[next];
      proof = pool_->contractorAt(j)->contract(X);
      certif_[j] = proof;

      if (proof != Proof::Empty)
      {
         ++next;

         // propagation when the queue is empty
         if (next == count)
         {
            if (++num_steps > maxiter_)
            {
               count = 0;
            }
            else
            {
               // detects the variables whose domains have been modified
               bool isModified = false;
               modified.setAllZero();

               for (auto v : scope)
               {
                  size_t iv = v.getId();
                  const Interval& prev = copy->at(iv);
                  const Interval& curr = X.at(iv);
                  
                  if (!dtol_.hasTolerance(prev, curr))
                  {
                     isModified = true;
                     modified.setOne(v.getId());
                  }
               }

               // activates all the contractors depending on a modified
               // variable
               next = count = 0;

               if (isModified)
               {
                  for (size_t i=0; i<N; ++i)
                     if (certif_[i] != Proof::Inner &&
                         pool_->contractorAt(i)->dependsOn(modified))
                     {
                        queue[count] = i;
                        ++count;
                     }

                  // save the current box for the next propagation step
                  if (count != 0)
                  {
                     copy->setOnScope(X, scope);
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

   return proof;
}

void Propagator::print(std::ostream& os) const
{
   os << "Propagator";
}

} // namespace