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
 * @file   ContractorAffine.hpp
 * @brief  Propagation algorithm based on affine contractors
 * @author Laurent Granvilliers
 * @date   23 Apr 2025
 */

#ifndef REALPAVER_CONTRACTOR_AFFINE_HPP
#define REALPAVER_CONTRACTOR_AFFINE_HPP

#include "realpaver/Dag.hpp"
#include "realpaver/IntervalPropagator.hpp"

namespace realpaver {

/**
 * @brief HC4 contractor.
 *
 * Constraint propagation algorithm applying affine contractors on a DAG.
 */
class ContractorAffine : public Contractor {
public:
   /**
    * @brief Constructor given a DAG.
    *
    * The boolean minrage is true if the linearization of the elementary
    * functions must use the Minrange approximation, false for the Chebyshev
    * approximation.
    */
   ContractorAffine(SharedDag dag, bool minrange = true);

   /// Destructor
   ~ContractorAffine();

   /// No copy
   ContractorAffine(const ContractorAffine &) = delete;

   /// No asignment
   ContractorAffine &operator=(const ContractorAffine &) = delete;

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
using SharedContractorAffine = std::shared_ptr<ContractorAffine>;

} // namespace realpaver

#endif
