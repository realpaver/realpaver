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
 * @file   ContractorAffineRevise.hpp
 * @brief  Contractor based on affine forms
 * @author Laurent Granvilliers
 * @date   21 Apr 2025
 */

#ifndef REALPAVER_CONTRACTOR_AFFINE_REVISE_HPP
#define REALPAVER_CONTRACTOR_AFFINE_REVISE_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/Dag.hpp"

namespace realpaver {

/**
 * @brief Contractor based on affine forms.
 *
 * Let 'f in I' be a function of a DAG and let B be a box. Let a0 + sum_i a_i e_i + E
 * be the affine form of f in B where E is the error term and each e_i lies in [-1, 1].
 * It follows the constraint sum_i a_i e_i in J with J = I - a0 + [-E, E].
 *
 * The algorithm is as follows. We first calculate s = sum_i |a_i|. Let S be the interval
 * [-s, s]. If S is contained in J then B is an inner box. If S and J are disjoint then
 * there is no solution. Otherwise, we try to reduce the domain of ei. For each i, let
 * si = s - |ai|. The new domain of ei is [-1, 1] inter (J + [-si, si]) / ai. The new
 * domain of xi is obtained from its affine form evaluated with the domain of ei.
 */
class ContractorAffineRevise : public Contractor {
public:
   /**
    * @brief Creates a contractor on the i-th function of the DAG.
    *
    * The boolean minrage is true if the linearization of the elementary
    * functions must use the Minrange approximation, false for the Chebyshev
    * approximation.
    */
   ContractorAffineRevise(SharedDag dag, size_t i, bool minrange = true);

   /// Default copy constructor
   ContractorAffineRevise(const ContractorAffineRevise &) = default;

   /// Default assignment operator
   ContractorAffineRevise &operator=(const ContractorAffineRevise &) = delete;

   /// Default destructor
   ~ContractorAffineRevise() = default;

   Scope scope() const override;
   Proof contract(IntervalBox &B) override;
   void print(std::ostream &os) const override;

   /// Returns the dag
   SharedDag getDag() const;

   /// Returns the function index in the dag
   size_t getFunIndex() const;

   /// Selects the Minrange or the Chebyshev approximation
   void useMinrange(bool minrange = true);

private:
   SharedDag dag_; // dag
   size_t idx_;    // function index in the dag
   bool minrange_; // true: Minrange, false:Chebyshev
};

} // namespace realpaver

#endif
