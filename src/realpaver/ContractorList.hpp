///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_LIST_HPP
#define REALPAVER_CONTRACTOR_LIST_HPP

#include "realpaver/ContractorPool.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor applying a group of contractors in sequence.
///////////////////////////////////////////////////////////////////////////////
class ContractorList : public Contractor {
public:
   /// Constructor
   /// @param pool a pool of contractors
   ContractorList(SharedContractorPool pool = nullptr);

   /// Default copy constructor
   ContractorList(const ContractorList&) = default;

   /// No assignment
   ContractorList& operator=(const ContractorList&) = delete;

   /// Default destructor
   ~ContractorList() = default;

   /// @return the number of contractors
   size_t poolSize() const;

   /// @return the pool
   SharedContractorPool getPool() const;

   /// Sets the pool
   /// @param pool new pool of contractors
   void setPool(SharedContractorPool pool);

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& B) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   SharedContractorPool pool_;
};

/// Type of shared pointers on list contractors
typedef std::shared_ptr<ContractorList> SharedContractorList;

} // namespace

#endif
