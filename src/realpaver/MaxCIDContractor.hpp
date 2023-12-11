///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2023 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_MAX_CID_CONTRACTOR_HPP
#define REALPAVER_MAX_CID_CONTRACTOR_HPP

#include "realpaver/CIDContractor.hpp"
#include "realpaver/VariableSelector.hpp"

namespace realpaver {

///////////////////////////////////////////////////////////////////////////////
/// This is a CID contractor applied to the variable having the largest domain.
///////////////////////////////////////////////////////////////////////////////
class MaxCIDContractor : public Contractor {
public:
   /// Creates a contractor
   /// @param op a contractor
   /// @param slicer object used to slice a variable domain
   MaxCIDContractor(SharedContractor op,
                    std::unique_ptr<IntervalSlicer> slicer);

   /// Destructor
   ~MaxCIDContractor();

   /// No copy
   MaxCIDContractor(const MaxCIDContractor&) = delete;

   /// No assignment
   MaxCIDContractor& operator=(const MaxCIDContractor&) = delete;

   ///@{
   Scope scope() const override;
   Proof contract(IntervalBox& box) override;
   void print(std::ostream& os) const override;
   ///@}

private:
   CIDContractor* op_;

   Variable selectMaxDom(const IntervalBox& box);
};

} // namespace

#endif
