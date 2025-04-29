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
 * @file   Contractor.hpp
 * @brief  Base class of interval contractors
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_HPP
#define REALPAVER_CONTRACTOR_HPP

#include "realpaver/IntervalBox.hpp"
#include "realpaver/Scope.hpp"
#include <memory>

namespace realpaver {

/**
 * @brief Base class of interval contractors.
 *
 * An interval contractor is in general associated with a constraint. Given an
 * interval box, it removes infeasible facets (it prunes interval bounds) and
 * returns a certificate of proof:
 * - Proof::Empty if there is no solution;
 * - Proof::Feasible if it is proved that there is a solution;
 * - Proof::Inner if the vector is included in the feasible set;
 * - Proof::Maybe otherwise, i.e. no proof is done.
 *
 * This class is a pure abstract class.
 */
class Contractor {
public:
   /// Default constructor
   Contractor() = default;

   /// Default copy constructor
   Contractor(const Contractor &) = default;

   /// No assignment
   Contractor &operator=(const Contractor &) = delete;

   /// Virtual destructor
   virtual ~Contractor();

   /// Returns true if v belongs to the scope of this
   bool dependsOn(const Variable &v) const;

   /// Returns the set of variable on which this depends on
   virtual Scope scope() const = 0;

   /// Contracts a box B and returns a certificate of proof
   virtual Proof contract(IntervalBox &B) = 0;

   /// Output on a stream
   virtual void print(std::ostream &os) const;
};

/// Output on a stream
std::ostream &operator<<(std::ostream &os, const Contractor &op);

/// Type of shared pointers on contractors
using SharedContractor = std::shared_ptr<Contractor>;

} // namespace realpaver

#endif
