///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPool.hpp"

namespace realpaver {

ContractorPool::~ContractorPool()
{}

///////////////////////////////////////////////////////////////////////////////

ContractorVector::ContractorVector()
      : ContractorPool(),
        v_(),
        s_(),
        bs_()
{}

size_t ContractorVector::poolSize() const
{
   return v_.size();
}

Scope ContractorVector::scope() const
{
   return s_;
}

void ContractorVector::push(const SharedContractor& op)
{
   ASSERT(op != nullptr, "Bad insertion in a vector of contractors");

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
          "Access out of range in a vector of contractors @ " << i);

   return v_[i];
}

void ContractorVector::removeContractorAt(size_t i)
{
   ASSERT(i < v_.size(),
          "Access out of range in a vector of contractors @ " << i);

   s_.remove(v_[i]->scope());
   bs_ = s_.toBitset();
   v_.erase(v_.begin() + i);
}

} // namespace
