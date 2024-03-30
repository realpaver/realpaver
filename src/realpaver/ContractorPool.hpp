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
 * @file   ContractorPool.hpp
 * @brief  Classes of pools of contractoirs
 * @author Laurent Granvilliers
 * @date   2022-5-6
*/

#ifndef REALPAVER_CONTRACTOR_POOL_HPP
#define REALPAVER_CONTRACTOR_POOL_HPP

#include <memory>
#include "realpaver/Contractor.hpp"

namespace realpaver {

/**
 * @brief Base class for pools of contractors.
 *
 * A pool manages a set of contractors. Its scopes is the union of the scopes
 * of those contractors. The contractors must be accessed through shared
 * pointers.
 */
class ContractorPool {
public:
   /// Default constructor
   ContractorPool() = default;

   /// Default copy constructor
   ContractorPool(const ContractorPool&) = default;

   /// No assignment
   ContractorPool& operator=(const ContractorPool&) = delete;

   /// Virtual destructor
   virtual ~ContractorPool();

   /// Returns the number of contractors
   virtual size_t poolSize() const = 0;

   /// Returns the set of variables it depends on
   virtual Scope scope() const = 0;

   /// Inserts a contractor in this
   virtual void push(SharedContractor op) = 0;

   /// Gets the i-th contractor given 0 <= i < poolSize()
   virtual SharedContractor contractorAt(size_t i) const = 0;

   /// Removes the i-th contractor given 0 <= i < poolSize()
   virtual void removeContractorAt(size_t i) = 0;
};

/// Type of shared contractor pools
using SharedContractorPool = std::shared_ptr<ContractorPool>;

/*----------------------------------------------------------------------------*/

/// Pool of contractors implemented as a vector
class ContractorVector : public ContractorPool {
public:
   /// Creates an empty pool
   ContractorVector();

   /// Default copy constructor
   ContractorVector(const ContractorVector&) = default;

   /// No assignment
   ContractorVector& operator=(const ContractorVector&) = delete;

   /// Default destructor
   ~ContractorVector() = default;

   size_t poolSize() const override;
   Scope scope() const override;
   void push(SharedContractor op) override;
   SharedContractor contractorAt(size_t i) const override;
   void removeContractorAt(size_t i) override;

private:
   std::vector<SharedContractor> v_;
   Scope scop_;
};

/// Type of shared contractor vectors
using SharedContractorVector = std::shared_ptr<ContractorVector>;

} // namespace

#endif
