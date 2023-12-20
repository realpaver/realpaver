///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_CONTRACTOR_CTR_HPP
#define REALPAVER_CONTRACTOR_CTR_HPP

#include "realpaver/Constraint.hpp"
#include "realpaver/Contractor.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a contractor associated with a constraint.
///
/// This contractor simply calls the default contraction of the constraint.
///////////////////////////////////////////////////////////////////////////////
class ContractorCtr : public Contractor {
public:
   /// Constructor
   /// @param c constraint of this
   ContractorCtr(Constraint c);

   /// Default copy constructor
   ContractorCtr(const ContractorCtr&) = default;

   /// No assignment
   ContractorCtr& operator=(const ContractorCtr&) = delete;

   /// Default destructor
   ~ContractorCtr() = default;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& box) override;
   void print(std::ostream& os) const override;
   ///@}

   /// @return the constraint enclosed in this
   Constraint getConstraint() const;

private:
   Constraint c_;
};

} // namespace

#endif
