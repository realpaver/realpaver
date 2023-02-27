///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_LIST_CONTRACTOR_HPP
#define REALPAVER_LIST_CONTRACTOR_HPP

#include "realpaver/ContractorPool.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor applying a group of contractors in sequence.
///////////////////////////////////////////////////////////////////////////////
class ListContractor : public Contractor {
public:
   /// Constructor
   /// @param pool a pool of contractors
   ListContractor(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   ListContractor(const ListContractor&) = default;

   /// No assignment
   ListContractor& operator=(const ListContractor&) = delete;

   /// Default destructor
   ~ListContractor() = default;

   /// @return the number of contractors
   size_t poolSize() const;

   /// @return the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   /// @param pool new pool of contractors
   void setPool(SharedContractorPool pool);

   ///@{
   bool dependsOn(const Bitset& bs) const override;
   Scope scope() const override;
   Proof contract(IntervalRegion& reg) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   SharedContractorPool pool_;
};

/// Type of shared pointers on list contractors
typedef std::shared_ptr<ListContractor> SharedListContractor;

} // namespace

#endif
