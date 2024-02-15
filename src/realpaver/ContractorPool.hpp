///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_POOL_HPP
#define REALPAVER_CONTRACTOR_POOL_HPP

#include <memory>
#include "realpaver/Contractor.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for pools of contractors.
///
/// A pool manages a set of contractors. Its scopes is the union of the scopes
/// of those contractors.
///
/// The contractors must be accessed through shared pointers.
///////////////////////////////////////////////////////////////////////////////
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

   /// @return the number of contractors
   virtual size_t poolSize() const = 0;

   /// @return the set of variables it depends on
   virtual Scope scope() const = 0;

   /// Inserts a contractor in this
   /// @param op a contractor
   virtual void push(SharedContractor op) = 0;

   /// Gets a contractor
   /// @param i an index between 0 and poolSize()-1
   /// @return the i-th contractor
   virtual SharedContractor contractorAt(size_t i) const = 0;

   /// Removes a contractor
   /// @param i an index between 0 and poolSize()-1
   virtual void removeContractorAt(size_t i) = 0;
};

/// Type of shared contractor pools.
typedef std::shared_ptr<ContractorPool> SharedContractorPool;

///////////////////////////////////////////////////////////////////////////////
/// This is a pool of contractors implemented as a vector.
///////////////////////////////////////////////////////////////////////////////
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

   ///@{
   size_t poolSize() const override;
   Scope scope() const override;
   void push(SharedContractor op) override;
   SharedContractor contractorAt(size_t i) const override;
   void removeContractorAt(size_t i) override;
   ///@}

private:
   std::vector<SharedContractor> v_;
   Scope scop_;
};

/// Type of shared contractor vectors.
typedef std::shared_ptr<ContractorVector> SharedContractorVector;

} // namespace

#endif
