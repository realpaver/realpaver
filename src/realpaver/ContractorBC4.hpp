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
 * @file   ContractorBC4.hpp
 * @brief  BC4 contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_BC4_HPP
#define REALPAVER_CONTRACTOR_BC4_HPP

#include "realpaver/ContractorBC4Revise.hpp"
#include "realpaver/Dag.hpp"
#include "realpaver/IntervalPropagator.hpp"

namespace realpaver {

/**
 * @brief Propagation algorithm implementing the BC4 strategy.
 *
 * A BC4Revise contractor is associated with each constraint from a DAG. The
 * contraction method implements an AC3-like propagation algorithm on the set
 * of contractors.
 */
class ContractorBC4 : public Contractor {
public:
   /// Constructor
   ContractorBC4(SharedDag dag);

   /// Destructor
   ~ContractorBC4();

   /// No copy
   ContractorBC4(const ContractorBC4&) = delete;

   /// No asignment
   ContractorBC4& operator=(const ContractorBC4&) = delete;

   /// Inserts a contractor in this
   void push(SharedContractor op);

   /// Returns the tolerance used as stopping criterion
   double getTol() const;

   /// Sets the tolerance used as stopping criterion
   void setTol(double tol);

   /**
    * @brief Sets the peel factor of the BC4Revise operators.
    *
    * Condition: 0.0 <= f <= 100.0
    */
   void setBC4RevisePeelFactor(double f);

   /// Sets the maximum number of steps in the BC4Revise operators
   void setBC4ReviseMaxIter(size_t val);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   SharedDag dag_;               // DAG
   IntervalPropagator* propag_;  // propagation algorithm
   std::vector<std::shared_ptr<ContractorBC4Revise>> vop_;  // contractors
};

/// Type of shared pointers to BC4 contractors
using SharedContractorBC4 = std::shared_ptr<ContractorBC4>;

} // namespace

#endif
