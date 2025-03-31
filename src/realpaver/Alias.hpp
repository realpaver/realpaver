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
 * @file   Alias.hpp
 * @brief  Alias of term in a problem
 * @author Laurent Granvilliers
 * @date   2024-12-19
 */

#ifndef REALPAVER_ALIAS_HPP
#define REALPAVER_ALIAS_HPP

#include "realpaver/Term.hpp"
#include <memory>
#include <string>

namespace realpaver {

/// Shared representation of an alias in a problem.
class AliasRep {
public:
   /**
    * @brief Constructor.
    *
    * throws an exception if t has no variable.
    */
   AliasRep(const std::string &name, Term t);

   /// No copy
   AliasRep(const AliasRep &) = delete;

   /// No assignment
   AliasRep &operator=(const AliasRep &) = delete;

   /// Default destructor
   ~AliasRep() = default;

   /// Returns the name of this
   std::string name() const;

   /// Returns the term of this
   Term term() const;

   /// Returns the scope of this
   Scope scope() const;

private:
   std::string name_; // name
   Term t_;           // term or expression
   Scope scop_;       // scope of term
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Alias in a problem.
 *
 * An alias associates a name with a term. It encloses a shared pointer to its
 * representation. It is a lightweight object that can be copied and assigned.
 */
class Alias {
public:
   /**
    * @brief Constructor.
    *
    * throws an exception if t has no variable.
    */
   Alias(const std::string &name, Term t);

   /// Default copy constructor
   Alias(const Alias &) = default;

   /// Default assignment operator
   Alias &operator=(const Alias &) = default;

   /// Default destructor
   ~Alias() = default;

   /// Returns the name of this
   std::string name() const;

   /// Returns the term of this
   Term term() const;

   /// Returns the scope of this
   Scope scope() const;

private:
   std::shared_ptr<AliasRep> rep_; // representation
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Alias &a);

} // namespace realpaver

#endif
