///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPool.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ContractorPool::~ContractorPool()
{}

///////////////////////////////////////////////////////////////////////////////

ContractorVector::ContractorVector()
      : ContractorPool(),
        v_(),
        scop_()
{}

size_t ContractorVector::poolSize() const
{
   return v_.size();
}

Scope ContractorVector::scope() const
{
   return scop_;
}

void ContractorVector::push(SharedContractor op)
{
   ASSERT(op != nullptr, "Bad insertion in a vector of contractors");

   v_.push_back(op);
   scop_.insert(op->scope());
   scop_ = ScopeBank::getInstance()->insertScope(scop_);
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

   scop_.remove(v_[i]->scope());
   v_.erase(v_.begin() + i);
}

} // namespace
