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
 * @file   ContractorLoop.hpp
 * @brief  Loop on a contractor
 * @author Laurent Granvilliers
 * @date   4-3-2025
*/

#ifndef REALPAVER_CONTRACTOR_LOOP_HPP
#define REALPAVER_CONTRACTOR_LOOP_HPP

#include "realpaver/Contractor.hpp"

namespace realpaver {

/**
 * @brief Contractor implementing a loop on a contractor.
 *
 *
 */
class ContractorLoop : public Contractor {
public:
   ContractorLoop(SharedContractor op);

   /// Default destructor
   ~ContractorLoop() = default;

   /// No copy
   ContractorLoop(const ContractorLoop&) = delete;

   /// No assignment
   ContractorLoop& operator=(const ContractorLoop&) = delete;

   /// Returns the relative tolerance
   double getTol() const;

   /// Assigns the relative tolerance
   void setTol(double tol);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   SharedContractor op_;   // operator
   double tol_;            // relative tolerance
};

} // namespace

#endif
