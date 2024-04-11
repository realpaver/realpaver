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
 * @file   Variables.hpp
 * @brief  Variable in a problem
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_VARIABLE_HPP
#define REALPAVER_VARIABLE_HPP

#include <memory>
#include "realpaver/Domain.hpp"
#include "realpaver/Tolerance.hpp"

namespace realpaver {

/// Shared representation of a variable in a problem.
class VariableRep {
public:
   /// Constructor
   VariableRep(const std::string& name);

   /// No copy
   VariableRep(const VariableRep&) = delete;

   /// No assignment
   VariableRep& operator=(const VariableRep&) = delete;

   /// Destructor
   ~VariableRep();

   /// Returns the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   void setId(size_t id);

   /// Returns the name of this
   std::string getName() const;

   /// Sets the name of this
   void setName(const std::string& name);

   /// Returns the domain of this
   Domain* getDomain() const;

   /// Sets the domain of this
   void setDomain(Domain* dom);

   /// Returns true if this is a binary variable, false otherwise
   bool isBinary() const;

   /// Returns true if this is an integer variable, false otherwise
   bool isInteger() const;

   /// Returns true if this is a real variable, false otherwise
   bool isReal() const;

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns the tolerance of this (output precision of interval solutions)
   Tolerance getTolerance() const;

   /// Sets the tolerance of this
   void setTolerance(const Tolerance& tol);

private:
   std::string name_;   // name
   size_t id_;          // identifier
   Domain* dom_;        // domain
   Tolerance tol_;      // tolerance

   static int NEXT_ID;  // management of ids
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Variable in a problem.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class Variable {
public:
   /**
    * @brief Constructor.
    *
    * The default domain is the interval universe.
    */
   Variable(const std::string& name);

   /// Creates a variable having no representation
   Variable();

   /// Default copy constructor
   Variable(const Variable&) = default;

   /// Default assignment operator
   Variable& operator=(const Variable&) = default;

   /// Default destructor
   ~Variable() = default;

   /// Returns the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   Variable& setId(size_t id);

   /// Returns the name of this
   std::string getName() const;

   /// Sets the name of this
   Variable& setName(const std::string& name);

   /// Returns the domain of this
   Domain* getDomain() const;

   /**
    * @brief Sets the domain of this.
    *
    * throws an exception if dom is null
    */
   Variable& setDomain(std::unique_ptr<Domain> dom);

   /// Returns true if this is an integer variable in [0, 1]
   bool isBinary() const;

   /// Returns true if this is an integer variable, false otherwise
   bool isInteger() const;

   /// Returns true if this is a real variable, false otherwise
   bool isReal() const;

   /// Returns the hash code of this
   size_t hashCode() const;

   /// Returns the tolerance of this (output precision of interval solutions)
   Tolerance getTolerance() const;

   /// Sets the tolerance of this
   Variable& setTolerance(const Tolerance& tol);

   /// Returns a clone of this with a new representation
   Variable clone() const;

   /// Tests the equality of two variables
   bool operator==(const Variable& other) const;

   /// Returns true if this has no representation (nullptr)
   bool hasNullPointer() const;

private:
   std::shared_ptr<VariableRep> rep_;  // representation
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Variable& v);

/*----------------------------------------------------------------------------*/

/**
 * @brief Variable hasher.
 *
 * This implements a function call operator that simply returns the hash code
 * of a variable.
 */
struct VariableHasher {
   /// Hash function of v
   std::size_t operator()(const Variable& v) const
   {
      return v.hashCode();
   }
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Equality comparator of variables.
 *
 * This implements a function call operator that returns true if two
 * variables have the same id.
 */
struct VariableEqual {
   /// Returns true if v and w represent the same variable
   bool operator()(const Variable& v, const Variable& w) const
   {
      return v.id() == w.id();
   }
};

} // namespace

#endif
