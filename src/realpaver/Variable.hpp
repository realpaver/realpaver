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
 * @file   Variable.hpp
 * @brief  Variable in a problem
 * @author Laurent Granvilliers
 * @date   11 Apr 2024
 */

#ifndef REALPAVER_VARIABLE_HPP
#define REALPAVER_VARIABLE_HPP

#include "realpaver/Domain.hpp"
#include "realpaver/Tolerance.hpp"
#include <memory>

namespace realpaver {

/// Status of a variable
enum class VariableStatus {
   Decision,  ///< decision variable
   Objective, ///< variable representing an objective function
   Auxiliary  ///< auxiliary variable introduced e.g. by reformulations
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, VariableStatus status);

/*----------------------------------------------------------------------------*/

/// Shared representation of a variable in a problem.
class VariableRep {
public:
   /// Constructor
   VariableRep(const std::string &name);

   /// No copy
   VariableRep(const VariableRep &) = delete;

   /// No assignment
   VariableRep &operator=(const VariableRep &) = delete;

   /// Destructor
   ~VariableRep();

   /// Returns the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   void setId(size_t id);

   /// Returns the name of this
   std::string getName() const;

   /// Sets the name of this
   void setName(const std::string &name);

   /// Returns the domain of this
   Domain *getDomain() const;

   /// Sets the domain of this
   void setDomain(Domain *dom);

   /// Returns the status of this
   VariableStatus getStatus() const;

   /// Sets the status of this
   void setStatus(VariableStatus status);

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
   void setTolerance(const Tolerance &tol);

   /// Sets this as a variable whose domain can be split if splitable=true
   void setSplitable(bool splitable);

   /// Returns true if the domain of this variable  can be split
   bool isSplitable() const;

private:
   std::string name_;      // name
   size_t id_;             // identifier
   Domain *dom_;           // domain
   VariableStatus status_; // status
   Tolerance tol_;         // tolerance
   bool split_;            // false if this is not splitable

   static int NEXT_ID; // management of ids
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Variable in a problem.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 *
 * A variable has a name, a unique identifier, a domain, a status, a tolerance, and a
 * split status. The default status is Decision. The default tolerance is equal to 0.
 * The default split status is true (when it is false, the domain of a variable cannot
 * be split)
 */
class Variable {
public:
   /**
    * @brief Constructor.
    *
    * The default domain is the interval universe.
    */
   Variable(const std::string &name);

   /**
    * @brief Default constructor.
    *
    * It creates a variable having no representation. This constructor is useful
    * when a Variable object must be initialized without effectively creating a
    * new constrained variable.
    *
    * The resulting object v is such that v.hasNullRep() == true.
    */
   Variable();

   /// Default copy constructor
   Variable(const Variable &) = default;

   /// Default assignment operator
   Variable &operator=(const Variable &) = default;

   /// Default destructor
   ~Variable() = default;

   /// Returns the unique identifier of this
   size_t id() const;

   /// Sets the unique identifier of this
   Variable &setId(size_t id);

   /// Returns the name of this
   std::string getName() const;

   /// Sets the name of this
   Variable &setName(const std::string &name);

   /// Returns the domain of this
   Domain *getDomain() const;

   /**
    * @brief Sets the domain of this.
    *
    * throws an exception if dom is null
    */
   Variable &setDomain(std::unique_ptr<Domain> dom);

   /// Returns the status of this
   VariableStatus getStatus() const;

   /// Sets the status of this
   Variable &setStatus(VariableStatus status);

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
   Variable &setTolerance(const Tolerance &tol);

   /// Returns a clone of this with a new representation
   Variable clone() const;

   /// Tests the equality of two variables
   bool operator==(const Variable &other) const;

   /// Tests the difference of two variables
   bool operator!=(const Variable &other) const;

   /// Returns true if this has no representation (nullptr)
   bool hasNullRep() const;

   /// Sets this as a variable whose domain can be split if splitable=true
   Variable &setSplitable(bool splitable);

   /// Returns true if the domain of this variable  can be split
   bool isSplitable() const;

private:
   std::shared_ptr<VariableRep> rep_; // representation
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Variable &v);

/*----------------------------------------------------------------------------*/

/**
 * @brief Variable hasher.
 *
 * This implements a function call operator that simply returns the hash code
 * of a variable.
 */
struct VariableHasher {
   /// Hash function of v
   std::size_t operator()(const Variable &v) const
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
   bool operator()(const Variable &v, const Variable &w) const
   {
      return v.id() == w.id();
   }
};

} // namespace realpaver

#endif
