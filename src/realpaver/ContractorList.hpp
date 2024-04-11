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
 * @file   ContractorList.hpp
 * @brief  List of contractors applied in sequence
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_LIST_HPP
#define REALPAVER_CONTRACTOR_LIST_HPP

#include "realpaver/ContractorPool.hpp"

namespace realpaver {

/// Contractor applying a list of contractors in sequence
class ContractorList : public Contractor {
public:
   /// Constructor
   ContractorList(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   ContractorList(const ContractorList&) = default;

   /// No assignment
   ContractorList& operator=(const ContractorList&) = delete;

   /// Default destructor
   ~ContractorList() = default;

   /// Returns the number of contractors
   size_t poolSize() const;

   /// Returns the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   void setPool(SharedContractorPool pool);

   /// Inserts a contractor at the end of this
   void push(SharedContractor op);

   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;

private:
   SharedContractorPool pool_;
};

/// Type of shared pointers on list contractors
using SharedContractorList = std::shared_ptr<ContractorList>;

} // namespace

#endif
