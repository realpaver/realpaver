// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PROPAGATOR_HPP
#define REALPAVER_PROPAGATOR_HPP

#include "realpaver/contractor_pool.hpp"
#include "realpaver/stopping.hpp"

namespace realpaver {

/*****************************************************************************
 * Class of propagators implementing an AC3-like  constraint propagation
 * algorithm over a set of contractors.
 *****************************************************************************/
class Propagator : public Contractor {
public:
   // constructor
   // this propagator does not own the pool
   Propagator(ContractorPool* pool = nullptr);

   // management of the pool
   ContractorPool* getPool() const;
   void setPool(ContractorPool* pool);

   // Override
   bool dependsOn(const Bitset& bs) const;
   Scope scope() const;
   Proof contract(Box& B);
   void print(std::ostream& os) const;

   // improvement factor
   Improvement improvement() const;
   void setImprovement(const Improvement& imp);

   // maximum number of propagation steps
   size_t maxSteps() const;
   void setMaxSteps(size_t n);

   // access to the proof certificates after a contraction
   size_t proofSize() const;
   Proof proofAt(size_t i) const;

private:
   ContractorPool* pool_;        // pool of contractors
   Improvement imp_;             // improvement factor
   size_t max_steps_;            // maximum number of propagation steps
   std::vector<Proof> certif_;   // proof certificates of contractors
                                 // after contract
};

inline Improvement Propagator::improvement() const
{
   return imp_;
}

inline void Propagator::setImprovement(const Improvement& imp)
{
   imp_ = imp;
}

inline size_t Propagator::maxSteps() const
{
   return max_steps_;
}

inline void Propagator::setMaxSteps(size_t n)
{
   max_steps_ = n;
}

inline size_t Propagator::proofSize() const
{
   return certif_.size();
}

inline Proof Propagator::proofAt(size_t i) const
{
   return certif_[i];
}

inline ContractorPool* Propagator::getPool() const
{
   return pool_;
}

inline void Propagator::setPool(ContractorPool* pool)
{
   pool_ = pool;
}

} // namespace

#endif
