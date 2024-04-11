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
 * @file   ContractorHC4Revise.hpp
 * @brief  HC4Revise contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_HC4_REVISE_HPP
#define REALPAVER_CONTRACTOR_HC4_REVISE_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"

namespace realpaver {

/**
 * @brief HC4Revise contractor.
 *
 * This contractor traverses the tree-representation of a constraint.
 * The first phase is an interval evaluation from the leaves to the root.
 * The second phase calculates the projections from the root to the leaves.
 * The expression of the constraint comes from a DAG.
 */
class ContractorHC4Revise : public Contractor {
public:
   /**
    * @brief Constructor.
    * @param dag a DAG
    * @param i index of a function in the dag with 0 <= i < dag->nbFuns()
    */
   ContractorHC4Revise(SharedDag dag, size_t i);

   /// Default copy constructor
   ContractorHC4Revise(const ContractorHC4Revise&) = default;

   /// Default assignment operator
   ContractorHC4Revise& operator=(const ContractorHC4Revise&) = delete;

   /// Default destructor
   ~ContractorHC4Revise() = default;

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

   /// Returns the dag
   SharedDag getDag() const;

   /// Returns the function index in the dag
   size_t getFunIndex() const;

private:
   SharedDag dag_;   // dag
   size_t idx_;      // function index in the dag
};

} // namespace

#endif
