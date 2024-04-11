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
 * @file   ContractorVarCID.hpp
 * @brief  CID contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_VAR_CID_HPP
#define REALPAVER_CONTRACTOR_VAR_CID_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

/**
 * @brief Contractor implementing Constructive Interval Disjunction.
 *
 * Given a box B, a variable v, a slicer and a contractor, the domain of
 * v in B is divided by the slicer, each slice is reduced by the contractor,
 * and the hull of the contracted slices is returned.
 *
 * The scope of this corresponds to the scope of the given contractor.
 */
class ContractorVarCID : public Contractor {
public:
   /// Constructor given a slicer
   ContractorVarCID(SharedContractor op, Variable v,
                    std::unique_ptr<IntervalSlicer> slicer);

   /// Constructor given a number of slices (at least 2)
   ContractorVarCID(SharedContractor op, Variable v, size_t n);

   /// Default destructor
   ~ContractorVarCID() = default;

   /// No copy
   ContractorVarCID(const ContractorVarCID&) = delete;

   /// No assignment
   ContractorVarCID& operator=(const ContractorVarCID&) = delete;

   /// Returns the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   void setVar(Variable v);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   SharedContractor op_;
   Variable v_;
   std::unique_ptr<IntervalSlicer> slicer_;
};

} // namespace

#endif
