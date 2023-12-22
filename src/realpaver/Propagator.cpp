///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include <unordered_set>
#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/Param.hpp"
#include "realpaver/Propagator.hpp"

namespace realpaver {

Propagator::Propagator(SharedContractorPool pool)
      : Contractor(),
        pool_(pool),
        tol_(Param::GetDblParam("PROPAGATION_REL_TOL"),
             Param::GetDblParam("PROPAGATION_ABS_TOL")),
        maxiter_(Param::GetIntParam("PROPAGATION_ITER_LIMIT")),
        certif_()
{}

Tolerance Propagator::getTol() const
{
   return tol_;
}

void Propagator::setTol(Tolerance tol)
{
   tol_ = tol;
}

size_t Propagator::poolSize() const
{
   return pool_->poolSize();
}

size_t Propagator::getMaxIter() const
{
   return maxiter_;
}

void Propagator::setMaxIter(size_t n)
{
   maxiter_ = n;
}

Proof Propagator::proofAt(size_t i) const
{
   return certif_[i];
}

SharedContractorPool Propagator::getPool() const
{
   return pool_;
}

void Propagator::setPool(SharedContractorPool pool)
{
   pool_ = pool;
}

Scope Propagator::scope() const
{
   return pool_->scope();
}

Proof Propagator::contract(IntervalBox& box)
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

   // Set of variables used to check the domain modifications
   ModifSetType modif;

   // copy used to check the domain modifications
   IntervalBox* copy = box.clone();

   // result of the algorithm
   Proof proof;

   // index of the next contractor to be applied from the queue
   size_t next = 0;

   // number of propagation steps
   size_t nb_steps = 0;

   LOG_NL();
   LOG_INTER("Propagator [" << tol_ << "]");
   LOG_INTER("Current box: " << box);

   do
   {
      // apply the next contractor from the queue
      size_t j = queue[next];
      proof = pool_->contractorAt(j)->contract(box);
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

               for (auto v : scope)
               {
                  const Interval& prev = copy->get(v);
                  const Interval& curr = box.get(v);

                  LOG_LOW("Propagation test on " << v.getName() << " ("
                                                 << tol_ << ")");

                  if (!tol_.areClose(prev, curr))
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
                     copy->setOnScope(box, scope);
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

   LOG_INTER(" -> " << proof << ", " << box);
   LOG_INTER("End of propagator, " << nb_steps << " loop(s)");

   return proof;
}

void Propagator::print(std::ostream& os) const
{
   os << "Propagator on " << pool_->poolSize() << " contractors";
}

bool Propagator::contractorDependsOn(size_t i, const ModifSetType& ms)
{
   SharedContractor op = pool_->contractorAt(i);

   for (const auto& v : ms)
      if (op->dependsOn(v)) return true;

   return false;
}

} // namespace
