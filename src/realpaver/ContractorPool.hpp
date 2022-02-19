///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_POOL_HPP
#define REALPAVER_CONTRACTOR_POOL_HPP

#include "realpaver/Contractor.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is an interface for pools of contractors.
///
/// A pool manages a set of contractors. Its scopes is the union of the scopes
/// of those contractors.
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

   /// Dependence test with respect to a set of variables
   /// @param bs a bitset for a set of variables
   /// @return true if a contractor from this depends on bs
   virtual bool dependsOn(const Bitset& bs) const = 0;

   /// Gets a contractor
   /// @param i an index between 0 and poolSize()-1
   /// @return the i-th contractor
   virtual SharedContractor contractorAt(size_t i) const = 0;

   /// Removes a contractor
   /// @param i an index between 0 and poolSize()-1
   virtual void removeContractorAt(size_t i) = 0;
};

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

   /// Inserts a contractor at the end of this
   /// @param op a contractor
   void push(const SharedContractor& op);

   ///@{
   /// Overrides the methods of ContractorPool
   size_t poolSize() const;
   Scope scope() const;
   bool dependsOn(const Bitset& bs) const;
   SharedContractor contractorAt(size_t i) const;
   void removeContractorAt(size_t i);
   ///@}

private:
   std::vector<SharedContractor> v_;
   Scope s_;
   Bitset bs_;
};

} // namespace

#endif
