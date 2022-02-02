// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_PROPAGATOR_HPP
#define REALPAVER_PROPAGATOR_HPP

#include "realpaver/contractor_pool.hpp"

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
   Proof contract(IntervalVector& X);
   void print(std::ostream& os) const;

   // improvement factor
   IntervalImprovement improvement() const;
   void setImprovement(const IntervalImprovement& imp);

   // maximum number of propagation steps
   size_t maxSteps() const;
   void setMaxSteps(size_t n);

   // access to the proof certificates after a contraction
   size_t proofSize() const;
   Proof proofAt(size_t i) const;

private:
   ContractorPool* pool_;        // pool of contractors
   IntervalImprovement imp_;             // improvement factor
   size_t max_steps_;            // maximum number of propagation steps
   std::vector<Proof> certif_;   // proof certificates of contractors
                                 // after contract
};

inline IntervalImprovement Propagator::improvement() const
{
   return imp_;
}

inline void Propagator::setImprovement(const IntervalImprovement& imp)
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
