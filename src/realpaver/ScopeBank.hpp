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
 * @file   ScopeBank.hpp
 * @brief  Bank of scopes
 * @author Laurent Granvilliers
 * @date   2022-5-6
 */

#ifndef REALPAVER_SCOPE_BANK_HPP
#define REALPAVER_SCOPE_BANK_HPP

#include <unordered_set>
#include "realpaver/Scope.hpp"

namespace realpaver {

/**
 * @brief Bank of scopes in order to share them.
 *
 * This class implements the Singleton pattern.
 */
class ScopeBank {
public:
   /// Returns the instance
   static ScopeBank* getInstance();

   /// Returns the number of scopes in this
   size_t size() const;

   /// Returns the number of scopes that have been inserted in this
   size_t nbInsertions() const;

   /**
    * @brief Inserts a scope in this.
    * 
    * If there is a scope S in this that is equal to scop then S is eturned,
    * scop is inserted and returned otherwise.
    */
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
