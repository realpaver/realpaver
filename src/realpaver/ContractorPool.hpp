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
 * @date   2024-4-11
*/

#ifndef REALPAVER_CONTRACTOR_POOL_HPP
#define REALPAVER_CONTRACTOR_POOL_HPP

#include <memory>
#include "realpaver/Contractor.hpp"

namespace realpaver {

/**
 * @brief Pool of contractors.
 *
 * A pool manages a set of contractors. Its scopes is the union of the scopes
 * of those contractors. The contractors must be accessed through shared
 * pointers.
 */
class ContractorPool {
public:
   /// Creates an empty pool
   ContractorPool();

   /// Default copy constructor
   ContractorPool(const ContractorPool&) = default;

   /// No assignment
   ContractorPool& operator=(const ContractorPool&) = delete;

   /// Default destructor
   ~ContractorPool() = default;

   /// Returns the number of contractors
   size_t poolSize() const;

   /// Returns the scope of this
   Scope scope() const;

   /// Inserts a contractor
   void push(SharedContractor op);

   /// Gets the i-th contractor
   SharedContractor contractorAt(size_t i) const;

   /// Returns the number of contractors depending on v
   size_t depSize(const Variable& v) const;

   /// Returns the index in this pool of the i-th contractor depending on v
   size_t depAt(const Variable& v, size_t i) const;

   /// Output on a stream
   void print(std::ostream& os) const;

private:
   std::vector<SharedContractor> v_;
   Scope scop_;

   // dependency structure
   // what is dep_[i] ? i with 0 <= i < scop_ < size() is a variable index in
   // scop_ and dep_[i] is a list of indexes of contractors in v_ that depend
   // on this variable
   std::vector<std::vector<size_t>> dep_;

   // makes the dependency relation between the variables and the contractors
   void makeDep();
};

/// Type of shared contractor pools
using SharedContractorPool = std::shared_ptr<ContractorPool>;

} // namespace

#endif
