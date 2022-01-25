// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/param.hpp"
#include "realpaver/propagator.hpp"

namespace realpaver {

Propagator::Propagator(ContractorPool* pool) :
   Contractor(),
   pool_(pool),
   imp_(Param::DefPropagatorImprovement()),
   max_steps_(Param::DefPropagatorMaxSteps()),
   certif_()
{}

bool Propagator::dependsOn(const Bitset& bs) const
{
   return pool_->dependsOn(bs);
}

Scope Propagator::scope() const
{
   return pool_->scope();
}

Proof Propagator::contract(Box& B)
{
   ASSERT(pool_ != nullptr, "null pool pointer in a propagator");

   Scope scope = pool_->scope();

   // initialization: activates all contractors
   size_t N = pool_->poolSize();

   // propagation queue
   std::vector<size_t> queue(N);
   for (size_t i=0; i<N; ++i)
      queue[i] = i;

   // number of active contractors
   size_t count = N;

   // vector of proof certificates
   certif_.resize(N);

   // bitset used to check the domain modifications
   Bitset modified(1 + scope.maxIndex());

   // copy used to check the domain modifications
   Box* copy = B.clone();

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
      proof = pool_->contractorAt(j)->contract(B);
      certif_[j] = proof;

      if (proof != Proof::Empty)
      {
         ++next;

         // propagation when the queue is empty
         if (next == count)
         {
            if (++num_steps > max_steps_)
            {
               count = 0;
            }
            else
            {
               // detects the variables whose domains have been modified
               bool isModified = false;
               modified.setAllZero();

               for (auto v : scope)
                  if (imp_.test(B.operator[](v.id()), copy->operator[](v.id())))
                  {
                     isModified = true;
                     modified.setOne(v.id());
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
                     copy->set(B, scope);
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
