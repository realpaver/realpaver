///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

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
