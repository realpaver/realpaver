// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_CONTRACTOR_POOL_HPP
#define REALPAVER_CONTRACTOR_POOL_HPP

#include "realpaver/contractor.hpp"

namespace realpaver {

/*****************************************************************************
 * Abstract class of pools, i.e. set of contractors.
 *****************************************************************************/
class ContractorPool {
public:
   // destructor
   virtual ~ContractorPool();

   // number of contractors
   virtual size_t poolSize() const = 0;

   // set of variables
   virtual Scope scope() const = 0;

   // returns true if a contractor in this pool depends on bs
   virtual bool dependsOn(const Bitset& bs) const = 0;

   // returns the i-th contractor
   virtual SharedContractor contractorAt(size_t i) const = 0;

   // removes the i-th contractor
   virtual void removeContractorAt(size_t i) = 0;
};

/*****************************************************************************
 * Vector of contractors.
 *****************************************************************************/
class ContractorVector : public ContractorPool {
public:
   // constructor
   ContractorVector();

   // insertion at the end
   // this pool does not own the operator
   void push(const SharedContractor& op);

   // Override
   size_t poolSize() const;
   Scope scope() const;
   bool dependsOn(const Bitset& bs) const;
   SharedContractor contractorAt(size_t i) const;
   void removeContractorAt(size_t i);

private:
   std::vector<SharedContractor> v_;
   Scope s_;
   Bitset bs_;
};

inline size_t ContractorVector::poolSize() const
{
   return v_.size();
}

inline Scope ContractorVector::scope() const
{
   return s_;
}

} // namespace

#endif
