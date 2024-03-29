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
 * @date   2022-5-6
*/

#ifndef REALPAVER_CONTRACTOR_HPP
#define REALPAVER_CONTRACTOR_HPP

#include <memory>
#include "realpaver/IntervalBox.hpp"
#include "realpaver/Scope.hpp"

namespace realpaver {

/**
 * @brief Base class of interval contractors
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
   /** @brief Default constructor */
   Contractor() = default;

   /** @brief Default copy constructor */
   Contractor(const Contractor&) = default;

   /** @brief No assignment */
   Contractor& operator=(const Contractor&) = delete;

   /** @brief Virtual destructor */
   virtual ~Contractor();

   /**
    * @brief Dependency test
    * @param v a variable
    * @return true if the scope of this contains v
    */
   bool dependsOn(const Variable& v) const;

   /** @brief @return the set of variable on which this depends on */
   virtual Scope scope() const = 0;

   /**
    * @brief Contraction method
    * @param B interval box that is contracted
    * @return a certificate of proof
    */
   virtual Proof contract(IntervalBox& B) = 0;

   /** @brief Output on a stream */
   virtual void print(std::ostream& os) const;
};

/** @brief Output on a stream */
std::ostream& operator<<(std::ostream& os, const Contractor& op);

/** @brief Type of shared pointers on contractors */
typedef std::shared_ptr<Contractor> SharedContractor;

} // namespace

#endif
