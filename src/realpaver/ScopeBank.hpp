///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_SCOPE_BANK_HPP
#define REALPAVER_SCOPE_BANK_HPP

#include <unordered_set>
#include "realpaver/Scope.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a bank of scopes in order to share them.
///
/// This class implements the Singleton pattern.
///////////////////////////////////////////////////////////////////////////////
class ScopeBank {
public:
   /// @return the instance
   static ScopeBank* getInstance();

   /// @return the number of scopes in this
   size_t size() const;

   /// @return the number of scopes that have been inserted in this
   size_t nbInsertions() const;

   /// Inserts a scope in this
   /// @param scop a scope
   /// @return if there is a scope S in this that is equal to scop then S is
   ///         returned, scop is returned otherwise
   Scope insertScope(Scope scop);

private:
   struct ScopeHasher {
      std::size_t operator()(const Scope& scop) const
      {
         return scop.hashCode();
      }
   };
   typedef std::unordered_set<Scope, ScopeHasher> SetType;

   SetType bank_;    // the set of scopes
   size_t nb_;       // the number of scopes that have been inserted in this

   static ScopeBank instance_;

   ScopeBank();
   ~ScopeBank() = default;
   ScopeBank(ScopeBank&) = delete;
   ScopeBank& operator=(ScopeBank&) = delete;
};

} // namespace

#endif
