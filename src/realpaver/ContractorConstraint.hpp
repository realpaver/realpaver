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
 * @file   ContractorConstraint.hpp
 * @brief  Contractor associated with a constraint
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_CONSTRAINT_HPP
#define REALPAVER_CONTRACTOR_CONSTRAINT_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/Contractor.hpp"

namespace realpaver {

/**
 * @brief Contractor associated with a constraint.
 *
 * This contractor simply calls the default contraction of the constraint.
 */
class ContractorConstraint : public Contractor {
public:
   /// Constructor
   ContractorConstraint(Constraint c);

   /// Default copy constructor
   ContractorConstraint(const ContractorConstraint &) = default;

   /// No assignment
   ContractorConstraint &operator=(const ContractorConstraint &) = delete;

   /// Default destructor
   ~ContractorConstraint() = default;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

   /// Returns the constraint enclosed in this
   Constraint getConstraint() const;

private:
   Constraint c_;
};

} // namespace realpaver

#endif
