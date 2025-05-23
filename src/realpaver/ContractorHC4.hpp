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
 * @file   ContractorHC4.hpp
 * @brief  HC4 contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
 */

#ifndef REALPAVER_CONTRACTOR_HC4_HPP
#define REALPAVER_CONTRACTOR_HC4_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalPropagator.hpp"

namespace realpaver {

/**
 * @brief HC4 contractor.
 *
 * Constraint propagation algorithm applying HC4Revise contractors on a DAG
 * such that the projections are intersected at the shared nodes.
 */
class ContractorHC4 : public Contractor {
public:
   /// Constructor
   ContractorHC4(SharedDag dag);

   /// Destructor
   ~ContractorHC4();

   /// No copy
   ContractorHC4(const ContractorHC4 &) = delete;

   /// No asignment
   ContractorHC4 &operator=(const ContractorHC4 &) = delete;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

   /// Inserts a contractor in this
   void push(SharedContractor op);

   /// Returns the tolerance used as stopping criterion
   double getTol() const;

   /// Sets the tolerance used as stopping criterion
   void setTol(double tol);

private:
   SharedDag dag_;              // DAG
   IntervalPropagator *propag_; // propagation algorithm
};

/// Type of shared pointers of HC4 contractors
using SharedContractorHC4 = std::shared_ptr<ContractorHC4>;

} // namespace realpaver

#endif
