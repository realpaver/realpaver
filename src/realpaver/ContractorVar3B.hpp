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
 * @file   ContractorVar3B.hpp
 * @brief  3B contractor
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_VAR_3B_HPP
#define REALPAVER_CONTRACTOR_VAR_3B_HPP

#include "realpaver/Contractor.hpp"
#include "realpaver/IntervalSlicer.hpp"

namespace realpaver {

/**
 * @brief Contractor implementing 3B consistency.
 *
 * Given a box B, a variable v, a slicer and a contractor, the domain of
 * v in B is divided by the slicer. The resulting domain is the hull of the
 * outermost consistent slices. The scope of this corresponds to the scope of
 * the given contractor.
 */
class ContractorVar3B : public Contractor {
public:
   /**
    * @brief Constructor.
    * @param op a contractor
    * @param v a variable
    * @param slicer a slicer
    */
   ContractorVar3B(SharedContractor op, Variable v,
                   std::unique_ptr<IntervalSlicer> slicer);

   /**
    * @brief Constructor.
    * @param op a contractor
    * @param v a variable
    * @param n number of slices (at least 2)
    */
   ContractorVar3B(SharedContractor op, Variable v, size_t n);

   /// Default destructor
   ~ContractorVar3B() = default;

   /// No copy
   ContractorVar3B(const ContractorVar3B&) = delete;

   /// No assignment
   ContractorVar3B& operator=(const ContractorVar3B&) = delete;

   /// Returns the variable whose domain is sliced
   Variable getVar() const;

   /// Assigns the variable whose domain is sliced
   void setVar(Variable v);

   /// Returns the threshold on the width of a variable domain
   double varMinWidth() const;

   /// Assigns the threshold on the width of a variable domain
   void setVarMinWidth(double val);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   SharedContractor op_;                     // contractors for slices
   Variable v_;                              // variable whose domain is sliced
   std::unique_ptr<IntervalSlicer> slicer_;  // slicer
   double var_min_width_;                    // threshold
   Interval left_, right_;                   // left and right consistent slices
                                             // in the last call of contract
};

} // namespace

#endif
