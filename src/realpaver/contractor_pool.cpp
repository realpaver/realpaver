// This file is part of Realpaver. License: see COPYING file.

#include "realpaver/AssertDebug.hpp"
#include "realpaver/common.hpp"
#include "realpaver/contractor_pool.hpp"

namespace realpaver {

ContractorPool::~ContractorPool()
{}

///////////////////////////////////////////////////////////////////////////////

ContractorVector::ContractorVector() : ContractorPool(), v_(), s_(), bs_()
{}

void ContractorVector::push(const SharedContractor& op)
{
   v_.push_back(op);
   s_.insert(op->scope());
   bs_ = s_.toBitset();
}

bool ContractorVector::dependsOn(const Bitset& bs) const
{
   return bs_.overlaps(bs);
}

SharedContractor ContractorVector::contractorAt(size_t i) const
{
   ASSERT(i < v_.size(),
          "access out of range in a vector of contractors @ " << i);

   return v_[i];
}

void ContractorVector::removeContractorAt(size_t i)
{
   ASSERT(i < v_.size(),
          "access out of range in a vector of contractors @ " << i);

   s_.remove(v_[i]->scope());
   bs_ = s_.toBitset();
   v_.erase(v_.begin() + i);
}

} // namespace
