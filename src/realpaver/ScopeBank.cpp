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
 * @file   ScopeBank.cpp
 * @brief  Bank of scopes
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#include "realpaver/AssertDebug.hpp"
#include "realpaver/Logger.hpp"
#include "realpaver/ScopeBank.hpp"

namespace realpaver {

ScopeBank ScopeBank::instance_;

ScopeBank::ScopeBank()
      : bank_(),
        nb_(0)
{}

size_t ScopeBank::size() const
{
   return bank_.size();
}

size_t ScopeBank::nbInsertions() const
{
   return nb_;
}

Scope ScopeBank::insertScope(Scope scop)
{
   ++nb_;
   auto it = bank_.find(scop);

   if (it != bank_.end())
      return *it;

   bank_.insert(scop);
   return scop;
}

ScopeBank* ScopeBank::getInstance()
{
   return &instance_;
}

} // namespace
