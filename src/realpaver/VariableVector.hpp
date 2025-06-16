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
 * @file   VariableVector.hpp
 * @brief  Vector of variables
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_VARIABLE_VECTOR_HPP
#define REALPAVER_VARIABLE_VECTOR_HPP

#include "realpaver/Variable.hpp"
#include <memory>
#include <vector>

namespace realpaver {

/// Representation of a vector of variables
class VariableVectorRep {
public:
   /**
    * @brief Constructor.
    *
    * @param name base name
    * @param first index of the first variable in this
    * @param last index of the last variable in this
    */
   VariableVectorRep(const std::string &name, int first, int last);

   /// Default destructor
   ~VariableVectorRep() = default;

   /// No copy
   VariableVectorRep(const VariableVectorRep &) = delete;

   /// No assignment
   VariableVectorRep &operator=(const VariableVectorRep &) = delete;

   /// Output on a stream
   void print(std::ostream &os) const;

   /// Returns the size of this
   int size() const;

   /// Returns the index of the first variable of this
   int firstIndex() const;

   /// Returns the index of the last variable of this
   int lastIndex() const;

   /// Gets the variable at index i in this
   Variable get(int i) const;

   /// Sets the tolerance of all the variables of this
   void setTolerance(const Tolerance &tol);

   /// Returns the hash code of this
   size_t hashCode() const;

private:
   std::string name_;           // base name
   int first_, last_;           // bounds of the variable indexes
   std::vector<Variable> vars_; // the variables
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Vector of variables.
 *
 * This encloses a shared pointer to its representation. It is a lightweight
 * object that can be copied and assigned.
 */
class VariableVector {
public:
   /**
    * @brief Constructor.
    *
    * @param name base name
    * @param first index of the first variable in this
    * @param last index of the last variable in this
    */
   VariableVector(const std::string &name, int first, int last);

   /// Default copy constructor
   VariableVector(const VariableVector &) = default;

   /// Default assignment
   VariableVector &operator=(const VariableVector &) = default;

   /// Default destructor
   ~VariableVector() = default;

   /// Output on a stream
   void print(std::ostream &os) const;

   /// Returns the size of this
   int size() const;

   /// Returns the index of the first variable of this
   int firstIndex() const;

   /// Returns the index of the last variable of this
   int lastIndex() const;

   /// Gets the variable at index i in this
   Variable get(int i) const;

   /// Gets the variable at index i in this
   Variable operator[](int i) const;

   /// Sets the tolerance of all the variables of this
   VariableVector &setTolerance(const Tolerance &tol);

   /// Returns the hash code of this
   size_t hashCode() const;

private:
   std::shared_ptr<VariableVectorRep> rep_;
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const VariableVector &v);

} // namespace realpaver

#endif
