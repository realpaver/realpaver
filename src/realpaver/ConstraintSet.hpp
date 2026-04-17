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
 * @file   ConstraintSet.hpp
 * @brief  Set of constraints
 * @author Laurent Granvilliers
 * @date   2 Apr 2026
 */

#ifndef REALPAVER_CONSTRAINT_SET_HPP
#define REALPAVER_CONSTRAINT_SET_HPP

#include "realpaver/Constraint.hpp"
#include <vector>

namespace realpaver {

/// Set of constraints
class ConstraintSet {
public:
   /// Default constructor of an empty set
   ConstraintSet() = default;

   // Default copy constructor
   ConstraintSet(const ConstraintSet &) = default;

   /// No assignment
   ConstraintSet &operator=(const ConstraintSet &) = delete;

   /// Default destructor
   ~ConstraintSet() = default;

   /// Inserts a constraint in this
   void insert(Constraint c);

   /// Returns the number of constraints in this
   size_t size() const;

   /// Returns the i-th constraint in this
   Constraint ctrAt(size_t i) const;

   /// Returns the scope of this
   Scope scope() const;

private:
   /// vector of constraints
   typedef std::vector<Constraint> VectorType;

   VectorType v_; // vector
   Scope scop_;   // scope

public:
   typedef VectorType::iterator iterator;
   typedef VectorType::const_iterator const_iterator;

   /// Returns an iterator on this
   iterator begin();

   /// Returns the end iterator on this
   iterator end();

   /// Returns an iterator on this
   const_iterator cbegin() const;

   /// Returns the end iterator on this
   const_iterator cend() const;
};

} // namespace realpaver

#endif
