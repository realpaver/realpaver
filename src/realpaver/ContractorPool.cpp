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
 * @file   ContractorPool.cpp
 * @brief  Classes of pools of contractoirs
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#include "realpaver/AssertDebug.hpp"
#include "realpaver/ContractorPool.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ContractorPool::ContractorPool()
      : v_(),
        scop_()
{}

size_t ContractorPool::poolSize() const
{
   return v_.size();
}

Scope ContractorPool::scope() const
{
   return scop_;
}

void ContractorPool::push(SharedContractor op)
{
   ASSERT(op != nullptr, "Bad insertion in a vector of contractors");

   v_.push_back(op);
   scop_.insert(op->scope());
   scop_ = ScopeBank::getInstance()->insertScope(scop_);
   makeDep();
}

SharedContractor ContractorPool::contractorAt(size_t i) const
{
   ASSERT(i < v_.size(),
          "Access out of range in a vector of contractors @ " << i);

   return v_[i];
}

void ContractorPool::makeDep()
{
   dep_.clear();
   dep_.resize(scop_.size());

   for (size_t i=0; i<scop_.size(); ++i)
   {
      Variable v = scop_.var(i);
      
      for (size_t j=0; j<v_.size(); ++j)
      {
         if (v_[j]->dependsOn(v))
         {
            dep_[i].push_back(j);
         }
      }
   }
}

size_t ContractorPool::depSize(const Variable& v) const
{
   return dep_[scop_.index(v)].size();
}

size_t ContractorPool::depAt(const Variable& v, size_t i) const
{
   return dep_[scop_.index(v)][i];
}

void ContractorPool::print(std::ostream& os) const
{
   os << "Pool of " << v_.size() << " contractors" << std::endl;
   for (size_t i=0; i<scop_.size(); ++i)
   {
      Variable v = scop_.var(i);
      os << v.getName() << ": ";
      
      for (size_t j=0; j<dep_[i].size(); ++j)
      {
         os << dep_[i][j] << " ";
      }
      os << std::endl;
   }
}

} // namespace
